/*!
* Copyright (c) 2012, 2013 Research In Motion Limited.
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

#include "User.hpp"

namespace pushcollector
{

User::User()
{
}

User::User(const QVariantMap &map)
{
	setUserId(map["userId"].toString());
	setPassword(map["password"].toString());
}

User::~User()
{
}

QString User::userId() const
{
    return m_userId;
}

QString User::password() const
{
    return m_password;
}

bool User::isEmpty() const
{
	return (m_userId.isEmpty() && m_password.isEmpty());
}

void User::setUserId(const QString &userId)
{
    m_userId = userId;
}

void User::setPassword(const QString &password)
{
    m_password = password;
}

QVariantMap User::toMap() const
{
    QVariantMap map;
    map["userId"] = userId();
    map["password"] = password();

    return map;
}

} // namespace pushcollector
