/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <QIODevice>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/XmlDataModel>
#include <bb/system/SystemDialog>
#include <bb/system/SystemToast>
#include <bb/cascades/Color>

#include <bps/bps.h>
#include <bps/netstatus.h>
#include <bps/locale.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI ( bb::cascades::Application *app ) :
      QObject(app)
{
   // prepare the localization
   m_pTranslator = new QTranslator(this);
   m_pLocaleHandler = new LocaleHandler(this);

   bool res = QObject::connect(m_pLocaleHandler,
                               SIGNAL(systemLanguageChanged()),
                               this,
                               SLOT(onSystemLanguageChanged()));

   // This is only available in Debug builds
   Q_ASSERT(res);

   // Since the variable is not used in the app, this is added to avoid a
   // compiler warning
   Q_UNUSED(res);

   // For localization.
   onSystemLanguageChanged();

   // Create scene document from main.qml asset, the parent is set
   // to ensure the document gets destroyed properly at shut down.
   QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

   // Create root object for the UI
   AbstractPane *root = qml->createRootObject < AbstractPane >();

   // Get a handle to the UI controls.
   pNetwrkConnDot = root->findChild < ImageView* >("netConnDot");
   pNetwrkIntrfcSymb = root->findChild < ImageView* >("netConnTypeIcon");
   pListView = root->findChild < ListView* >("list");

   // Get a handle to the Action items.
   pRefreshAction = root->findChild < ActionItem* >("refresh");

   // Set created root object as the application scene
   app->setScene(root);

   // Initialize member variables.
   pNetworkAccessManager = new QNetworkAccessManager(this);
   networkConnected = false;
   numOfRetries = 1;
   reply = NULL;

   // Create a file in the device file system that
   // we can use to save the data model.
   pFileObj = new QFile("data/model.xml");

   // Create a status event object that is used to listen for changes
   // in the network status and/or connectivity interface.
   // When a change signal is received, it'll trigger the
   // onNetworkStatusUpdated slot to update the status for us.
   statusEvent = new StatusEvent();

   currentToast = new SystemToast();

   // Check signal and slot connections.
   bool result;
   Q_UNUSED(result);

   result = connect(currentToast, SIGNAL(finished(bb::system::SystemUiResult::Type)),
         this, SLOT(onToastFinished(bb::system::SystemUiResult::Type)));

   Q_ASSERT(result);

   result = connect(statusEvent, SIGNAL(networkStatusUpdated(bool, QString)),
                    this, SLOT(onNetworkStatusUpdated(bool, QString)));

   Q_ASSERT(result);

   result = connect(pRefreshAction, SIGNAL(triggered()),
                    this, SLOT(startRefreshActionItem ()));

   Q_ASSERT(result);

   result = connect(pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(onRequestFinished(QNetworkReply*)));

   Q_ASSERT(result);

   result = connect(this, SIGNAL(updateListView(QNetworkReply*)),
                    this, SLOT(onUpdateListView(QNetworkReply*)));

   Q_ASSERT(result);

   result = connect(this, SIGNAL(requestFileFromNetwork()),
                    this, SLOT(onRequestFileFromNetwork()));

   Q_ASSERT(result);
}

// Action bar refresh action.
void ApplicationUI::startRefreshActionItem ()
{
   emit requestFileFromNetwork();
}

// Action bar exit action.
void ApplicationUI::startExitActionItem ()
{
   Application::instance()->requestExit();
}

// Updates the status of the network connection in the UI.
void ApplicationUI::onNetworkStatusUpdated ( bool connectionStatus, QString interfaceType )
{
   networkConnected = connectionStatus;

   if (connectionStatus)
   {
      currentToast->cancel();

      pNetwrkConnDot->setImageSource(QUrl("asset:///images/greenDot.gif"));

      pNetwrkIntrfcSymb->setImageSource(QUrl(interfaceType));

      emit requestFileFromNetwork();
   }
   else
   {
      pNetwrkConnDot->setImageSource(QUrl("asset:///images/redDot.gif"));

      pNetwrkIntrfcSymb->setImageSource(QUrl(interfaceType));

      showToast("No network connection is active", "Retry?", true);
   }
}

void ApplicationUI::onRequestFinished ( QNetworkReply* reply )
{
   QNetworkReply::NetworkError netError = reply->error();

   if (netError == QNetworkReply::NoError)
   {
      // Emit updateListView signal to set the
      // new data model on the list.
      emit updateListView(reply);
   }
   else
   {
      switch (netError)
      {
         case QNetworkReply::ContentNotFoundError:
            showToast("The content was not found on the server", "", false);
            break;

         case QNetworkReply::HostNotFoundError:
            showToast("The server was not found", "", false);
            break;

         default:
            showToast(reply->errorString(), "", false);
            break;
      }
   }
}

