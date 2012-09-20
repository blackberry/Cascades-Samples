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

#ifndef ACCOUNTVIEWER_HPP
#define ACCOUNTVIEWER_HPP

#include <bb/cascades/QListDataModel>
#include <bb/pim/account/AccountsChanged>
#include <bb/pim/account/AccountService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the account viewer
 *
 * The AccountViewer class is an UI-independent representation of the account viewer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of loading an account from the persistent storage,
 * provide its data as properties and update the properties if the account has
 * changed in the storage backend.
 */
//! [0]
class AccountViewer : public QObject
{
    Q_OBJECT

    // The abstract description of the viewer fields
    Q_PROPERTY(bb::cascades::DataModel* fields READ fields CONSTANT)

public:
    AccountViewer(bb::pim::account::AccountService *service, QObject *parent = 0);

    // Sets the ID of the account that should be displayed.
    void setAccountId(bb::pim::account::AccountKey accountId);

private Q_SLOTS:
    /**
     * This slot is invoked whenever the account service reports that an account has been changed.
     */
    void accountsChanged(const bb::pim::account::AccountsChanged &changed);

private:
    // The accessor methods of the property
    bb::cascades::DataModel* fields() const;

    // Loads the account from the persistent storage and updates the properties
    void updateAccount();

    // The central object to access the account service
    bb::pim::account::AccountService* m_accountService;

    // The ID of the account that is displayed
    bb::pim::account::AccountKey m_accountId;

    // The property value
    bb::cascades::QMapListDataModel* m_fields;
};
//! [0]

#endif
