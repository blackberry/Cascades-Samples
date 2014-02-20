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

#include <QFile>
#include <QtNetwork>
#include <QNetworkSession>
#include <bb/cascades/Image>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/system/SystemToast>
#include <bb/system/SystemDialog>
#include <bb/system/SystemProgressToast>

namespace bb {
namespace cascades {
class Application;
class LocaleHandler;
}
}

using namespace bb::cascades;
using namespace bb::system;

class QTranslator;

class ApplicationUI: public QObject {
Q_OBJECT

public:
	ApplicationUI(bb::cascades::Application *app);
	virtual ~ApplicationUI() {
	}

signals:
	void onlineStateChanged(bool);
	void sendNewRequest();
	void updateListView();
	void networkConnectionFailed();
	void fileOpenFailed();

private slots:
	void onSystemLanguageChanged();

	void onOnlineStateChanged(bool);
	void onSendNewRequest();
	void onUpdateListView();
	void onRequestFinished();
	void onDownloadProgress(qint64, qint64);
	void onNetworkConnectionFailed();
	void onFileOpenFailed();
	void onFileOpenDialogFinished(bb::system::SystemUiResult::Type);
	void onDialogFinished(bb::system::SystemUiResult::Type);
	void onFatalError();

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
	SystemDialog* m_pCurrentDialog;
	int m_ConnectionRetries;
	int m_FileOpenRetries;
	bool m_RetryDialogIsDisplayed;
	QString m_CurrentInterface;
	void refreshInterface(QString);
	bool checkConnection();
	void showConnectionRetryDialog();
	void showExitDialog();
};

#endif /* ApplicationUI_HPP_ */
