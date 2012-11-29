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

#include "AccountEditor.hpp"

#include <bb/cascades/QmlDocument>
#include <bb/pim/account/Account>
#include <bb/pim/account/Provider>
#include <bb/pim/account/Result>

using namespace bb::cascades;
using namespace bb::pim::account;

//! [1]
AccountEditor::AccountEditor(AccountService *service, QObject *parent)
    : QObject(parent)
    , m_accountService(service)
    , m_accountId(-1)
    , m_providerId("caldav")
    , m_mode(CreateMode)
{
}
//! [1]

//! [2]
void AccountEditor::loadAccount(AccountKey accountId)
{
    m_accountId = accountId;

    updateFields();
    loadAccount();
}
//! [2]

//! [3]
void AccountEditor::saveAccount()
{
    if (m_mode == CreateMode) {
        const Provider provider = m_accountService->provider(m_providerId);

        Account account(provider);

        // Iterate over all settings keys of the provider
        foreach (const QString &key, provider.settingsKeys()) {
            // Lookup the matching field for this key and retrieve the value
            Control *field = m_fields.value(key);
            if (field) {
                const QVariant value = field->property("value");
                if (value.isValid())
                    account.setSettingsValue(key, value);
            }
        }

        m_accountService->createAccount(provider.id(), account);

    } else if (m_mode == EditMode) {
        Account account = m_accountService->account(m_accountId);

        // Iterate over all settings keys of the provider
        foreach (const QString &key, account.provider().settingsKeys()) {
            // Lookup the matching field for this key and retrieve the value
            Control *field = m_fields.value(key);
            if (field)
                account.setSettingsValue(key, field->property("value"));
            else
                account.setSettingsValue(key, QVariant());
        }

        m_accountService->updateAccount(account.id(), account);
    }
}
//! [3]

//! [4]
void AccountEditor::reset()
{
    m_accountId = -1;
    m_providerId = "caldav";

    if (m_form)
        m_form->removeAll();

    m_fields.clear();
}
//! [4]

QString AccountEditor::providerId() const
{
    return m_providerId;
}

void AccountEditor::setProviderId(const QString &providerId)
{
    if (m_providerId == providerId)
        return;

    m_providerId = providerId;
    emit providerIdChanged();

    updateFields();
}

bb::cascades::Container* AccountEditor::form() const
{
    return m_form;
}

void AccountEditor::setForm(Container *form)
{
    if (m_form == form)
        return;

    m_form = form;
    emit formChanged();

    updateFields();
    loadAccount();
}

void AccountEditor::setMode(Mode mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged();
}

AccountEditor::Mode AccountEditor::mode() const
{
    return m_mode;
}

//! [5]
void AccountEditor::loadAccount()
{
    if (m_accountId == -1)
        return;

    // Fetch new values from persistent storage
    const Account account = m_accountService->account(m_accountId);

    // Iterate over all settings keys of the provider
    foreach (const QString &key, account.provider().settingsKeys()) {
        // Retrieve the value for this settings key
        const QVariant value = account.settingsProperty(key);

        // Lookup the matching field for this key and update the value
        Control *field = m_fields.value(key);
        if (field)
            field->setProperty("value", value);
    }
}
//! [5]

//! [6]
void AccountEditor::updateFields()
{
    if (m_form.isNull())
        return;

    // Clear all fields from the form
    m_form->removeAll();

    // Clear the internal mapping
    m_fields.clear();

    // Retrieve the provider of the current account or the one selected by the user in 'create' mode
    const Provider provider = (m_accountId == -1 ? m_accountService->provider(m_providerId)
                                                  : m_accountService->account(m_accountId).provider());

    // Iterate over all settings keys of the provider
    foreach (const QString &key, provider.settingsKeys()) {
        // Instantiate an editor field for this settings key
        QmlDocument *qml = QmlDocument::create("asset:///EditorField.qml").parent(this);
        Control *field = qml->createRootObject<Control>();

        // Configure the field according to the settings key
        field->setProperty("type", provider.settingsProperty(key, Property::Type));
        field->setProperty("title", key);

        // Add the field to the container
        m_form->add(field);

        // Add the field to the internal mapping
        m_fields.insert(key, field);
    }
}
//! [6]
