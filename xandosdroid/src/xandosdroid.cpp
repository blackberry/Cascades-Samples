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
#include <bb/cascades/Application>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <cstdlib>

//! [0]
// Grid selections with possible winning scenarios
int xandosdroid::m_possibilities[9][9] = { { 1, 1, 0, 0, 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0, 1, 0, 0, 0 }, { 0, 1, 0, 0, 0, 0, 1, 1, 0 }
                                            , { 0, 0, 1, 0, 1, 0, 0, 0, 0 }, { 1, 0, 1, 0, 0, 1, 0, 1, 0 }, { 0, 0, 1, 0, 0, 0, 1, 0, 0 }
                                            , { 0, 0, 0, 1, 1, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0, 1, 0, 0, 0 }, { 1, 0, 0, 1, 0, 0, 1, 0, 0 } };
//! [0]
//! [1]
xandosdroid::xandosdroid(bb::cascades::Application *parent)
    : QObject(parent)
    , m_size(sizeof(m_possibilities) / sizeof(m_possibilities[0]))
    , m_gameMatrix({ 0, 0, 0, 0, 0, 0, 0, 0 })
    , m_nextMove(-1)
    , m_port(9876)
    , m_invokeManager(new bb::system::InvokeManager(this))
    , m_app(parent)
    , m_clientSocket(new QTcpSocket(this)) {
    // establish signal/slot connection with invocation manager
    bool ok = connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                      this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    Q_ASSERT(ok);
    // establish signal/slot connection with socket for being informed of new established connection
    ok = connect(m_clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [1]
xandosdroid::~xandosdroid() {
}
//! [2]
void xandosdroid::onInvoked(const bb::system::InvokeRequest& request) {
    if (request.action().compare("bb.action.START") == 0) {
        qDebug() << "XandOsDroid : start requested";
        // once the headless is started, communicate back to ui that its ready to play
        connectToServer();
    } else if (request.action().compare("bb.action.STOP") == 0) {
        qDebug() << "XandOsDroid: stop request";
        // terminate headless droid
        terminateDroid();
    } else if (request.action().compare("bb.action.CHOICE") == 0) {
        int choice = QString(request.data()).toInt();
        qDebug() << "XandOsDroid: received user choice: " << choice;
        // mark the user selection in the grid matrix and update the
        // game matrix state
        select(choice, 1);
        // verify there are still moves available
        if (availableChoices().isEmpty()) {
            qDebug() << "XandOsDroid: game over!";
            terminateDroid();
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

    } else {
        qDebug() << "XandOsDroid : unknown service request "
                << request.action();
    }
}
//! [2]
//! [3]
void xandosdroid::select(int index, int player) {
    if(0 > index || 8 < index) {
        qDebug() << "XandOsDroid: invalid index -> index";
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
            terminateDroid();
        }
    }
}
//! [3]
//! [4]
QList<int> xandosdroid::availableChoices(int index) {
    QList<int> choices;
    for (int i = 0; i < m_size; i++) {
        // Check the selections, and base the result on the cell index if index is set.
        if (0 == m_possibilities[i][8]
                && (index == -1 || 1 == m_possibilities[i][index])) {
            choices << i;
        }
    }
    return choices;
}
//! [4]
//! [5]
void xandosdroid::sendSelection(int index) {
    if (-1 != m_nextMove) {
        qDebug() << "XandOsDroid: failed to send last move, check code";
        return;
    }
    m_nextMove = index;
    connectToServer();
}

void xandosdroid::connectToServer() {
    qWarning() << "XandOsDroid: connecting to server socket";
    if (!m_clientSocket->isOpen()) {
        m_clientSocket->connectToHost(QHostAddress::LocalHost, m_port);
    } else {
        connected();
    }
}

void xandosdroid::connected() {
    qWarning() << "XandOsDroid: Connection established.";
    m_clientSocket->write(QByteArray::number(m_nextMove));
    m_clientSocket->flush();
    select(m_nextMove, -1);
    m_nextMove = -1;
}
//! [5]
//! [6]
int xandosdroid::nextMove(int player) {
    QList<int> moves;
    for (int i = 0; i < 8; i++) {
        // check for next move based on players
        // current existing 2 sequences, otherwise
        // block player's 1 sequence selections.
        if ((1 * player) == m_gameMatrix[i]) {
            moves << availableChoices(i);
        } else if ((2 * player) == m_gameMatrix[i]) {
            moves.clear();
            moves << availableChoices(i);
            break;
        }
    }
    if (moves.isEmpty()) {
        return -1;
    }
    // return move based on a random selection of possible next move choices
    return moves.at(rand() % moves.size());
}
//! [6]
//! [7]
inline void xandosdroid::terminateDroid() {
    qDebug() << "XandOsDroid: terminating droid";
    m_clientSocket->close();
    m_clientSocket->deleteLater();
    if (!m_app->requestExit()) {
        qDebug() << "XandOsDroid: error trying to terminate droid";
    }
}
//! [7]
