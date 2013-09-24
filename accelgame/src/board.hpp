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

#ifndef BOARD_HPP
#define BOARD_HPP

#include <QObject>
#include <bb/cascades/Container>

/**
 * The Board class contains all the business logic for handling the maze board.
 * It generate a distribution of blocks on the mazze and it also generates the
 * tiles (ImageViews) that are displayed in the UI.
 */
//! [0]
class Board : public QObject
{
    Q_OBJECT

public:
    // We take the Container that represents the maze board as a parameter
    Board(bb::cascades::Container *boardContainer, QObject *parent = 0);

    /**
     * Returns whether a player can move to the given position.
     * The position is given in logical coordinates (0-9).
     */
    bool canMoveTo(int x, int y) const;

    /**
     * The Container that represents the maze board.
     */
    bb::cascades::Container *board() const;

public Q_SLOTS:
    /**
     * Reset the maze board and regenerate all blocks.
     */
    void reset();

private:
    // The Container object that represents the maze board in the UI
    bb::cascades::Container *m_board;

    // The list of block tiles that we created
    QVector<bb::cascades::Control *> m_blocks;

    // The map where we store the locations of the blocks inside the maze
    QVector<QVector<bool> > m_blockMap;

};
//! [0]

#endif
