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

#include "ScoreloopData.hpp"

#include <QDebug>

// A helper method to convert a SC_String_h to a QString
//! [0]
static QString asQString(SC_String_h scString)
{
    if (scString) {
        const char *cp = SC_String_GetData(scString);
        if (cp) {
            return QString::fromUtf8(cp);
        }
    }

    return QString();
}
//! [0]

//! [1]
ScoreloopData::ScoreloopData(SC_Client_h client, QObject *parent)
    : QObject(parent)
    , m_client(client)
    , m_userController(0)
    , m_user(0)
{
    // We pass 'this' as cookie for the callback, so we can retrieve the it when in the
    // static callback
    SC_Client_CreateUserController(m_client, &m_userController, userControllerCallback, this);
}
//! [1]

//! [2]
void ScoreloopData::load()
{
    SC_UserController_LoadUser(m_userController);
}
//! [2]

//! [3]
void ScoreloopData::userControllerCallback(void* cookie, SC_Error_t status)
{
    // This callback is registered when creating the controller
    // it will be invoked every time the controller completes a request

    // Since this is a static function, we passed our object through as cookie
    ScoreloopData* self = static_cast<ScoreloopData*>(cookie);

    if (status == SC_OK) {
        // Loading went ok, we can update our properties
        self->m_user = SC_UserController_GetUser(self->m_userController);
        emit self->userNameChanged();
    } else {
        // You might want to add proper error handling.
        qDebug() << SC_MapErrorToStr(status);
    }
}
//! [3]

//! [4]
QString ScoreloopData::userName() const
{
    if (m_user) {
        return asQString(SC_User_GetLogin(m_user));
    }

    return QString();
}
//! [4]
