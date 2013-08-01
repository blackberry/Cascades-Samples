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

#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <QObject>
#include <bb/cascades/Container>

#include "accelerationsensor.hpp"

// Forward declare some classes
class Board;
class Player;

/**
 * The GameController is the central class of this application which contains
 * the Board and Player objects and moves the player around depending on
 * current sensor values.
 */
//! [0]
class GameController : public QObject
{
    Q_OBJECT

public:
    GameController(QObject *parent = 0);

    /**
     * Set which Container object should be the board for the game.
     */
    void setBoard(bb::cascades::Container *board);

public Q_SLOTS:
    // Called when 'New Game' action selected in UI
    void newGame();

private Q_SLOTS:
    /**
     * Evalue the sensor data and move the player depending on sensor values.
     */
    void evaluateInput();

private:
    // The Container Object the game is run on
    bb::cascades::Container *m_boardContainer;

    // The Board object that contains the business logic for the maze board
    Board *m_board;

    // The Player object that contains the business logic for the main player
    Player *m_player;

    AccelerationSensor m_sensor;

};
//! [0]

#endif
