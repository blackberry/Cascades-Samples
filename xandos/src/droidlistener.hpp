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

#ifndef DROIDLISTENER_HPP_
#define DROIDLISTENER_HPP_

#include <QObject>

class QTcpServer;
class QTcpSocket;


/**
 * This class serves mainly as a server socket listening
 * for client connection(s). The client being a the droid
 * in this situation. It serves as a communication line
 * to exchange grid selections between the user and the droid.
 */
//! [0]
class droidlistener: public QObject {
    Q_OBJECT
public:
    droidlistener(QObject *parent = 0);
    virtual ~droidlistener();
    /**
     * Method to start listening on the server socket
     * for new connections.
     */
    void listen();

public Q_SLOTS:
    /**
     * This method is invoked through the signal/slot
     * mechanism when a new connection occurs on the server socket.
     */
    void newConnection();

    /**
     * This method is invoked through the signal/slot mechanism
     * as well, when the socket is ready to read data.
     */
    void readyRead();
    /**
     * This method is invoked when the socket connection disconnects.
     */
    void disconnected();

Q_SIGNALS:
    /*
     * Signal used to inform the listening parties
     * of the selection made by the droid.
     */
    void droidSelection(const QString choice);
private:
    // The port used for the server socket
    int m_port;
    //The server socket variable
    QTcpServer *m_server;
    //The socket that is created when a connection with the client is made
    QTcpSocket *m_socket;
};
//! [0]
#endif /* DROIDLISTENER_HPP_ */
