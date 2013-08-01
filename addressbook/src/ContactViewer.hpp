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

#ifndef CONTACTVIEWER_HPP
#define CONTACTVIEWER_HPP

#include <bb/pim/contacts/ContactService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the contact viewer
 *
 * The ContactViewer class is an UI-independent representation of the contact viewer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of loading a contact from the persistent storage,
 * provide its data as properties and update the properties if the contact has
 * changed in the storage backend.
 */
//! [0]
class ContactViewer : public QObject
{
    Q_OBJECT

    // The data properties of the contact that is displayed
    Q_PROPERTY(QString firstName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(QDateTime birthday READ birthday NOTIFY birthdayChanged)
    Q_PROPERTY(QString formattedBirthday READ formattedBirthday NOTIFY birthdayChanged)
    Q_PROPERTY(QString email READ email NOTIFY emailChanged)

public:
    ContactViewer(bb::pim::contacts::ContactService *service, QObject *parent = 0);

    // Sets the ID of the contact that should be displayed.
    void setContactId(bb::pim::contacts::ContactId contactId);

Q_SIGNALS:
    // The change notification signals of the properties
    void firstNameChanged();
    void lastNameChanged();
    void birthdayChanged();
    void emailChanged();

private Q_SLOTS:
    /**
     * This slot is invoked whenever the contact service reports that a contact has been changed.
     */
    void contactsChanged(const QList<int> &ids);

private:
    // The accessor methods of the properties
    QString firstName() const;
    QString lastName() const;
    QDateTime birthday() const;
    QString formattedBirthday() const;
    QString email() const;

    // Loads the contact from the persistent storage and updates the properties
    void updateContact();

    // The central object to access the contact service
    bb::pim::contacts::ContactService* m_contactService;

    // The ID of the contact that is displayed
    bb::pim::contacts::ContactId m_contactId;

    // The property values
    QString m_firstName;
    QString m_lastName;
    QDateTime m_birthday;
    QString m_email;
};
//! [0]

#endif
