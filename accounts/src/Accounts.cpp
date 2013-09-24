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

#include "Accounts.hpp"

#include "AccountEditor.hpp"
#include "AccountViewer.hpp"

#include <bb/pim/account/Provider>
#include <bb/pim/account/Result>

using namespace bb::cascades;
using namespace bb::pim::account;

//! [0]
Accounts::Accounts(QObject *parent)
    : QObject(parent)
    , m_accountService(new AccountService())
    , m_model(new GroupDataModel(this))
    , m_filter("Calendars")
    , m_accountEditor(new AccountEditor(m_accountService, this))
    , m_accountViewer(new AccountViewer(m_accountService, this))
    , m_currentAccountId(-1)
{
    // Disable grouping in data model
    m_model->setGrouping(ItemGrouping::None);

    // Ensure to invoke the filterAccounts() method whenever an account has been added, changed or removed
    bool ok = connect(m_accountService, SIGNAL(accountsChanged(bb::pim::account::AccountsChanged)), SLOT(filterAccounts()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    // Fill the data model with accounts initially
    filterAccounts();
}
//! [0]

//! [1]
void Accounts::setCurrentAccount(const QVariantList &indexPath)
{
    // Extract the ID of the selected account from the model
    if (indexPath.isEmpty()) {
        m_currentAccountId = -1;
    } else {
        const QVariantMap entry = m_model->data(indexPath).toMap();
        m_currentAccountId = entry.value("accountId").toLongLong();
    }
}
//! [1]

//! [2]
void Accounts::createAccount()
{
    // Prepare the account editor for creating a new account
    m_accountEditor->reset();
    m_accountEditor->setMode(AccountEditor::CreateMode);
}
//! [2]

//! [3]
void Accounts::editAccount()
{
    // Prepare the account editor for editing the current account
    m_accountEditor->loadAccount(m_currentAccountId);
    m_accountEditor->setMode(AccountEditor::EditMode);
}
//! [3]

//! [4]
void Accounts::viewAccount()
{
    // Prepare the account viewer for displaying the current account
    m_accountViewer->setAccountId(m_currentAccountId);
}
//! [4]

//! [5]
void Accounts::deleteAccount()
{
    m_accountService->deleteAccount(m_currentAccountId);
}
//! [5]

bb::cascades::GroupDataModel* Accounts::model() const
{
    return m_model;
}

QString Accounts::filter() const
{
    return m_filter;
}

//! [6]
void Accounts::setFilter(const QString &filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    emit filterChanged();

    // Update the model now that the filter criterion has changed
    filterAccounts();
}
//! [6]

AccountEditor* Accounts::accountEditor() const
{
    return m_accountEditor;
}

AccountViewer* Accounts::accountViewer() const
{
    return m_accountViewer;
}

//! [7]
void Accounts::filterAccounts()
{
    static QHash<QString, Service::Type> serviceTypes;
    if (serviceTypes.isEmpty()) {
        serviceTypes.insert(QLatin1String("Calendars"), Service::Calendars);
        serviceTypes.insert(QLatin1String("Contacts"), Service::Contacts);
        serviceTypes.insert(QLatin1String("Notebook"), Service::Notebook);
        serviceTypes.insert(QLatin1String("Geolocations"), Service::Geolocations);
        serviceTypes.insert(QLatin1String("Linking"), Service::Linking);
        serviceTypes.insert(QLatin1String("Memos"), Service::Memos);
        serviceTypes.insert(QLatin1String("Messages"), Service::Messages);
        serviceTypes.insert(QLatin1String("Tags"), Service::Tags);
        serviceTypes.insert(QLatin1String("Tasks"), Service::Tasks);
        serviceTypes.insert(QLatin1String("Phone"), Service::Phone);
    }

    const QList<Account> accounts = m_accountService->accounts(serviceTypes.value(m_filter));

    // Clear the old account information from the model
    m_model->clear();

    // Iterate over the list of accounts
    foreach (const Account &account, accounts) {
        // Copy the data into a model entry
        QVariantMap entry;
        entry["accountId"] = account.id();
        entry["provider"] = account.provider().name();
        entry["displayName"] = account.displayName();

        // Add the entry to the model
        m_model->insert(entry);
    }
}
//! [7]
