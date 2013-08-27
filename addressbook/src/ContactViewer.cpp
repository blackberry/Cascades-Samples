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

#include "ContactViewer.hpp"

#include <bb/pim/contacts/Contact>

using namespace bb::pim::contacts;

//! [0]
ContactViewer::ContactViewer(ContactService *service, QObject *parent)
    : QObject(parent)
    , m_contactService(service)
    , m_contactId(-1)
{
    // Ensure to invoke the contactsChanged() method whenever a contact has been changed
    bool ok = connect(m_contactService, SIGNAL(contactsChanged(QList<int>)), SLOT(contactsChanged(QList<int>)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]

//! [1]
void ContactViewer::updateContact()
{
    // Store previous values
    const QString oldFirstName = m_firstName;
    const QString oldLastName = m_lastName;
    const QDateTime oldBirthday = m_birthday;
    const QString oldEmail = m_email;

    // Fetch new values from persistent storage
    const Contact contact = m_contactService->contactDetails(m_contactId);

    m_firstName = contact.firstName();
    m_lastName = contact.lastName();

    m_birthday = QDateTime();
    const QList<ContactAttribute> dateAttributes = contact.filteredAttributes(AttributeKind::Date);
    foreach (const ContactAttribute &dateAttribute, dateAttributes) {
        if (dateAttribute.subKind() == AttributeSubKind::DateBirthday)
            m_birthday = dateAttribute.valueAsDateTime();
    }

    m_email.clear();
    const QList<ContactAttribute> emails = contact.emails();
    if (!emails.isEmpty())
        m_email = emails.first().value();

    // Check whether values have changed
    if (oldFirstName != m_firstName)
        emit firstNameChanged();

    if (oldLastName != m_lastName)
        emit lastNameChanged();

    if (oldBirthday != m_birthday)
        emit birthdayChanged();

    if (oldEmail != m_email)
        emit emailChanged();
}
//! [1]

//! [2]
void ContactViewer::contactsChanged(const QList<int> &contactIds)
{
    /**
     * Call updateContact() only if the contact we are currently displaying
     * has been changed.
     */
    if (contactIds.contains(m_contactId))
        updateContact();
}
//! [2]

//! [3]
void ContactViewer::setContactId(ContactId contactId)
{
    if (m_contactId == contactId)
        return;

    m_contactId = contactId;

    // Trigger a refetch of the contact for the new ID
    updateContact();
}
//! [3]

QString ContactViewer::firstName() const
{
    return m_firstName;
}

QString ContactViewer::lastName() const
{
    return m_lastName;
}

QDateTime ContactViewer::birthday() const
{
    return m_birthday;
}

QString ContactViewer::formattedBirthday() const
{
    if (!m_birthday.isValid())
        return tr("n/a");
    else
        return m_birthday.date().toString();
}

QString ContactViewer::email() const
{
    return m_email;
}
