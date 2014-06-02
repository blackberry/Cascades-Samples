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
#include "xandosdroid.hpp"
#include <bb/Application>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

#include <QDebug>
#include <cstdlib>
#include <time.h>

//! [0]
// Grid selections with possible winning scenarios
int xandosdroid::m_possibilities[9][9] = { { 1, 1, 0, 0, 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0, 1, 0, 0, 0 }, { 0, 1, 0, 0, 0, 0, 1, 1, 0 },
                                           { 0, 0, 1, 0, 1, 0, 0, 0, 0 }, { 1, 0, 1, 0, 0, 1, 0, 1, 0 }, { 0, 0, 1, 0, 0, 0, 1, 0, 0 },
                                           { 0, 0, 0, 1, 1, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0, 1, 0, 0, 0 }, { 1, 0, 0, 1, 0, 0, 1, 0, 0 } };
//! [0]
//! [1]
xandosdroid::xandosdroid()
    : QObject()
    , m_size(sizeof(m_possibilities) / sizeof(m_possibilities[0]))
    , m_gameMatrix({ 0, 0, 0, 0, 0, 0, 0, 0 }), m_nextMove(-1)
    , m_port(9876)
    , m_invokeManager(new bb::system::InvokeManager(this))
    , m_clientSocket(new QTcpSocket(this))
{
    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
    // log the service PID
    qDebug() << "PID------------" << QString::number(QCoreApplication::applicationPid());
}

void xandosdroid::init()
{
    srand (time(NULL));
    // establish signal/slot connection with invocation manager
    bool ok = connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                      this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    Q_ASSERT (ok);
    // establish signal/slot connection with socket for being informed of new established connection
    ok = connect(m_clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [1]
xandosdroid::~xandosdroid()
{
    if (m_clientSocket) {
        qDebug() << "XandOsDroid: closing socket";
        disconnect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        m_clientSocket->close();
        m_clientSocket->deleteLater();
    }
}
//! [2]
void xandosdroid::onInvoked(const bb::system::InvokeRequest& request)
{
    if (request.action().compare("bb.action.START") == 0) {
        qDebug() << "XandOsDroid : start requested";
        // once the headless is started, communicate back to ui that its ready to play
        connectToServer();
    } else if (request.action().compare("bb.action.STOP") == 0) {
        qDebug() << "XandOsDroid: stop requested";
        // terminate headless droid
        disconnected();
    } else {
        qDebug() << "XandOsDroid : unknown service request " << request.action();
    }
}
//! [2]
//! [3]
void xandosdroid::select(int index, int player)
{
    if (0 > index || 8 < index) {
        qDebug() << "XandOsDroid: invalid index -> " << index;
        return;
    }
    // update the grid matrix with the selection
    m_possibilities[index][8] = 1;
    for (int i = 0; i < 8; i++) {
        // update game matrix based on the selection
        m_gameMatrix[i] += m_possibilities[index][i] * player;
        // verify if droid has 3 in a row sequences and terminate if so.
        if (0 != m_gameMatrix[i] && m_gameMatrix[i] % 3 == 0) {
            qDebug() << "XandOsDroid: droid has won";
            disconnected();
        }
    }
}
//! [3]
//! [4]
QList<int> xandosdroid::availableChoices(int index)
{
    QList<int> choices;
    for (int i = 0; i < m_size; i++) {
        // Check the selections, and base the result on the cell index if index is set.
        if (0 == m_possibilities[i][8] && (index == -1 || 1 == m_possibilities[i][index])) {
            choices << i;
        }
    }
    return choices;
}
//! [4]

void xandosdroid::sendSelection(const int index)
{
    m_nextMove = index;
    connectToServer();
}
//![5]
void xandosdroid::connectToServer()
{
    if (!m_clientSocket->isOpen()) {
        qDebug() << "XandOsDroid: connecting to server socket";
        m_clientSocket->connectToHost(QHostAddress::LocalHost, m_port);
        bool ok = connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        Q_ASSERT(ok);
        ok = connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
    } else {
        connected();
    }
}

void xandosdroid::connected()
{
    qWarning() << "XandOsDroid: writing to socket.";
    m_clientSocket->write(QByteArray::number(m_nextMove));
    m_clientSocket->flush();
    select(m_nextMove, -1);
}

void xandosdroid::readyRead()
{
    QByteArray ba = m_clientSocket->read(20);
    qDebug() << "XandOsDroid: received move from user: " << ba;
    const int choice = ba.toInt();
    // mark the user selection in the grid matrix and update the
    // game matrix state
    select(choice, 1);
    // verify there are still moves available
    if (availableChoices().isEmpty()) {
        qDebug() << "XandOsDroid: game over!";
        disconnected();
        return;
    }
    // Check whether you have any two in a row sequences
    int nextM = nextMove(-2);
    // If no, than block the user based on his possible selections for
    // a consecutive sequence
    if (-1 == nextM) {
        nextM = nextMove(1);
    }
    qDebug() << "XandOsDroid: droid selection: " << nextM;
    // send your next selection to the UI.
    sendSelection(nextM);
}

void xandosdroid::disconnected()
{
    qDebug() << "XandOsDroid: disconnected...";
    bb::Application::instance()->quit();
}
//! [5]
//! [6]
int xandosdroid::nextMove(int player)
{
    QList<int> moves;
    bool blockUserWin = false;
    for (int i = 0; i < 8; i++) {
        // check for next move based on players
        // current existing 2 sequences, otherwise
        // block player's 1 sequence selections.
        if ((1 * player) == m_gameMatrix[i] && !blockUserWin) {
            moves << availableChoices(i);
        } else if ((2 * player) == m_gameMatrix[i]) {
            if (!blockUserWin) {
                moves.clear();
                blockUserWin = true;
            }
            moves << availableChoices(i);
        }
    }
    if (moves.isEmpty()) {
        return -1;
    }
    // return move based on a random selection of possible next move choices
    return moves.at(rand() % moves.size());
}
//! [6]

void xandosdroid::resetGame()
{
    for (int i = 0; i < 9; i++) {
        m_possibilities[i][8] = 0;
        if (i < 8) {
            m_gameMatrix[i] = 0;
        }
    }
    m_nextMove = -1;
}
