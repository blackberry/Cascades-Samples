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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <bb/cascades/AbstractAnimation>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>

// Forward declare
class Board;

/**
 * The Player class encapsulates the business logic of a player on the mace board.
 * It provides methods to change direction and position of the player.
 */
//! [0]
class Player : public QObject, public QScriptable
{
    Q_OBJECT

public:
    /**
     * Constructor takes the Board object, where the player should play on
     */
    Player(Board *board, QObject *parent = 0);
    ~Player();

public Q_SLOTS:
    // Reset the player to its initial state
    void reset();

    // These methods turn and move the player in a given direction
    void goUp();
    void goRight();
    void goDown();
    void goLeft();

Q_SIGNALS:
    // Emitted whenever the user has finished its move animation
    void moved();

private:
    // Describes the possible directions the player can move to
    enum Direction {
        Up, Right, Down, Left
    };

    // Move the player in a given direction
    void go(Direction direction);

    // The Board object the player is playing on
    Board *m_board;

    // The tile that represents the player in the UI
    bb::cascades::ImageView *m_playerTile;

    // The direction the player is currently moving
    Direction m_currentDirection;

    // The current position of the player on the board
    QPoint m_currentPosition;

    // The move animation that is current running
    bb::cascades::AbstractAnimation *m_currentAnimation;
};
//! [0]

#endif
