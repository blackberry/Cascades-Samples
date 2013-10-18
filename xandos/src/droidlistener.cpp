/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#include "droidlistener.hpp"
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

//! [0]
droidlistener::droidlistener(QObject *parent)
    : QObject(parent), m_port(9876)
{
    m_server = new QTcpServer(this);
    // Connect into the signal/slot mechanism to invoke this class method when a new connection
    // is available
    bool ok = connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]
droidlistener::~droidlistener()
{
    if (m_socket) {
        disconnected();
        m_socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
}

//! [1]
void droidlistener::listen()
{
    qDebug() << "xandos: start listening for connections";
    m_server->listen(QHostAddress::LocalHost, m_port);
}
//! [1]
//! [2]
void droidlistener::newConnection()
{
    m_socket = m_server->nextPendingConnection();
    if (m_socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "xandos: New connection established.";
    }
    // Make connections for reveiving disconnect and read ready signals for the
    // new connection socket
    bool ok = connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    Q_ASSERT(ok);
    ok = connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [2]
//! [3]
void droidlistener::readyRead()
{
    QByteArray ba = m_socket->read(20);
    // Emit the droid selection when finished reading in the
    // data from the droid client socket
    Q_EMIT droidSelection(QString(ba));
}

void droidlistener::readyWrite(const int code)
{
    if (m_socket && m_socket->state() == QTcpSocket::ConnectedState) {
        m_socket->write(QByteArray::number(code));
        m_socket->flush();
    }
}
//! [3]
//! [4]
void droidlistener::disconnected()
{
    qDebug() << "xandOs: disconnecting...";
    disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    m_socket->close();
}
//! [4]
