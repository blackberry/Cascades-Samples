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

#ifndef USER_HPP
#define USER_HPP

#include <QString>

/*!
 * Value object relating to a user that will either register
 * to receive pushes or unregister from receiving pushes.
 */
class User
{
public:
    User();
    virtual ~User();

    QString userId() const;
    QString password() const;

    void setUserId(const QString &userId);
    void setPassword(const QString &password);

private:
    // The user ID of a user
    QString m_userId;

    // The user's password
    QString m_password;
};

#endif
