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

#include "Person.hpp"

Person::Person(QObject *parent)
    : QObject(parent)
{
}

Person::Person(const QString &id, const QString &firstname, const QString &lastname, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_firstName(firstname)
    , m_lastName(lastname)
{
}

QString Person::customerID() const
{
    return m_id;
}

QString Person::firstName() const
{
    return m_firstName;
}

QString Person::lastName() const
{
    return m_lastName;
}

void Person::setCustomerID(const QString &newId)
{
    if (newId != m_id) {
        m_id = newId;
        emit customerIDChanged(newId);
    }
}

void Person::setFirstName(const QString &newName)
{
    if (newName != m_firstName) {
        m_firstName = newName;
        emit firstNameChanged(newName);
    }
}

void Person::setLastName(const QString &newName)
{
    if (newName != m_lastName) {
        m_lastName = newName;
        emit lastNameChanged(newName);
    }
}
