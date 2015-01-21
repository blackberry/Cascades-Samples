/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 */

#include "RawHeaderView.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/XmlDataModel>
#include <bb/cascades/LocaleHandler>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    // Prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res;
    // Since the variable is not used in the app,
    // this is added to avoid a compiler warning
    Q_UNUSED(res);

    res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this,
            SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Instantiate the raw header dialog
    RawHeaderView *rawHeaderView = new RawHeaderView();

    // To avoid a reference error, this derived context must be
    // defined before your app calls qml->createRootObject
    qml->setContextProperty("_RawHeaderView", rawHeaderView);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set pointers to the UI controls.
    m_pNetwrkConnIcon = root->findChild<ImageView*>("netConnDot");
    m_pNetwrkIntrfcSymb = root->findChild<ImageView*>("netConnTypeIcon");
    m_pViewRawHdrTbn = root->findChild<ToggleButton*>("tbnVewRawHdr");
    m_pRawHeaderInfoTxa = root->findChild<TextArea*>("txaRawHeaderInfo");
    m_pListView = root->findChild<ListView*>("list");

    // Initialize member variables
    m_pNetConfigMngr = new QNetworkConfigurationManager();
    m_pNetAccessMngr = new QNetworkAccessManager(this);
    m_pNetReply = NULL;
    m_pCurrentDialog = NULL;
    m_pDwnldProgressToast = NULL;
    m_ConnectionRetries = 1;
    m_FileOpenRetries = 1;
    m_RetryFileOpenIsDisplayed = false;
    m_RetryConnIsDisplayed = false;

    // Create a file in the device file system to save
    // the data model
    m_pFileObj = new QFile("data/contacts_list.xml");

    // Set created root object as the application scene
    app->setScene(root);

    // Connect to the QNetworkAccessManager finished signal
    res = QObject::connect(m_pNetAccessMngr, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(onRequestFinished()));
    Q_ASSERT(res);

    res = QObject::connect(m_pNetConfigMngr, SIGNAL(onlineStateChanged(bool)), this,
            SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(res);

    waitForConnection();
}

