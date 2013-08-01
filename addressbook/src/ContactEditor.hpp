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

#ifndef CONTACTEDITOR_HPP
#define CONTACTEDITOR_HPP

#include <bb/pim/contacts/ContactService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the contact editor
 *
 * The ContactEditor class is an UI-independent representation of the contact editor,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of creating a new contact or updating an existing
 * one.
 */
//! [0]
class ContactEditor : public QObject
{
    Q_OBJECT

    // The data properties of the contact that is created or updated
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QDateTime birthday READ birthday WRITE setBirthday NOTIFY birthdayChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)

    // Defines whether the editor is in 'create' or 'edit' mode
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_ENUMS(Mode)

public:
    /**
     * Describes the mode of the contact editor.
     * The mode information are used to adapt the behavior of the editor and
     * provide hints to the UI.
     */
    enum Mode {
        CreateMode,
        EditMode
    };

    ContactEditor(bb::pim::contacts::ContactService *service, QObject *parent = 0);

    void setMode(Mode mode);
    Mode mode() const;

public Q_SLOTS:
    /**
     * Loads the contact with the given ID.
     */
    void loadContact(bb::pim::contacts::ContactId contactId);

    /**
     * Save the currently loaded contact if in 'edit' mode or creates a new one
     * if in 'create' mode.
     */
    void saveContact();

    /**
     * Resets all fields of the contact editor.
     */
    void reset();

Q_SIGNALS:
    // The change notification signals of the properties
    void firstNameChanged();
    void lastNameChanged();
    void birthdayChanged();
    void emailChanged();
    void modeChanged();

private:
    // The accessor methods of the properties
    void setFirstName(const QString &firstName);
    QString firstName() const;

    void setLastName(const QString &lastName);
    QString lastName() const;

    void setBirthday(const QDateTime &birthday);
    QDateTime birthday() const;

    void setEmail(const QString &email);
    QString email() const;

    // The central object to access the contact service
    bb::pim::contacts::ContactService *m_contactService;

    // The ID of the currently loaded contact (if in 'edit' mode)
    bb::pim::contacts::ContactId m_contactId;

    // The property values
    QString m_firstName;
    QString m_lastName;
    QDateTime m_birthday;
    QString m_email;

    Mode m_mode;
};
//! [0]

#endif
