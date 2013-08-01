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

#ifndef ADDRESSBOOK_HPP
#define ADDRESSBOOK_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/pim/contacts/ContactService>

#include <QtCore/QObject>

class ContactEditor;
class ContactViewer;

/**
 * @short The controller class that makes access to contacts available to the UI.
 */
//! [0]
class AddressBook : public QObject
{
    Q_OBJECT

    // The model that provides the filtered list of contacts
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

    // The pattern to filter the list of contacts
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged);

    // The viewer object for the current contact
    Q_PROPERTY(ContactViewer* contactViewer READ contactViewer CONSTANT);

    // The editor object for the current contact
    Q_PROPERTY(ContactEditor* contactEditor READ contactEditor CONSTANT);

public:
    AddressBook(QObject *parent = 0);

public Q_SLOTS:
    /**
     * Marks the contact with the given @p indexPath as current.
     */
    void setCurrentContact(const QVariantList &indexPath);

    /**
     * Prepares the contact editor to create a new contact.
     */
    void createContact();

    /**
     * Prepares the contact editor to edit the current contact.
     */
    void editContact();

    /**
     * Prepares the contact viewer to display the current contact.
     */
    void viewContact();

    /**
     * Deletes the current contact.
     */
    void deleteContact();

Q_SIGNALS:
    // The change notification signal for the property
    void filterChanged();

private Q_SLOTS:
    // Filters the contacts in the model according to the filter property
    void filterContacts();

private:
    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    QString filter() const;
    void setFilter(const QString &filter);
    ContactViewer* contactViewer() const;
    ContactEditor* contactEditor() const;

    // The central object to access the contacts service
    bb::pim::contacts::ContactService* m_contactService;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_filter;

    // The controller object for viewing a contact
    ContactViewer* m_contactViewer;

    // The controller object for editing a contact
    ContactEditor* m_contactEditor;

    // The ID of the current contact
    bb::pim::contacts::ContactId m_currentContactId;
};
//! [0]

#endif