// Waits for the device to make a connection and
// if it fails to connect, it displays the
// connection retry dialog.
void ApplicationUI::waitForConnection()
{
    QTime resumeTime = QTime::currentTime().addSecs(3);

    SystemToast* waitToast = new SystemToast();
    QString waitMsg = "Waiting for network connection ... ";
    waitToast->setBody(waitMsg);
    waitToast->setPosition(SystemUiPosition::MiddleCenter);
    waitToast->show();

    // Pause the app to wait for a connection
    while (QTime::currentTime() < resumeTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    if (m_pNetConfigMngr->isOnline() == false)
    {
        displayConnRetryDialog();
    }
}

// This function creates a new network request and sends
// it to the server, and monitors download progress
void ApplicationUI::sendNewRequest()
{
    // Create the network request
    QNetworkRequest request = QNetworkRequest();

    QString requestUrl = "http://developer.blackberry.com";
    requestUrl.append("/native/files/documentation");
    requestUrl.append("/cascades/images/contacts_list.xml");
    request.setUrl(QUrl(requestUrl));

    bool res;
    Q_UNUSED(res);

    // Send the network request
    m_pNetReply = m_pNetAccessMngr->get(request);

    if (m_pNetReply != NULL)
    {
        // Show download progress
        res = QObject::connect(m_pNetReply, SIGNAL(downloadProgress(qint64, qint64)), this,
                SLOT(onDownloadProgress(qint64, qint64)));
        Q_ASSERT(res);
    }
}

QString ApplicationUI::getErrorString(QNetworkReply::NetworkError networkError)
{
    QString errStr = "Network Error: ";

    switch (networkError)
    {
        case 1:
            errStr += "Connection refused";
            break;

        case 3:
            errStr += "Host not found";
            break;

        case 7:
            errStr += "Temporary network failure";
            break;

        case 201:
            errStr += "Content access denied";
            break;

        case 203:
            errStr += "Content not found";
            break;

        case 299:
            errStr += "Unknown content error";
            break;

        default:
            errStr += "Unknown error";
            break;
    }

    return errStr;
}

void ApplicationUI::onRequestFinished()
{
    if (m_pNetReply != NULL && m_pNetReply->bytesAvailable() > 0
            && m_pNetReply->error() == QNetworkReply::NoError)
    {
        // Update the online status in the UI
        updateOnlineStatus(true);

        m_ConnectionRetries = 1;

        // Update the contact list
        openFile();

        // Update the raw headers dialog
        QString rawHdrInfo = getRawHeaderInfo(m_pNetReply);
        m_pRawHeaderInfoTxa->setText(rawHdrInfo);

        // Enable the view raw header toggle
        m_pViewRawHdrTbn->setEnabled(true);

        m_pNetReply->deleteLater();
    }
    else
    {
        if (m_pDwnldProgressToast != NULL) m_pDwnldProgressToast->cancel();

        m_pViewRawHdrTbn->setEnabled(false);

        displayConnRetryDialog();
    }
}

// This function collects the raw headers and returns them as a
// QString object
QString ApplicationUI::getRawHeaderInfo(QNetworkReply* reply)
{
    QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    QString rawHdrs;

    foreach(QByteArray hdrName, rawHeaderList){
    QByteArray hdrValue = reply->rawHeader(hdrName);
    rawHdrs += hdrName;
    rawHdrs += " : ";
    rawHdrs += hdrValue;
    rawHdrs += "\n";
}

    return rawHdrs;
}

void ApplicationUI::onOnlineStateChanged(bool isOnline)
{
    // Update the UI
    updateOnlineStatus(isOnline);

    if (isOnline)
    {
        sendNewRequest();
    }
}

void ApplicationUI::updateOnlineStatus(bool isOnline)
{
    QUrl connIconUrl;
    QNetworkConfiguration::BearerType bearerType;

    if (isOnline)
    {
        if (m_RetryConnIsDisplayed)
        {
            m_pCurrentDialog->cancel();
            m_ConnectionRetries = 0;
        }

        connIconUrl = "asset:///images/greenDot.png";
    }
    else
    {
        connIconUrl = "asset:///images/redDot.png";
    }

    bearerType = m_pNetAccessMngr->activeConfiguration().bearerType();

    // Refresh the interface and online icons in UI
    refreshConnectionIcons(bearerType, connIconUrl);
}

void ApplicationUI::refreshConnectionIcons(QNetworkConfiguration::BearerType bearerType,
        QUrl onlineIcon)
{
    switch (bearerType)
    {
        case QNetworkConfiguration::BearerUnknown:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/unknown.png"));
            break;

        case QNetworkConfiguration::BearerEthernet:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/wired.png"));
            break;

        case QNetworkConfiguration::BearerWLAN:
        case QNetworkConfiguration::BearerWiMAX:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/wifi.png"));
            break;

        case QNetworkConfiguration::Bearer2G:
        case QNetworkConfiguration::BearerCDMA2000:
        case QNetworkConfiguration::BearerWCDMA:
        case QNetworkConfiguration::BearerHSPA:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/cellular.png"));
            break;

        case QNetworkConfiguration::BearerBluetooth:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/bluetooth.png"));
            break;

        default:
            m_pNetwrkIntrfcSymb->setImageSource(QUrl("asset:///images/unknown.png"));
            break;
    }

    m_pNetwrkConnIcon->setImageSource(onlineIcon);
}

// This function opens the file to store the contact list,
// and also updates the ListView control
void ApplicationUI::openFile()
{
    if (m_pFileObj->open(QIODevice::ReadWrite))
    {
        // Write to the file using the m_pNetReply object
        m_pFileObj->write(m_pNetReply->readAll());
        m_pFileObj->flush();
        m_pFileObj->close();

        // Create a temporary data model file using the contents
        // of a local XML file
        XmlDataModel* pDataModel = new XmlDataModel();

        // Set the file path to the downloaded contacts list .xml file
        QUrl fileUrl = "file://" + QDir::homePath() + "/contacts_list.xml";

        // Set the data model property for the ListView control with the
        // path of the downloaded contacts_list.xml file
        pDataModel->setSource(fileUrl);

        // Update the ListView control with the new data model
        m_pListView->setDataModel(pDataModel);

        m_FileOpenRetries = 1;
    }
    else
    {
        // The progress toast must be canceled to
        // keep it from displaying when this error
        // has occurred
        m_pDwnldProgressToast->cancel();

        // Handle file access errors
        displayFileOpenRetryDialog();
    }
}

void ApplicationUI::displayConnRetryDialog()
{
    QString btnMsg = "Retry ";
    btnMsg.append(QString::number(m_ConnectionRetries));
    btnMsg.append(" of 3");

    SystemDialog* retryNtwrkConnDialog = new SystemDialog(btnMsg, "Cancel");
    retryNtwrkConnDialog->setTitle("Connection Error");

    QString errMsg;

    if (m_pNetReply != NULL)
    {
        errMsg = getErrorString(m_pNetReply->error());
    }
    else
    {
        errMsg = "Network Error: Host not found";
    }
    retryNtwrkConnDialog->setBody(errMsg);

    bool res = QObject::connect(retryNtwrkConnDialog,
            SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
            SLOT(onConnRetryDialogFinished(bb::system::SystemUiResult::Type)));
    Q_ASSERT(res);
    Q_UNUSED(res);

    m_pCurrentDialog = retryNtwrkConnDialog;
    m_RetryConnIsDisplayed = true;
    retryNtwrkConnDialog->show();

    if (m_pDwnldProgressToast != NULL) m_pDwnldProgressToast->cancel();
}

void ApplicationUI::onConnRetryDialogFinished(bb::system::SystemUiResult::Type type)
{
    m_RetryConnIsDisplayed = false;

    if (m_pNetConfigMngr != NULL && m_ConnectionRetries < 3
            && type == SystemUiResult::ConfirmButtonSelection)
    {
        m_ConnectionRetries += 1;

        // Update the UI
        updateOnlineStatus(m_pNetConfigMngr->isOnline());

        // Retry the connection
        sendNewRequest();
    }
    else
    {
        m_pCurrentDialog->cancel();

        QString msg = "The app will now exit";
        SystemToast* exitMessage = new SystemToast();
        exitMessage->setBody(msg);
        exitMessage->setPosition(SystemUiPosition::MiddleCenter);
        exitMessage->show();

        bool result = QObject::connect(exitMessage,
                SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                SLOT(fatalErrorCloseApp()));
        Q_ASSERT(result);
        Q_UNUSED(result);
    }
}

void ApplicationUI::displayFileOpenRetryDialog()
{
    QString btnMsg = "Retry ";
    btnMsg.append(QString::number(m_FileOpenRetries));
    btnMsg.append(" of 3");

    SystemDialog* fileOpenRetryDialog = new SystemDialog(btnMsg, "Cancel");
    fileOpenRetryDialog->setTitle("File Error");
    QString errMsg = "Could not open the file needed for this procedure";
    fileOpenRetryDialog->setBody(errMsg);

    bool res = QObject::connect(fileOpenRetryDialog,
            SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
            SLOT(onFileOpenRetryDialogFinished(bb::system::SystemUiResult::Type)));
    Q_ASSERT(res);
    Q_UNUSED(res);

    m_RetryFileOpenIsDisplayed = true;
    m_pCurrentDialog = fileOpenRetryDialog;
    fileOpenRetryDialog->show();
}

// The user has three tries to open the file needed to continue
// and if those retries fail, then the fatalErrorCloseApp() function
// is called to close the app
void ApplicationUI::onFileOpenRetryDialogFinished(bb::system::SystemUiResult::Type type)
{
    m_RetryFileOpenIsDisplayed = false;

    if (m_FileOpenRetries < 3 && type == SystemUiResult::ConfirmButtonSelection)
    {
        m_FileOpenRetries += 1;

        // Retry opening a file
        openFile();
    }
    else
    {
        // Display the 'Exit the app' toast
        SystemToast* exitFileMsg = new SystemToast();
        QString exitMsg = "The app could not open the necessary file needed ";
        exitMsg.append("to update the list data, and will exit");
        exitFileMsg->setBody(exitMsg);
        exitFileMsg->setPosition(SystemUiPosition::MiddleCenter);
        exitFileMsg->show();

        bool result = QObject::connect(exitFileMsg,
                SIGNAL(finished(bb::system::SystemUiResult::Type)), this,
                SLOT(fatalErrorCloseApp()));
        Q_ASSERT(result);
        Q_UNUSED(result);
    }
}

// Changes the network interface icon shown in the UI
void ApplicationUI::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesSent > 0 && bytesTotal > 0)
    {
        // Show download progress toast
        int currentProgress = (bytesSent * 100) / bytesTotal;

        SystemProgressToast* pProgToast = new SystemProgressToast();
        pProgToast->setBody("Downloading file ...");
        pProgToast->setProgress(currentProgress);
        pProgToast->setState(SystemUiProgressState::Active);
        pProgToast->setPosition(SystemUiPosition::MiddleCenter);
        m_pDwnldProgressToast = pProgToast;
        pProgToast->show();
    }
}

void ApplicationUI::fatalErrorCloseApp()
{
    Application::instance()->requestExit();
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("networking_dwnld_xml_file_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm"))
    {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

