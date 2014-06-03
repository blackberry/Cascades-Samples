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

#include "xandos.hpp"

#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <bb/cascades/ImageView>
#include <bb/cascades/Application>

#include <QDebug>
#include <QtNetwork/QTcpSocket>

//! [0]
// The grid matrix with all the winning possibilities
// representing the wins from left to right {D1,H1,H2,H3,V1,V2,V3,D2}
int xandos::m_possibilities[9][9] = { { 1, 1, 0, 0, 1, 0, 0, 0, 0 }, { 0, 1, 0, 0, 0, 1, 0, 0, 0 }, { 0, 1, 0, 0, 0, 0, 1, 1, 0 },
                                      { 0, 0, 1, 0, 1, 0, 0, 0, 0 }, { 1, 0, 1, 0, 0, 1, 0, 1, 0 }, { 0, 0, 1, 0, 0, 0, 1, 0, 0 },
                                      { 0, 0, 0, 1, 1, 0, 0, 1, 0 }, { 0, 0, 0, 1, 0, 1, 0, 0, 0 }, { 1, 0, 0, 1, 0, 0, 1, 0, 0 } };
//! [0]
//! [1]
xandos::xandos(QObject *parent)
    : QObject(parent)
    , m_size(sizeof(m_possibilities) / sizeof(m_possibilities[0]))
    , m_gameMatrix({ 0, 0, 0, 0, 0, 0, 0, 0 })
    , m_invokeManager(new bb::system::InvokeManager(this))
{
}
//! [1]
xandos::~xandos()
{
}
//! [2]
void xandos::select(int index, int player, bool send)
{
    if (0 > index || 8 < index) {
        qDebug() << "XandOs: invalid index ->" << index;
        return;
    }

    m_possibilities[index][8] = 1;
    for (int i = 0; i < 8; i++) {
        // Add the matrix at the selected index to the game matrix
        // multiply the matrix values based on player, if its user than
        // matrix row addition is used, matrix subtraction otherwise for the droid
        m_gameMatrix[i] += m_possibilities[index][i] * player;
        if (0 != m_gameMatrix[i] && m_gameMatrix[i] % 3 == 0) {
            // emit wining signal if any matrix value equals 3
            Q_EMIT won(m_gameMatrix[i] / 3);
            stopDroid();
            return;
        }
    }
    if (noMoreSelections()) {
        // emit tie signal if there are no more choices available
        // since there is only two players (1,-1) that means -2
        // represents a tie
        Q_EMIT won(-2);
        stopDroid();
        return;
    }
    if (send) {
        Q_EMIT sendSelection(index);
    }
}
//! [2]
void xandos::resetGame()
{
    for (int i = 0; i < m_size; i++) {
        // reset all grid selections to 0's
        m_possibilities[i][8] = 0;
        if (0 < i) {
            // reset game matrix back to all 0's
            m_gameMatrix[i - 1] = 0;
        }
    }
}
//! [3]
void xandos::startDroid()
{
    qDebug() << "requesting to start droid";
    bb::system::InvokeRequest request;
    request.setTarget("com.example.xandos.droid");
    request.setAction("bb.action.START");
    request.setMimeType("text/plain");
    bb::system::InvokeTargetReply *reply = m_invokeManager->invoke(request);
    if (!reply) {
        qDebug() << "failed to start droid: " << reply->errorCode();
        reply->deleteLater();
    }
}

void xandos::stopDroid()
{
    qDebug() << "XandOs: requesting to terminate droid";
    bb::system::InvokeRequest request;
    request.setTarget("com.example.xandos.droid");
    request.setAction("bb.action.STOP");
    bb::system::InvokeTargetReply *reply = m_invokeManager->invoke(request);
    if (!reply) {
        qWarning() << "failed to stop droid: " << reply->errorCode();
        reply->deleteLater();
    }
}
//! [3]
//! [4]
void xandos::droidSelection(const QString choice)
{
    // the first droid choice being a -1 that represents
    // the droid establishing communication and is in ready state to play
    if (-1 == choice.toInt()) {
        qDebug() << "XandOs: emit droid ready";
        Q_EMIT droidReady();
        return;
    }
    // Find the droid grid choice and set it to 0
    bb::cascades::ImageView * image = bb::cascades::Application::instance()->findChild<bb::cascades::ImageView*>(choice);
    if (image) {
        image->setImageSource(QUrl("asset:///images/o.png"));
        select(choice.toInt(), -1, false);
    } else {
        qDebug() << "XandOs: failed to find ImageView: " << choice;
    }
}
//! [4]

//! [6]
bool xandos::noMoreSelections()
{
    for (int i = 0; i < m_size; i++) {
        if (0 == m_possibilities[i][8]) {
            return false;
        }
    }
    return true;
}
//! [6]
