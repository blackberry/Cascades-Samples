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
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <bb/system/SystemToast>
#include <bb/cascades/Button>

#include "CertInfoControl.hpp"

namespace bb {
namespace cascades {
		class Application;
		class LocaleHandler;
	}
}

class QTranslator;
class CertInfoControl;

class ApplicationUI: public QObject {
	Q_OBJECT

	// The host name of the connection server
	Q_PROPERTY(QString hostName
        READ hostName WRITE setHostName NOTIFY hostNameChanged)

	// The port number of the connection server
	Q_PROPERTY(QString port
        READ port WRITE setPort NOTIFY portChanged)

	// Reports whether an SSL session has been established
	Q_PROPERTY(bool sessionActive
        READ sessionActive NOTIFY sessionActiveChanged)

	// The name of the cipher that is used by the SSL connection
	Q_PROPERTY(QString cipher
	    READ cipher NOTIFY cipherChanged)

	// The response data that is sent by the server
	Q_PROPERTY(QString response
	    READ response NOTIFY responseChanged)

public:
	ApplicationUI(bb::cascades::Application *app);
	virtual ~ApplicationUI() {}

	// Accessor methods for above properties
	void setHostName(const QString &hostName);
	QString hostName() const;
	void setPort(const QString &port);
	QString port() const;
	bool sessionActive() const;
	QString cipher() const;
	QString response() const;

public slots:
	// Called from the UI to create a secure connection
	void secureConnect();

	// Called from the UI to send data to the server
	// over the secure connection
	void sendData(const QString);

	void onSslErrors(const QList<QSslError> &errList);

signals:
	// The change notification signals for the above properties
	void hostNameChanged();
	void portChanged();
	void sessionActiveChanged();
	void cipherChanged();
	void responseChanged();

	// This signal is emitted whenever the server reports the
	// certificate chain that should be used
	void certificateChainChanged(const QList<QSslCertificate> &chain);

	// This signal is emitted if the user requests
	// to view the certificate chain
	void viewCertificateChain();

private slots:
	// For localization
	void onSystemLanguageChanged();

	// Updates the enabled state of the connectBtn
	void onUpdateEnabledState();

	// Called whenever the state of the SSL socket changes
	void onSocketSateChange(QAbstractSocket::SocketState);

	// Called after a successful SSL connection has been made
	void onSocketEncrypted();

	// Called when data arrives from the server
	// through a secure connection
	void onSocketReadyRead();

private:
	// For localization
	QTranslator* m_pTranslator;
	bb::cascades::LocaleHandler* m_pLocaleHandler;

	// Adds a response from the server to the response property
	void appendString(const QString);

	// The SSL socket that performs the low-level communication
	QSslSocket *m_socket;

	// The property values
	QString m_hostName;
	int m_port;
	bool m_sessionActive;
	QString m_cipher;
	QString m_response;
	bb::cascades::Button *p_mConnectBtn;

	void showIconMessage();
};

#endif /* ApplicationUI_HPP_ */
