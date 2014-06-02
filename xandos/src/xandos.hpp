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
#ifndef XANDOS_HPP_
#define XANDOS_HPP_

#include <QObject>

namespace bb
{
    namespace system
    {
        class InvokeManager;
        class InvokeRequest;
    }
}

/**
 * This class represents the backend mechanism for
 * the game. The user is always X and the droid always
 * represents O sprite.
 */
//! [0]
class xandos: public QObject
{
    Q_OBJECT
public:
    xandos(QObject *parent = 0);
    virtual ~xandos();

public Q_SLOTS:
    /**
     * Method for inputing the user and droid
     * selections into the game matrix. This causes
     * the game matrix to be changed with the new selection.
     *
     * @param player The player thats making the selection.
     * @param index The grid index being selected
     * @param send True if the selection is to be sent to the droid, or false otherwise
     */
    void select(int index, int player, bool send = true);

    /**
     * Reset the game back to it's prior to start of game state
     */
    void resetGame();

    /**
     * Start the droid for playing
     */
    void startDroid();

    /**
     * Terminate the droid
     */
    void stopDroid();

    /**
     * Method invoked by the signal/slot mechanism when
     * the droid has communicated it's selection to us
     */
    void droidSelection(const QString choice);

Q_SIGNALS:
    /**
     * Signal emitted when the user or droid has won or a
     * tie occured.
     *
     * @param player The player that has won or tied.
     */
    void won(int player);

    // Signal emitted when socket receives data indicating that
    // the droid is ready to play
    void droidReady();

    /**
     * Sginal emitting the user selection
     * to the droid, in order to keep in synce both
     * game matrices.
     */
    void sendSelection(int index);

private:

    /**
     * Convenience method to check the grid
     * matrix that there are no more selections available
     */
    bool noMoreSelections();

    // The grid matrix, containing all the winning possibilites
    static int m_possibilities[9][9];
    // size of the grid matrix
    int m_size;
    // The game matrix, representing the current state of the game
    int m_gameMatrix[8];
    // Invoke manager to start/stop the headless droid
    bb::system::InvokeManager *m_invokeManager;
};
//! [0]
#endif /* XANDOS_HPP_ */
