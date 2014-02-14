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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

#include <QFile>
#include <QtNetwork>
#include <QNetworkSession>
#include <bb/cascades/Image>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/system/SystemToast>
#include <bb/system/SystemProgressToast>

namespace bb
{
   namespace cascades
   {
      class Application;
      class LocaleHandler;
   }
}

using namespace bb::cascades;
using namespace bb::system;

class QTranslator;

class ApplicationUI : public QObject
{
   Q_OBJECT

   public:
      ApplicationUI ( bb::cascades::Application *app );
      virtual ~ApplicationUI () {}

   signals:
      void updateDataModel ();
      void updateListView ();
      void networkConnectionFailed ();
      void fileOpenFailed ();

   private slots:
      void onSystemLanguageChanged ();

      void onOnlineStateChanged ( bool );
      void onUpdateDataModel ();
      void onUpdateListView ();
      void onRequestFinished ();
      void onDownloadProgress ( qint64, qint64 );
      void onNetworkConnectionFailed ();
      void onFileOpenFailed ();
      void onFileOpenMsgFinished( bb::system::SystemUiResult::Type );
      void onToastFinished ( bb::system::SystemUiResult::Type );
      void onExitMessageFinished();

   private:
      QTranslator* m_pTranslator;
      bb::cascades::LocaleHandler* m_pLocaleHandler;

      // UI control handles.
      ImageView* m_pNetwrkConnIcon;
      ImageView* m_pNetwrkIntrfcSymb;
      ListView* m_pListView;

      QNetworkConfigurationManager* m_pNetConfigMngr;
      QNetworkAccessManager* m_pNetAccessMngr;
      QNetworkReply* m_pNetReply;
      QFile* m_pFileObj;
      SystemToast* m_pCurrentToast;
      int m_ConnectionRetries;
      int m_FileOpenRetries;
      bool m_RetryToastIsDisplayed;
      QString m_CurrentInterface;
      void refreshInterface ( QString );
};

#endif /* ApplicationUI_HPP_ */
