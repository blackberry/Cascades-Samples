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

#ifndef XANDOSDROID_HPP_
#define XANDOSDROID_HPP_

#include <QObject>
#include <QtNetwork/QTcpSocket>

namespace bb
{
    namespace system
    {
        class InvokeManager;
        class InvokeRequest;
    }
}

/**
 * This class represents the game droid player. Basically
 * containing the same matrices as it's UI counterpart in order
 * to keep the game in-sync. The difference is that this one
 * has logic to choose the next move based on the user selections.
 */
//! [0]
class xandosdroid: public QObject
{
    Q_OBJECT
public:
    xandosdroid();
    virtual ~xandosdroid();

    /**
     * This method marks the selection based on the player.
     * When invoked it updates the game matrix based on the
     * selection and player. Matrix row additions are perfomed
     * is selection made by user, and row subtraction if made
     * by droid.
     */
    void select(int index, int player);

    /**
     * Method containing the logic to dictate
     * the droids next grid selection based on the
     * current grid state.
     */
    int nextMove(int player);

public Q_SLOTS:
    /**
     * Method (slot) invoked when a invocation request
     * arrives form the invocation manager.
     */
    void onInvoked(const bb::system::InvokeRequest& request);

    /**
     * This method is invoked through the signal/slot mechanism
     * as well, when the socket is ready to read data.
     */
    void readyRead();

    /**
     * This method is invoked when the socket connects
     * to it's UI server socket for grid selection communication.
     */
    void connected();

    /**
     * This method is invoked when the socket disconnects.
     */
    void disconnected();
    /**
     * Initialization method to create slot/signal connections.
     */
    void init();
private:
    /**
     * Method which returns the choices available
     * to the droid. The selections can be filtered
     * based on the cell that you are looking for.
     */
    QList<int> availableChoices(int i = -1);

    /**
     * Method to transmit the droid selection back to the UI.
     */
    void sendSelection(const int index = -1);

    /**
     * Method to establish the socket connection with
     * the UI server socket.
     */
    void connectToServer();

    void resetGame();

    // The grid matrix containing the winning possibilities for each cell
    static int m_possibilities[9][9];

    // The size of the grid matrix
    int m_size;

    // The game matrix representing the current game state
    int m_gameMatrix[8];

    // place holder for the next move to be sent back to UI
    int m_nextMove;

    // The port on which to establish socket connection with server.
    int m_port;

    // Invoke manager used to issuing or receiving invoke requests
    bb::system::InvokeManager *m_invokeManager;
    // Place holder for the communication socket
    QTcpSocket *m_clientSocket;
};
//! [0]
#endif /* XANDOSDROID_HPP_ */
