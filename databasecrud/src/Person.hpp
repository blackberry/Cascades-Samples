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

#ifndef PERSON_HPP
#define PERSON_HPP

#include <QObject>

class Person: public QObject
{
    Q_OBJECT

    // These are the properties that will be accessible by the datamodel in the view.
    Q_PROPERTY(QString customerID READ customerID WRITE setCustomerID NOTIFY customerIDChanged FINAL)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged FINAL)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged FINAL)

public:
    Person(QObject *parent = 0);
    Person(const QString &id, const QString &firstname, const QString &lastname, QObject *parent = 0);

    QString customerID() const;
    QString firstName() const;
    QString lastName() const;

    void setCustomerID(const QString &newId);
    void setFirstName(const QString &newName);
    void setLastName(const QString &newName);

Q_SIGNALS:
    void customerIDChanged(const QString &newId);
    void firstNameChanged(const QString &firstName);
    void lastNameChanged(const QString &lastName);

private:
    QString m_id;
    QString m_firstName;
    QString m_lastName;
};

#endif
