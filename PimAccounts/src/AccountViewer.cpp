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

#include "AccountViewer.hpp"

#include <bb/cascades/QmlDocument>
#include <bb/pim/account/Account>
#include <bb/pim/account/Provider>

using namespace bb::cascades;
using namespace bb::pim::account;

static QString valueToString(const QVariant &value)
{
    switch (value.type()) {
        case QVariant::String:
        case QVariant::Int:
            return value.toString();
        case QVariant::DateTime:
            return value.toDateTime().toString();
        case QVariant::Bool:
            return (value.toBool() ? QObject::tr("true") : QObject::tr("false"));
        default:
            qDebug() << "unhandled type:" << value.type();
            return QString();
    }
}

//! [0]
AccountViewer::AccountViewer(AccountService *service, QObject *parent)
    : QObject(parent)
    , m_accountService(service)
    , m_accountId(-1)
    , m_fields(new QMapListDataModel())
{
    m_fields->setParent(this);

    // Ensure to invoke the accountsChanged() method whenever an account has been changed
    connect(m_accountService, SIGNAL(accountsChanged(bb::pim::account::AccountsChanged)), SLOT(accountsChanged(bb::pim::account::AccountsChanged)));
}
//! [0]

//! [1]
void AccountViewer::updateAccount()
{
    // Clear all field descriptions from the model
    m_fields->clear();

    // Fetch new values from persistent storage
    const Account account = m_accountService->account(m_accountId);

    foreach (const QString &key, account.provider().settingsKeys()) {
        const QVariant value = account.settingsProperty(key);

        // Add a new description to the model
        QVariantMap entry;
        entry["title"] = key;
        entry["value"] = valueToString(value);

        m_fields->append(entry);
    }
}
//! [1]

//! [2]
void AccountViewer::accountsChanged(const AccountsChanged &changed)
{
    /**
     * Call updateAccount() only if the account we are currently displaying
     * has been changed.
     */
    if (changed.updatedAccountIds().contains(m_accountId))
        updateAccount();
}
//! [2]

//! [3]
void AccountViewer::setAccountId(AccountKey accountId)
{
    m_accountId = accountId;

    // Trigger a refetch of the account for the new ID
    updateAccount();
}
//! [3]

bb::cascades::DataModel* AccountViewer::fields() const
{
    return m_fields;
}
