/* Copyright (c) 2012 Research In Motion Limited.
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

#include "ContactEditor.hpp"

#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactAttributeBuilder>
#include <bb/pim/contacts/ContactBuilder>

using namespace bb::pim::contacts;

//! [0]
/**
 * A helper method to update a single attribute on a Contact object.
 * It first deletes the old attribute (if it exists) and adds the attribute with the
 * new value afterwards.
 */
template <typename T>
static void updateContactAttribute(ContactBuilder &builder, const Contact &contact,
                                        AttributeKind::Type kind, AttributeSubKind::Type subKind,
                                        const T &value)
{
    // Delete previous instance of the attribute
    QList<ContactAttribute> attributes = contact.filteredAttributes(kind);
    foreach (const ContactAttribute &attribute, attributes) {
        if (attribute.subKind() == subKind)
            builder.deleteAttribute(attribute);
    }

    // Add new instance of the attribute with new value
    builder.addAttribute(ContactAttributeBuilder()
                        .setKind(kind)
                        .setSubKind(subKind)
                        .setValue(value));
}
//! [0]

//! [1]
ContactEditor::ContactEditor(ContactService *service, QObject *parent)
    : QObject(parent)
    , m_contactService(service)
    , m_contactId(-1)
    , m_birthday(QDateTime::currentDateTime())
    , m_mode(CreateMode)
{
}
//! [1]

//! [2]
void ContactEditor::loadContact(ContactId contactId)
{
    m_contactId = contactId;

    // Load the contact from the persistent storage
    const Contact contact = m_contactService->contactDetails(m_contactId);

    // Update the properties with the data from the contact
    m_firstName = contact.firstName();
    m_lastName = contact.lastName();

    m_birthday = QDateTime::currentDateTime();
    const QList<ContactAttribute> dateAttributes = contact.filteredAttributes(AttributeKind::Date);
    foreach (const ContactAttribute &dateAttribute, dateAttributes) {
        if (dateAttribute.subKind() == AttributeSubKind::DateBirthday)
            m_birthday = dateAttribute.valueAsDateTime();
    }

    m_email.clear();
    const QList<ContactAttribute> emails = contact.emails();
    if (!emails.isEmpty())
        m_email = emails.first().value();

    // Emit the change notifications
    emit firstNameChanged();
    emit lastNameChanged();
    emit birthdayChanged();
    emit emailChanged();
}
//! [2]

//! [3]
void ContactEditor::saveContact()
{
    if (m_mode == CreateMode) {
        // Create a builder to assemble the new contact
        ContactBuilder builder;

        // Set the first name
        builder.addAttribute(ContactAttributeBuilder()
                            .setKind(AttributeKind::Name)
                            .setSubKind(AttributeSubKind::NameGiven)
                            .setValue(m_firstName));

        // Set the last name
        builder.addAttribute(ContactAttributeBuilder()
                            .setKind(AttributeKind::Name)
                            .setSubKind(AttributeSubKind::NameSurname)
                            .setValue(m_lastName));

        // Set the birthday
        builder.addAttribute(ContactAttributeBuilder()
                            .setKind(AttributeKind::Date)
                            .setSubKind(AttributeSubKind::DateBirthday)
                            .setValue(m_birthday));

        // Set the email address
        builder.addAttribute(ContactAttributeBuilder()
                            .setKind(AttributeKind::Email)
                            .setSubKind(AttributeSubKind::Other)
                            .setValue(m_email));

        // Save the contact to persistent storage
        m_contactService->createContact(builder, false);

    } else if (m_mode == EditMode) {
        // Load the contact from persistent storage
        Contact contact = m_contactService->contactDetails(m_contactId);
        if (contact.id()) {
            // Create a builder to modify the contact
            ContactBuilder builder = contact.edit();

            // Update the single attributes
            updateContactAttribute<QString>(builder, contact, AttributeKind::Name, AttributeSubKind::NameGiven, m_firstName);
            updateContactAttribute<QString>(builder, contact, AttributeKind::Name, AttributeSubKind::NameSurname, m_lastName);
            updateContactAttribute<QDateTime>(builder, contact, AttributeKind::Date, AttributeSubKind::DateBirthday, m_birthday);
            updateContactAttribute<QString>(builder, contact, AttributeKind::Email, AttributeSubKind::Other, m_email);

            // Save the updated contact back to persistent storage
            m_contactService->updateContact(builder);
        }
    }
}
//! [3]

//! [4]
void ContactEditor::reset()
{
    // Reset all properties
    m_firstName.clear();
    m_lastName.clear();
    m_birthday = QDateTime::currentDateTime();
    m_email.clear();

    // Emit the change notifications
    emit firstNameChanged();
    emit lastNameChanged();
    emit birthdayChanged();
    emit emailChanged();
}
//! [4]

void ContactEditor::setFirstName(const QString &firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged();
}

QString ContactEditor::firstName() const
{
    return m_firstName;
}

void ContactEditor::setLastName(const QString &lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged();
}

QString ContactEditor::lastName() const
{
    return m_lastName;
}

void ContactEditor::setBirthday(const QDateTime &birthday)
{
    if (m_birthday.date() == birthday.date())
        return;

    m_birthday = birthday;
    emit birthdayChanged();
}

QDateTime ContactEditor::birthday() const
{
    return m_birthday;
}

void ContactEditor::setEmail(const QString &email)
{
    if (m_email == email)
        return;

    m_email = email;
    emit emailChanged();
}

QString ContactEditor::email() const
{
    return m_email;
}

void ContactEditor::setMode(Mode mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged();
}

ContactEditor::Mode ContactEditor::mode() const
{
    return m_mode;
}
