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

#include "CertInfoControl.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

using namespace bb::cascades;

ApplicationUI::ApplicationUI ( bb::cascades::Application *app ) :
        QObject(app)
{
    // Prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler,
            SIGNAL(systemLanguageChanged()), this,
            SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app,
    // this is added to avoid a compiler warning
    Q_UNUSED(res);

    // For localization only
    onSystemLanguageChanged();

    CertInfoControl *certInfoDialog = new CertInfoControl();

    res = QObject::connect(this,
            SIGNAL(certificateChainChanged(QList<QSslCertificate>)),
            certInfoDialog,
            SLOT(onCertificateChainChanged(QList<QSslCertificate>)));
    Q_ASSERT(res);

    res = QObject::connect(this,
            SIGNAL(viewCertificateChain()),
            certInfoDialog,
            SLOT(onViewCertificateChain()));
    Q_ASSERT(res);

    m_socket = 0;
    m_hostName = "www.blackberry.com";
    m_port = 443;
    m_sessionActive = false;
    m_cipher = "";

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make the business logic objects available to
    // the UI as context properties
    qml->setContextProperty("_app", this);
    qml->setContextProperty("_CertInfoControl", certInfoDialog);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject < AbstractPane >();

    p_mConnectBtn = root->findChild < Button* >("connectBtnObj");

    // Set created root object as the application scene
    app->setScene(root);
}

void ApplicationUI::setHostName ( const QString &hostName )
{
    if (m_hostName == hostName) return;

    m_hostName = hostName;
}

QString ApplicationUI::hostName () const
{
    return m_hostName;
}

void ApplicationUI::setPort ( const QString &portString )
{
    bool ok = false;
    const int port = portString.toInt(&ok);

    if (!ok || (m_port == port)) return;

    m_port = port;
    emit portChanged();
}

QString ApplicationUI::port () const
{
    return QString::number(m_port);
}

bool ApplicationUI::sessionActive () const
{
    return m_sessionActive;
}

QString ApplicationUI::cipher () const
{
    return m_cipher;
}

QString ApplicationUI::response () const
{
    return m_response;
}

// Updates the enabled state of the connectBtn
void ApplicationUI::onUpdateEnabledState ()
{
    // Update the sessionActive property
    // with the current SSL socket state
    const bool sessionActive = (m_socket
            && (m_socket->state() == QAbstractSocket::ConnectedState));

    // Signals no change of state
    if (m_sessionActive == sessionActive) return;

    // Let the user know that more information on the certificate
    // is available by clicking the lock icon
    showIconMessage();

    p_mConnectBtn->setEnabled(!sessionActive);

    m_sessionActive = sessionActive;
    emit sessionActiveChanged();
}

void ApplicationUI::secureConnect ()
{
    if (!m_socket)
    {
        // Create a new SSL socket
        m_socket = new QSslSocket(this);

        // Connect to signals to receive notifications
        // about state changes
        bool res;
        Q_UNUSED(res);

        res = QObject::connect(m_socket,
                    SIGNAL(sslErrors(QList<QSslError>)),
                    this,
                    SLOT(onSslErrors(QList<QSslError>)));

        Q_ASSERT(res);

        res = QObject::connect(m_socket,
                        SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                        this,
                        SLOT(onSocketSateChange(QAbstractSocket::SocketState)));
        Q_ASSERT(res);

        res =  QObject::connect(m_socket,
                        SIGNAL(encrypted()),
                        this,
                        SLOT(onSocketEncrypted()));
        Q_ASSERT(res);

        res = QObject::connect(m_socket,
                        SIGNAL(readyRead()),
                        this,
                        SLOT(onSocketReadyRead()));
        Q_ASSERT(res);
    }

    // Make the SSL connection to the host on the specified port
    m_socket->connectToHostEncrypted(m_hostName, m_port);
}

void ApplicationUI::onSocketSateChange ( QAbstractSocket::SocketState state )
{
    if (state == QAbstractSocket::UnconnectedState)
    {
        // Clear the cipher text and delete the SSL socket
        // if it's been disconnected
        m_cipher = "== DISCONNECTED ==";

        // Update the cipher property
        emit cipherChanged();

        m_socket->close();
        m_socket->deleteLater();
    }
    else if(state == QAbstractSocket::ConnectedState)
    {
        onUpdateEnabledState();
    }
}

void ApplicationUI::onSocketEncrypted ()
{
    // The connection may have been disconnected
    if (!m_socket) return;

    // Clear the response data from previous connections
    // to create a new connection
    m_response.clear();
    emit responseChanged();

    // Retrieve information about the cipher used
    // and update the m_cipher property
    const QSslCipher cipher = m_socket->sessionCipher();

    // Take the component parts of the cipher data and format
    // them into a QString to update the m_cipher property and
    // for viewing on-screen
    m_cipher = QString("%1, %2 (%3 / %4)")
            .arg(cipher.authenticationMethod())
            .arg(cipher.name())
            .arg(cipher.usedBits())
            .arg(cipher.supportedBits());

    // Update the cipher property
    emit cipherChanged();

    // Tell the CertInfoControl about the certificate
    // chain of this connection
    emit certificateChainChanged(m_socket->peerCertificateChain());
}

void ApplicationUI::onSocketReadyRead ()
{
    QByteArray data = m_socket->readAll();

    // Read the response from the server and append it
    // to the 'response' property
    appendString(QString::fromUtf8(data));
}

void ApplicationUI::onSslErrors(const QList<QSslError> &errList)
{
    // Create the error message list
    QStringList messages;

    foreach (const QSslError &error, errList)
        messages << error.errorString();

    // Update the CertificateInfoControl certificate chain
    emit certificateChainChanged(m_socket->peerCertificateChain());

    // Display errors in the console
    qDebug() << messages.join("\n");

    // Update the socket state
    m_socket->setSocketDescriptor(0, m_socket->state());
}

void ApplicationUI::sendData ( const QString input )
{
    if (input.isEmpty()) return;

    // Add an additional line break to the input to
    // accommodate some protocols that require it
    appendString(input + '\n');

    // Send the data to the server
    m_socket->write(input.toUtf8() + "\r\n");
}

void ApplicationUI::appendString ( const QString line )
{
    // Update the m_response property
    m_response.append(line);
    emit responseChanged();
}

void ApplicationUI::showIconMessage ()
{
    // Display the message on screen.
    bb::system::SystemToast* toast = new bb::system::SystemToast(this);

    toast->setBody("Click the lock icon for certificate information");
    toast->setPosition(bb::system::SystemUiPosition::TopCenter);
    toast->show();
}

// For localization only
void ApplicationUI::onSystemLanguageChanged ()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("Networking_SSL_v1_0_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm"))
    {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
