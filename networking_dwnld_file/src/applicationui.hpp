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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QFile>
#include <QObject>
#include <QtNetwork>
#include <QNetworkSession>
#include <bb/cascades/Image>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/TextArea>
#include <bb/cascades/ImageView>
#include <bb/system/SystemToast>
#include <bb/system/SystemDialog>
#include <bb/cascades/ToggleButton>
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
class RawHeaderView;

class ApplicationUI : public QObject
{
    Q_OBJECT

    public:
        ApplicationUI ( bb::cascades::Application *app );
        virtual ~ApplicationUI () {}

    private slots:
        // For localization
        void onSystemLanguageChanged ();

        void openFile ();
        void onRequestFinished();
        void onOnlineStateChanged(bool);
        void onDownloadProgress ( qint64, qint64 );
        void onFileOpenRetryDialogFinished ( bb::system::SystemUiResult::Type );
        void onConnRetryDialogFinished ( bb::system::SystemUiResult::Type );
        void fatalErrorCloseApp ();
    private:
        // For localization
        QTranslator* m_pTranslator;
        bb::cascades::LocaleHandler* m_pLocaleHandler;

        // UI control handles
        ImageView* m_pNetwrkConnIcon;
        ImageView* m_pNetwrkIntrfcSymb;
        ToggleButton* m_pViewRawHdrTbn;
        TextArea* m_pRawHeaderInfoTxa;
        ListView* m_pListView;

        QNetworkConfigurationManager* m_pNetConfigMngr;
        QNetworkAccessManager* m_pNetAccessMngr;
        QNetworkReply* m_pNetReply;
        QFile* m_pFileObj;
        SystemDialog* m_pCurrentDialog;
        SystemProgressToast* m_pDwnldProgressToast;
        int m_ConnectionRetries;
        int m_FileOpenRetries;
        bool m_RetryConnIsDisplayed;
        bool m_RetryFileOpenIsDisplayed;
        QString m_CurrentInterface;

        // Utility functions
        void updateOnlineStatus ( bool );
        void refreshConnectionIcons ( QString, QUrl );
        void sendNewRequest ();
        void displayFileOpenRetryDialog ();
        void displayConnRetryDialog ();
        void waitForConnection();
        QString getErrorString(QNetworkReply::NetworkError);
        QString getRawHeaderInfo(QNetworkReply*);
};

#endif /* ApplicationUI_HPP_ */
