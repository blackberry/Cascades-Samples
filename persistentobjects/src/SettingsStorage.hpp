/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#ifndef SETTINGSSTORAGE_HPP
#define SETTINGSSTORAGE_HPP

#include "Storage.hpp"

class Person;

class SettingsStorage: public Storage
{
public:
    SettingsStorage();
    virtual ~SettingsStorage();

    virtual bool clear();
    virtual int load(int& lastID, GroupDataModel *model);
    virtual bool save(int lastID, GroupDataModel *model);

private:
    static const QString m_author; // for creating settings
    static const QString m_appName; // for creating settings

    //Keys for the settings file:
    static const QString m_personListSettingsKey;
    static const QString m_lastCustomerIDKey;
    static const QString m_customerIDKey;
    static const QString m_firstNameKey;
    static const QString m_lastNameKey;

    bool loadPerson(const QSettings& settings, Person*& person) const;
};

#endif
