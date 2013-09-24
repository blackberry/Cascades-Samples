/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "ChatManager.hpp"

#include <QtCore>

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

#include <errno.h>
#include <iostream>

/*
 * Base UUID for SPP, per https://bluetooth.org/Technical/AssignedNumbers/service_discovery.htm
 */
//! [0]
#define SPP_SERVICE_UUID "00001101-0000-1000-8000-00805F9B34FB"
//! [0]

/**
 * Callback for events triggered by SPP server API to indicate a new incoming connection.
 */
//! [1]
void BTControllerSPPCallback(long param, int fd)
{
    ChatManager *chatManager = reinterpret_cast<ChatManager*>(param);

    if (chatManager) {
        chatManager->setSPPServer(fd);
    }
}
//! [1]

SPPThread::SPPThread(QObject *parent)
    : QThread(parent)
{
    resetFD();
}

void SPPThread::init(int fd, bool isServer)
{
    m_sppFD = fd;
    m_sppServer = isServer;
}

int SPPThread::getFD() const
{
    return m_sppFD;
}

void SPPThread::resetFD()
{
    m_sppFD = -1;
    m_sppServer = false;
}

bool SPPThread::isServer() const
{
    return m_sppServer;
}

bool SPPThread::active() const
{
    return (m_sppFD >= 0);
}

//! [2]
void SPPThread::run()
{
    int readlen;
    char tempbuff[128];

    while (m_sppFD >= 0) {

        if ((readlen = read(m_sppFD, tempbuff, (128 - 1))) > 0) {
            tempbuff[readlen] = '\0';
            emit incomingMessage(QString::fromLatin1(tempbuff));
        } else {
            if (readlen <= 0) {
                emit connectionClosed();
            }
        }
    }
}
//! [2]

//! [3]
ChatManager::ChatManager(QObject *parent)
    : QObject(parent)
{
    bool ok = connect(&m_sppDataThread, SIGNAL(incomingMessage(QString)), SLOT(incomingMessage(QString)));
    Q_ASSERT(ok);
    ok = connect(&m_sppDataThread, SIGNAL(connectionClosed()), SLOT(closeSPPConnection()));
    Q_ASSERT(ok);
    ok = connect(&m_sppDataThread, SIGNAL(connectionClosed()), SIGNAL(chatEnded()));
    Q_ASSERT(ok);
}
//! [3]

//! [4]
ChatManager::~ChatManager()
{
    // Disconect SPP (if connected)
    closeSPPConnection();
}
//! [4]

void ChatManager::setRemoteAddress(const QString &address)
{
    m_remoteAddress = address;
}

//! [5]
void ChatManager::setSPPServer(int fd)
{
    if (fd >= 0) {
        m_sppDataThread.init(fd, true);
        m_sppDataThread.start();
        updateChatWindow("Client connected!\n\n");
    }
}
//! [5]

//! [6]
void ChatManager::setSPPClient(int fd)
{
    if (fd >= 0) {
        m_sppDataThread.init(fd, false);
        m_sppDataThread.start();
    }
}
//! [6]

//! [7]
void ChatManager::connectToSPPService()
{
    m_chatHistory.clear();

    const int fd = bt_spp_open(m_remoteAddress.toAscii().data(), (char *) SPP_SERVICE_UUID, false);

    if (fd >= 0) {
        updateChatWindow("SPP Client\n\n");

        setSPPClient(fd);
    } else {
        showDialog("spp_open fail", "errno = " + QString::number(errno) );
    }
}
//! [7]

//! [8]
void ChatManager::startSPPServer()
{
    m_chatHistory.clear();

    if (bt_spp_open_server((char *) "", (char *) SPP_SERVICE_UUID, 0, BTControllerSPPCallback, reinterpret_cast<long>(this)) == 0) {
        updateChatWindow("SPP Server started. Waiting for clients..\n\n");

        m_sppDataThread.init(-1, true);
    } else {
        showDialog("spp_open_server fail", "errno = " + QString::number(errno) );
    }
}
//! [8]

//! [9]
void ChatManager::sendSPPMessage(const QString &msg)
{
    // The send button in the SPPChat screen is clicked. The msg parameter is the message the user intends to send over SPP.
    if (m_sppDataThread.active()) {
        write(m_sppDataThread.getFD(), msg.toAscii().data(), msg.size());

        updateChatWindow("  We: " + msg);
    } else {
        updateChatWindow("ERROR - send with no file descriptor.");
    }
}
//! [9]

//! [10]
void ChatManager::closeSPPConnection()
{
    if (m_sppDataThread.isServer()) {
        m_sppDataThread.resetFD();
        bt_spp_close_server((char *) SPP_SERVICE_UUID);
    } else {
        if (m_sppDataThread.active()) {
            int closeFD = m_sppDataThread.getFD();
            m_sppDataThread.resetFD();
            bt_spp_close(closeFD);
        }
    }

    std::cout << "SPP Closed" << std::endl;
}
//! [10]

//! [11]
void ChatManager::incomingMessage(const QString &message)
{
    updateChatWindow("They: " + message);
}
//! [11]

//! [12]
void ChatManager::updateChatWindow(const QString &msg)
{
    if (m_chatHistory.size() > 0)
        m_chatHistory.append("\n");

    m_chatHistory.append(msg);

    emit chatHistoryChanged();
}
//! [12]

void ChatManager::showDialog(const QString &title, const QString &message)
{
    m_waitDialog.setTitle(title);
    m_waitDialog.setBody(message);
    m_waitDialog.show();
}

QString ChatManager::chatHistory() const
{
    return m_chatHistory;
}
