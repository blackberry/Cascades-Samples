/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.See the License for the specific
 * language governing permissions and limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/XmlDataModel>
#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>

using namespace bb::cascades;

ApplicationUI::ApplicationUI ( bb::cascades::Application *app ) :
        QObject(app)
{
    // Prepare the localization
    this->m_pTranslator = new QTranslator(this);
    this->m_pLocaleHandler = new LocaleHandler(this);

    bool result;

    // Since the variable is not used in the app,
    // this is added to avoid a compiler warning
    Q_UNUSED(result);

    result = QObject::connect(this->m_pLocaleHandler,
            SIGNAL(systemLanguageChanged()), this,
            SLOT(onSystemLanguageChanged()));
    // Q_ASSERT is only available in Debug builds
    Q_ASSERT(result);

    // For localization
    this->onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject < AbstractPane >();

    // Get a handle to the UI controls.
    this->m_pNetwrkConnIcon = root->findChild < ImageView* >("netConnDot");
    this->m_pNetwrkIntrfcSymb = root->findChild < ImageView* >(
            "netConnTypeIcon");
    this->m_pListView = root->findChild < ListView* >("list");

    // Set created root object as the application scene
    app->setScene(root);

    // Initialize member variables
    this->m_pNetConfigMngr = new QNetworkConfigurationManager();
    this->m_pNetAccessMngr = new QNetworkAccessManager(this);
    this->m_pNetReply = NULL;
    this->m_pCurrentDialog = NULL;
    this->m_ConnectionRetries = 0;
    this->m_FileOpenRetries = 0;
    this->m_RetryDialogIsDisplayed = false;

    // Create a file in the device file system to save the data model
    this->m_pFileObj = new QFile("data/model.xml");

    this->m_CurrentInterface =
            this->m_pNetAccessMngr->activeConfiguration().bearerTypeName();

    result = QObject::connect(this->m_pNetConfigMngr,
            SIGNAL(onlineStateChanged(bool)),
            this,
            SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(result);

    result = QObject::connect(this,
            SIGNAL(onlineStateChanged(bool)),
            this,
            SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(result);

    result = QObject::connect(this->m_pNetAccessMngr,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(onRequestFinished()));
    Q_ASSERT(result);

    result = QObject::connect(this,
            SIGNAL(networkConnectionFailed()),
            this,
            SLOT(onNetworkConnectionFailed()));
    Q_ASSERT(result);

    result = QObject::connect(this,
            SIGNAL(fileOpenFailed()),
            this,
            SLOT(onFileOpenFailed()));
    Q_ASSERT(result);

    result = QObject::connect(this,
            SIGNAL(updateListView()),
            this,
            SLOT(onUpdateListView()));
    Q_ASSERT(result);

    result = QObject::connect(this,
            SIGNAL(sendNewRequest()),
            this,
            SLOT(onSendNewRequest()));
    Q_ASSERT(result);

    this->checkConnection();
}

// Sets the network connection status icon shown in the UI
void ApplicationUI::onOnlineStateChanged ( bool isOnline )
{
    QUrl connectionIconUrl;

    if (isOnline)
    {
        if (m_RetryDialogIsDisplayed)
        {
            this->m_pCurrentDialog->cancel();
            this->m_ConnectionRetries = 0;
        }

        connectionIconUrl = "asset:///images/greenDot.png";

        // Get the current interface name as a string
        this->m_CurrentInterface =
                this->m_pNetAccessMngr->activeConfiguration().bearerTypeName();

        emit sendNewRequest();
    }
    else
    {
        connectionIconUrl = "asset:///images/redDot.png";

        this->m_CurrentInterface = "";
        emit networkConnectionFailed();
    }

    this->m_pNetwrkConnIcon->setImageSource(connectionIconUrl);

    // Refresh the interface icon
    this->refreshInterface(this->m_CurrentInterface);
}

// Changes the network interface icon shown in the UI
void ApplicationUI::refreshInterface ( QString interfaceTypeName )
{
    if (interfaceTypeName == "Ethernet")
    {
        this->m_pNetwrkIntrfcSymb->setImageSource(
                QUrl("asset:///images/wired.png"));
    }
    else if (interfaceTypeName == "WLAN" || interfaceTypeName == "WiMAX")
    {
        this->m_pNetwrkIntrfcSymb->setImageSource(
                QUrl("asset:///images/wifi.png"));
    }
    else if (interfaceTypeName == "2G" || interfaceTypeName == "CDMA2000"
            || interfaceTypeName == "WCDMA" || interfaceTypeName == "HSPA")
    {
        this->m_pNetwrkIntrfcSymb->setImageSource(
                QUrl("asset:///images/cellular.png"));
    }
    else if (interfaceTypeName == "Bluetooth")
    {
        this->m_pNetwrkIntrfcSymb->setImageSource(
                QUrl("asset:///images/bluetooth.png"));
    }
    else
    {
        this->m_pNetwrkIntrfcSymb->setImageSource(
                QUrl("asset:///images/unknown.png"));
    }

    this->m_pNetConfigMngr->updateConfigurations();
}

void ApplicationUI::onRequestFinished ()
{
    QNetworkReply::NetworkError netError = this->m_pNetReply->error();

    if (netError == QNetworkReply::NoError)
    {
        emit updateListView();
    }
    else
    {
        switch (netError)
        {
            case QNetworkReply::ContentNotFoundError:
                qDebug() << "The content was not found on the server";
                break;

            case QNetworkReply::HostNotFoundError:
                qDebug() << "The server was not found";
                break;

            case QNetworkReply::AuthenticationRequiredError:
                qDebug() << "Server requires authentication";
                break;

            default:
                qDebug() << this->m_pNetReply->errorString();
                break;
        }
    }

    this->m_pNetReply->deleteLater();
}

void ApplicationUI::onSendNewRequest ()
{
    // Create and send the network request
    QNetworkRequest request = QNetworkRequest();
    QString requestUrl = "https://developer.blackberry.com";
    requestUrl.append("/native/files/documentation");
    requestUrl.append("/cascades/images/model.xml");

    request.setUrl(QUrl(requestUrl));

    // Send the network request
    this->m_pNetReply = m_pNetAccessMngr->get(request);

    // Show download progress
    bool result = QObject::connect(this->m_pNetReply,
            SIGNAL(downloadProgress(qint64, qint64)), this,
            SLOT(onDownloadProgress(qint64, qint64)));

    Q_ASSERT(result);
    Q_UNUSED(result);
}

// Updates the ListView control
void ApplicationUI::onUpdateListView ()
{
    if (this->m_pFileObj->open(QIODevice::ReadWrite))
    {
        // Write to the file using the pNetReply
        this->m_pFileObj->write(this->m_pNetReply->readAll());
        this->m_pFileObj->flush();
        this->m_pFileObj->close();

        // Create a temporary data model using the contents
        // of a local XML file
        XmlDataModel* pDataModel = new XmlDataModel();
        QUrl fileUrl = "file://" + QDir::homePath() + "/model.xml";
        pDataModel->setSource(fileUrl);

        this->m_pListView->setDataModel(pDataModel);
    }
    else
    {
        emit fileOpenFailed();
    }
}

void ApplicationUI::onFileOpenFailed ()
{
    QString btnMsg = "Retry " + QString::number(this->m_FileOpenRetries)
            + " of 3";

    SystemDialog* fileOpenDialog = new SystemDialog(btnMsg, "Cancel");
    this->m_FileOpenRetries += 1;

    fileOpenDialog->setTitle("File access error");
    fileOpenDialog->setBody("File failed to open");

    bool result =
            QObject::connect(fileOpenDialog,
                    SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                    SLOT(onFileOpenDialogFinished(bb::system::SystemUiResult::Type)));

    Q_ASSERT(result);
    Q_UNUSED(result);

    fileOpenDialog->show();
}

void ApplicationUI::onFileOpenDialogFinished (
        bb::system::SystemUiResult::Type type )
{
    if (type == SystemUiResult::ButtonSelection && this->m_FileOpenRetries < 3)
    {
        emit updateListView();
    }
    else
    {
        this->m_FileOpenRetries = 0;
        SystemToast* exitFileMsg = new SystemToast();
        QString exitMsg = "The app could not open the necessary file needed ";
        exitMsg.append("to update the list data, and will exit");
        exitFileMsg->setBody(exitMsg);
        exitFileMsg->setPosition(SystemUiPosition::MiddleCenter);
        exitFileMsg->show();

        bool result = QObject::connect(exitFileMsg,
                SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                SLOT(onFatalError()));

        Q_ASSERT(result);
        Q_UNUSED(result);
    }
}

bool ApplicationUI::checkConnection ()
{
    // Was the connection re-established
    if (this->m_pNetConfigMngr->isOnline() == true
            || this->m_CurrentInterface.isEmpty() == false)
    {
        this->m_ConnectionRetries = 0;
        emit onlineStateChanged(true);

        return true;
    }
    else
    {
        emit onlineStateChanged(false);
    }

    return false;
}

void ApplicationUI::onNetworkConnectionFailed ()
{
    if (this->m_ConnectionRetries < 3)
    {
        showConnectionRetryDialog();
    }
    else
    {
        showExitDialog();
    }
}

void ApplicationUI::showConnectionRetryDialog ()
{
    this->m_ConnectionRetries += 1;

    QString btnMsg = "Retry " + QString::number(m_ConnectionRetries) + " of 3";

    SystemDialog* retryDialog = new SystemDialog(btnMsg, "Cancel");
    this->m_pCurrentDialog = retryDialog;
    retryDialog->setTitle("Connection error");
    retryDialog->setBody("The connection has failed");

    bool result = QObject::connect(retryDialog,
            SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
            SLOT(onDialogFinished(bb::system::SystemUiResult::Type)));
    Q_ASSERT(result);
    Q_UNUSED(result);

    this->m_RetryDialogIsDisplayed = true;

    retryDialog->show();
}

void ApplicationUI::showExitDialog ()
{
    QString msg = "The app could not re-establish a connection, and will exit";
    SystemToast* exitMessage = new SystemToast();
    exitMessage->setBody(msg);
    exitMessage->setPosition(SystemUiPosition::MiddleCenter);
    exitMessage->show();

    bool result = QObject::connect(exitMessage,
            SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
            SLOT(onFatalError()));

    Q_ASSERT(result);
    Q_UNUSED(result);
}

void ApplicationUI::onDialogFinished ( SystemUiResult::Type type )
{
    if (type == SystemUiResult::ConfirmButtonSelection)
    {
        // Retry connection
        this->checkConnection();
    }
    else
    {
        QString msg = "The app will now exit";
        SystemToast* exitMessage = new SystemToast();
        exitMessage->setBody(msg);
        exitMessage->setPosition(SystemUiPosition::MiddleCenter);
        exitMessage->show();

        bool result = QObject::connect(exitMessage,
                SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                SLOT(onFatalError()));

        Q_ASSERT(result);
        Q_UNUSED(result);
    }
}

void ApplicationUI::onFatalError ()
{
    Application::instance()->requestExit();
}

void ApplicationUI::onDownloadProgress ( qint64 bytesSent, qint64 bytesTotal )
{
    if (bytesSent == 0 || bytesTotal == 0)
    {
        SystemToast* infoMessage = new SystemToast();

        infoMessage->setBody("No data to download or display");
        infoMessage->setPosition(SystemUiPosition::MiddleCenter);
        infoMessage->show();

        bool result = QObject::connect(infoMessage,
                SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                SLOT(onFatalError()));
        Q_UNUSED(result);
        Q_ASSERT(result);
    }
    else
    {
        int currentProgress = (bytesSent * 100) / bytesTotal;

        SystemProgressToast* pProgToast = new SystemProgressToast();
        pProgToast->setBody("Contacting network to download file ...");
        pProgToast->setProgress(currentProgress);
        pProgToast->setState(SystemUiProgressState::Active);
        pProgToast->setPosition(SystemUiPosition::MiddleCenter);
        pProgToast->show();
    }
}

// For localization
void ApplicationUI::onSystemLanguageChanged ()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files
    QString locale_string = QLocale().name();
    QString file_name = QString("Networking_v2_0_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm"))
    {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
