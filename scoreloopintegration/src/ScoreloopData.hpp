/*
* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef SCORELOOPDATA_HPP
#define SCORELOOPDATA_HPP

#include <QtCore/QObject>

#include <scoreloop/scoreloopcore.h>

/**
 * @short The central class that provides Scoreloop functionality to the UI
 *
 * The ScoreloopData class uses the low-level C API internally to access the
 * Scoreloop service and provides a high-level API (properties, signals and slots)
 * so that the UI can access information, trigger some action or be informed about
 * state changes.
 */
//! [0]
class ScoreloopData : public QObject
{
    Q_OBJECT

    // The name of the user at the Scoreloop service
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged);

public:
    // Creates a new ScoreloopData object
    ScoreloopData(SC_Client_h client, QObject *parent = 0);

public Q_SLOTS:
    // Triggers the load of the user name from the Scoreloop service
    void load();

Q_SIGNALS:
    // The change notification signal of the property
    void userNameChanged();

private:
    // A helper method for integration with the low-level C API
    static void userControllerCallback(void* cookie, SC_Error_t status);

    // The accessor method of the property
    QString userName() const;

    // The handle to access the Scoreloop service
    SC_Client_h m_client;

    // The handle to access the Scoreloop user service
    SC_UserController_h m_userController;

    // The handle that represents the Scoreloop user
    SC_User_h m_user;
};
//! [0]

#endif
