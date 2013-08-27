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

#include <bb/cascades/Color>
#include <bb/cascades/ImageView>

#include "board.hpp"

using namespace bb::cascades;

const static int s_boardDimension = 9;
const static int s_tileSize = 50;

//! [0]
Board::Board(bb::cascades::Container *boardContainer, QObject *parent)
    : QObject(parent)
    , m_board(boardContainer)
{

    // Initialize the random number generator so that we can use it for dynamic
    // block distribution
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    // Initialise the block map with 'false' for all cells -> no blocks available
    for (int x = 0; x < s_boardDimension; x++) {
        m_blockMap << QVector<bool>();
        for (int y = 0; y < s_boardDimension; y++)
            m_blockMap[x] << false;
    }

}
//! [0]

bb::cascades::Container * Board::board() const
{
    return m_board;
}

//! [1]
bool Board::canMoveTo(int x, int y) const
{

    // We can't move beyond the borders of the board
    if (x < 0 || x >= s_boardDimension || y < 0 || y >= s_boardDimension)
        return false;

    // We can't move to coordinate where a block is located
    return m_blockMap[x][y] == false;

}
//! [1]

//! [2]
void Board::reset()
{

    // Remove all block controls from the board...
    Q_FOREACH (Control *block, m_blocks) {
        m_board->remove(block);
    }

    // ... and delete them
    qDeleteAll(m_blocks);
    m_blocks.clear();

    // Clear out internal block map
    for (int x = 0; x < s_boardDimension; x++)
        for (int y = 0; y < s_boardDimension; y++)
            m_blockMap[x][y] = false;

    QVector<QPoint> blockCoordinates;

    // For dynamic block distribution we use random coordinates
    for (int i = 0; i < 20; i++) {

        const QPoint newPoint(qrand() % s_boardDimension, qrand() % s_boardDimension);

        // Contains a block already
        if (blockCoordinates.contains(newPoint))
            continue;

        // That's the starting place for the player
        if (newPoint == QPoint(0, 0))
            continue;

        blockCoordinates << newPoint;

    }

    // Generate the new blocks
    Q_FOREACH (const QPoint position, blockCoordinates) {

        // Mark as occupied in blockMap
        m_blockMap[position.x()][position.y()] = true;

        // Create block tile
        ImageView *block = new ImageView();
        block->setPreferredWidth(50);
        block->setPreferredHeight(50);
        block->setImage(Image(QUrl("asset:///images/block.png")));
        block->setTranslationX(position.x() * s_tileSize);
        block->setTranslationY(position.y() * s_tileSize);

        // Add the block tile to the board container...
        m_board->add(block);

        // ... and store the object in our internal list, so that we can
        // clean it up later
        m_blocks << block;

    }
}
//! [2]
