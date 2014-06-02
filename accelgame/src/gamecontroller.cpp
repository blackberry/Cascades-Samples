/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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

#include "gamecontroller.hpp"

#include "board.hpp"
#include "player.hpp"
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

//! [0]
GameController::GameController(QObject *parent)
    : QObject(parent)
    , m_boardContainer(0)
    , m_board(0)
    , m_player(0)
{
    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make the GameController and Application object available to the UI as context properties
    qml->setContextProperty("_gameController", this);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();

    if(appPage) {
        // Tell the gameController which Container should be used as board
        setBoard(appPage->findChild<Container *>("board"));

        Application::instance()->setScene(appPage);
    }

    // Start the sensor to gather data
    m_sensor.start();
}
//! [0]

//! [1]
void GameController::setBoard(bb::cascades::Container *board)
{
    /*
     * Now that we know on which Container we are supposed to work,
     * create our Board and Player objects that handle logic.
     */
    m_boardContainer = board;
    m_board = new Board(m_boardContainer, this);
    m_board->reset();

    m_player = new Player(m_board, this);
    m_player->reset();

    // Whenever the player has finished its move animation we check for new input
    bool ok = connect(m_player, SIGNAL(moved()), SLOT(evaluateInput()), Qt::QueuedConnection);
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    // and to kick things off...
    evaluateInput();
}
//! [1]

//! [2]
void GameController::newGame()
{
    // For a new game we let the Board generate a new random maze...
    m_board->reset();

    // ... and move the player back to its initial position
    m_player->reset();

    m_sensor.start();
    evaluateInput();
}
//! [2]

//! [3]
void GameController::evaluateInput()
{
    if (!m_player)
        return;

    // Move the player depending on the current x/y/z values of the sensor
    if (m_sensor.x() > 0.8)
        m_player->goLeft();
    else if (m_sensor.x() < -0.8)
        m_player->goRight();
    else if (m_sensor.y() > 0.8)
        m_player->goDown();
    else if (m_sensor.y() < -0.8)
        m_player->goUp();
    else // Try again in 80 milliseconds
        QTimer::singleShot(80, this, SLOT(evaluateInput()));
}
//! [3]
