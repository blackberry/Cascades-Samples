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

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/ParallelAnimation>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/StockCurve>
#include <bb/cascades/TranslateTransition>

#include "board.hpp"
#include "player.hpp"

using namespace bb::cascades;

const static int s_tileSize = 50;

//! [0]
Player::Player(Board *board, QObject *parent)
    : QObject(parent)
    , m_board(board)
    , m_playerTile(new ImageView)
    , m_currentDirection(Up)
    , m_currentPosition(0, 0)
    , m_currentAnimation(0)
{

    // Initialise the player tile and add it to the board container
    m_playerTile->setPreferredWidth(s_tileSize);
    m_playerTile->setPreferredHeight(s_tileSize);
    m_playerTile->setImage(Image(QUrl("asset:///images/player.png")));
    m_board->board()->add(m_playerTile);

    // Ensure that the x/y position is really 0,0 otherwise using the
    // translationX/translationY properties does not work as expected.
    AbsoluteLayoutProperties *props
        = qobject_cast<AbsoluteLayoutProperties*>(m_playerTile->layoutProperties());

    if (props) {
        props->setPositionX(0);
        props->setPositionY(0);
    }

}
//! [0]

Player::~Player()
{

    // Remove the tile from the UI when this player is deleted
    m_board->board()->remove(m_playerTile);
    delete m_playerTile;

    delete m_currentAnimation;

}

void Player::reset()
{

    // Reset the current direction
    m_currentDirection = Up;
    m_playerTile->setRotationZ(0);

    // Reset the current position
    m_currentPosition = QPoint(0, 0);
    m_playerTile->setTranslationX(0);
    m_playerTile->setTranslationY(0);

}

//! [1]
void Player::go(Direction direction)
{

    // Update the current direction
    m_currentDirection = direction;

    // A falg to store whether the player can actually move
    bool move = false;

    // Depending on the new direction rotate the plater and move it one step
    switch (direction) {
    case Up:
        if (m_board->canMoveTo(m_currentPosition.x(), m_currentPosition.y() - 1)) {
            move = true;
            m_currentPosition.setY(m_currentPosition.y() - 1);
        }
        break;
    case Right:
        if (m_board->canMoveTo(m_currentPosition.x() + 1, m_currentPosition.y())) {
            move = true;
            m_currentPosition.setX(m_currentPosition.x() + 1);
        }
        break;
    case Down:
        if (m_board->canMoveTo(m_currentPosition.x(), m_currentPosition.y() + 1)) {
            move = true;
            m_currentPosition.setY(m_currentPosition.y() + 1);
        }
        break;
    case Left:
        if (m_board->canMoveTo(m_currentPosition.x() - 1, m_currentPosition.y())) {
            move = true;
            m_currentPosition.setX(m_currentPosition.x() - 1);
        }
        break;
    default:
        break;
    }

    // Update the position of the player tile on screen

    // Stop any previously running animation
    if (m_currentAnimation) {

        m_currentAnimation->stop();
        m_currentAnimation->deleteLater();

    }

    // Calculate the rotation of the player image depending on the direction
    const int rotationOffset
        = (move ? ((m_currentDirection == Up || m_currentDirection == Left)
            ? -115 : 115) : 0);

    // The animation should take 80 milliseconds
    const int duration = 80;

    // Create a new move animation
    // It's a parallel animation that consists of two translate transition for x & y
    // directions and the rotate transition.
    m_currentAnimation = ParallelAnimation::create(m_playerTile)
        .add(TranslateTransition::create()
            .toX(m_currentPosition.x() * s_tileSize)
            .duration(duration).easingCurve(StockCurve::Linear))
        .add(TranslateTransition::create()
            .toY(m_currentPosition.y() * s_tileSize)
            .duration(duration).easingCurve(StockCurve::Linear))
        .add(RotateTransition::create()
            .toAngleZ(m_playerTile->rotationZ() + rotationOffset)
            .duration(duration).easingCurve(StockCurve::Linear));

    // Emit the moved() signal when the animation has finished,
    // so that the GameController can evaluate the next input
    bool ok = connect(m_currentAnimation, SIGNAL(ended()), SIGNAL(moved()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    // Start the animation
    m_currentAnimation->play();

}
//! [1]

//! [2]
void Player::goUp()
{
    go(Up);
}
//! [2]

void Player::goRight()
{
    go(Right);
}

void Player::goDown()
{
    go(Down);
}

void Player::goLeft()
{
    go(Left);
}

