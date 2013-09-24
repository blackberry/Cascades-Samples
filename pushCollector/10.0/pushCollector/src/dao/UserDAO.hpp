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

#ifndef USERDAO_HPP
#define USERDAO_HPP

#include "../vo/User.hpp"

#define QSETTINGS_USER_GROUP "user"
#define USER_ID_KEY "providerAppId"
#define USER_PASSWORD_KEY "ppgUrl"

/*!
 * DAO related to the handling / processing of users that will either register
 * to receive pushes or unregister from receiving pushes.
 */
class UserDAO
{
public:
    UserDAO();
    ~UserDAO();

    void save(const User &user);

    void remove();

    User& user();

    bool hasExisting() const;

private:
    User m_user;
};


#endif
