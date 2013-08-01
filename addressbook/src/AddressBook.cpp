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

#include "AddressBook.hpp"

#include "ContactEditor.hpp"
#include "ContactViewer.hpp"

using namespace bb::cascades;
using namespace bb::pim::contacts;

//! [0]
AddressBook::AddressBook(QObject *parent)
    : QObject(parent)
    , m_contactService(new ContactService(this))
    , m_model(new GroupDataModel(this))
    , m_contactViewer(new ContactViewer(m_contactService, this))
    , m_contactEditor(new ContactEditor(m_contactService, this))
    , m_currentContactId(-1)
{
    // Disable grouping in data model
    m_model->setGrouping(ItemGrouping::None);

    // Ensure to invoke the filterContacts() method whenever a contact has been added, changed or removed
    bool ok = connect(m_contactService, SIGNAL(contactsAdded(QList<int>)), SLOT(filterContacts()));
    Q_ASSERT(ok);
    ok = connect(m_contactService, SIGNAL(contactsChanged(QList<int>)), SLOT(filterContacts()));
    Q_ASSERT(ok);
    ok = connect(m_contactService, SIGNAL(contactsDeleted(QList<int>)), SLOT(filterContacts()));
    Q_ASSERT(ok);

    // Fill the data model with contacts initially
    filterContacts();
}
//! [0]

//! [1]
void AddressBook::setCurrentContact(const QVariantList &indexPath)
{
    // Extract the ID of the selected contact from the model
    if (indexPath.isEmpty()) {
        m_currentContactId = -1;
    } else {
        const QVariantMap entry = m_model->data(indexPath).toMap();
        m_currentContactId = entry.value("contactId").toInt();
    }
}
//! [1]

//! [2]
void AddressBook::createContact()
{
    // Prepare the contact editor for creating a new contact
    m_contactEditor->reset();
    m_contactEditor->setMode(ContactEditor::CreateMode);
}
//! [2]

//! [3]
void AddressBook::editContact()
{
    // Prepare the contact editor for editing the current contact
    m_contactEditor->loadContact(m_currentContactId);
    m_contactEditor->setMode(ContactEditor::EditMode);
}
//! [3]

//! [4]
void AddressBook::viewContact()
{
    // Prepare the contact viewer for displaying the current contact
    m_contactViewer->setContactId(m_currentContactId);
}
//! [4]

//! [5]
void AddressBook::deleteContact()
{
    m_contactService->deleteContact(m_currentContactId);
}
//! [5]

bb::cascades::GroupDataModel* AddressBook::model() const
{
    return m_model;
}

QString AddressBook::filter() const
{
    return m_filter;
}

//! [6]
void AddressBook::setFilter(const QString &filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    emit filterChanged();

    // Update the model now that the filter criterion has changed
    filterContacts();
}
//! [6]

ContactViewer* AddressBook::contactViewer() const
{
    return m_contactViewer;
}

ContactEditor* AddressBook::contactEditor() const
{
    return m_contactEditor;
}

//! [7]
void AddressBook::filterContacts()
{
    QList<Contact> contacts;

    if (m_filter.isEmpty()) {
        // No filter has been specified, so just list all contacts
        ContactListFilters filter;
        contacts = m_contactService->contacts(filter);
    } else {
        // Use the entered filter string as search value
        ContactSearchFilters filter;
        filter.setSearchValue(m_filter);
        contacts = m_contactService->searchContacts(filter);
    }

    // Clear the old contact information from the model
    m_model->clear();

    // Iterate over the list of contact IDs
    foreach (const Contact &idContact, contacts) {
        // Fetch the complete details for this contact ID
        const Contact contact = m_contactService->contactDetails(idContact.id());

        // Copy the data into a model entry
        QVariantMap entry;
        entry["contactId"] = contact.id();
        entry["firstName"] = contact.firstName();
        entry["lastName"] = contact.lastName();

        const QList<ContactAttribute> emails = contact.emails();
        if (!emails.isEmpty())
            entry["email"] = emails.first().value();

        // Add the entry to the model
        m_model->insert(entry);
    }
}
//! [7]