// Updates the ListView control with an XML file retrieved
// from the network.
void ApplicationUI::onUpdateListView ( QNetworkReply* reply )
{
   // Open the file.
   if (pFileObj->open(QIODevice::ReadWrite) == false)
   {
      showToast("Failed to open the file", "Exit app", true);
      numOfRetries = 3;
   }
   else
   {
      // Write to the file using the reply data
      // and close the file.
      pFileObj->write(reply->readAll());
      pFileObj->flush();
      pFileObj->close();

      // Create the data model using the contents of the file.
      XmlDataModel* pDataModel = new XmlDataModel();
      QString fileUrl = "file://" + QDir::homePath() + "/model.xml";
      pDataModel->setSource(QUrl(fileUrl));

      pListView->setDataModel(pDataModel);
   }

   reply->deleteLater();
}

void ApplicationUI::onRequestFileFromNetwork ()
{
   if (networkConnected)
   {
      // Create and send the network request.
      QNetworkRequest request = QNetworkRequest();
      QString url = "https://developer.blackberry.com/native/files";
      url.append("/documentation/cascades/images/model.xml");
      request.setUrl(QUrl(url));

      reply = pNetworkAccessManager->get(request);

      // Show download progress.
      bool result;
      Q_UNUSED(result);

      result = connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
                       this, SLOT(onDownloadProgress(qint64, qint64)));

      Q_ASSERT(result);
   }
   else
   {
      QString buttonMsg = "Retry: ";
      buttonMsg.append(QString::number(numOfRetries)).append("of 3");

      showToast("No network connection", buttonMsg, true);
   }
}

void ApplicationUI::onDownloadProgress ( qint64 bytesSent, qint64 bytesTotal )
{
   if (bytesSent == 0 || bytesTotal == 0) return;

   int currentProgress = (bytesSent * 100) / bytesTotal;

   SystemProgressToast* pProgToast = new SystemProgressToast();

   pProgToast->setBody("Contacting network to download file ...");
   pProgToast->setProgress(currentProgress);
   pProgToast->setState(SystemUiProgressState::Active);
   pProgToast->setPosition(SystemUiPosition::MiddleCenter);
   pProgToast->show();
}

// Shows the toast with the message and button label passed in as parameters.
void ApplicationUI::showToast ( QString msg, QString btnMsg, bool showButton )
{
   // Display the message on screen.
   SystemUiButton* toastRetryBtn = NULL;

   if (showButton)
   {
      toastRetryBtn = currentToast->button();
      toastRetryBtn->setLabel(btnMsg);
   }

   currentToast->setBody(msg);
   currentToast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
   currentToast->show();
}

// Gives the user 3 chances to re-establish the network connection before the
// app asks them to exit.
void ApplicationUI::onToastFinished ( bb::system::SystemUiResult::Type resultType )
{
   if (numOfRetries != 3 && resultType == SystemUiResult::ButtonSelection)
   {
      numOfRetries += 1;
      emit requestFileFromNetwork();
   }
   else if (networkConnected)
   {
      numOfRetries = 1;
   }
   else
   {
      showFinalDialog();
   }
}

// Shows the toast with the an exit message.
void ApplicationUI::showFinalDialog ()
{
   SystemDialog* dialog = new bb::system::SystemDialog(this);
   SystemUiButton* toastCancelBtn = dialog->cancelButton();
   SystemUiButton* toastExitBtn = dialog->confirmButton();

   toastCancelBtn->setLabel("Cancel");
   toastExitBtn->setLabel("Exit app");
   dialog->setBody("A network connection could not be re-established");
   dialog->show();

   bool result;
   Q_UNUSED(result);

   result = connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)),
                    this, SLOT(onFinalDialogFinished(bb::system::SystemUiResult::Type)));

   Q_ASSERT(result);
}

void ApplicationUI::onFinalDialogFinished ( bb::system::SystemUiResult::Type resultType )
{
   if (resultType == SystemUiResult::ConfirmButtonSelection)
   {
      Application::instance()->requestExit();
   }
   else
   {
      numOfRetries = 1;
   }
}

void ApplicationUI::onSystemLanguageChanged ()
{
   QCoreApplication::instance()->removeTranslator(m_pTranslator);
   // Initiate, load and install the application translation files.
   QString locale_string = QLocale().name();
   QString file_name = QString("ContactList_V23_2_%1").arg(locale_string);
   if (m_pTranslator->load(file_name, "app/native/qm"))
   {
      QCoreApplication::instance()->installTranslator(m_pTranslator);
   }
}
