/*!
* Copyright (c) 2012, 2013  BlackBerry Limited.
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
#include "UserDAO.hpp"

#include <QSettings>
#include <QStringList>

UserDAO::UserDAO()
{

}

UserDAO::~UserDAO()
{

}

void UserDAO::save(const User &user)
{
    QSettings settings;
    settings.beginGroup(QSETTINGS_USER_GROUP);
    settings.setValue(USER_ID_KEY, user.userId());
    settings.setValue(USER_PASSWORD_KEY, user.password());
    settings.endGroup();
}

void UserDAO::remove()
{
    QSettings settings;
    settings.remove(QSETTINGS_USER_GROUP);
}

User& UserDAO::user()
{
    QSettings settings;

    settings.beginGroup(QSETTINGS_USER_GROUP);
    m_user.setUserId(settings.value(USER_ID_KEY).toString());
    m_user.setPassword(settings.value(USER_PASSWORD_KEY).toString());
    settings.endGroup();

    return m_user;
}

bool UserDAO::hasExisting() const
{
    int keysCount = 0;

    QSettings settings;
    settings.beginGroup(QSETTINGS_USER_GROUP);
    keysCount = settings.allKeys().count();
    settings.endGroup();

    return keysCount > 0;
}
