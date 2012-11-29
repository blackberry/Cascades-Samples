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

#ifndef ACCOUNTEDITOR_HPP
#define ACCOUNTEDITOR_HPP

#include <bb/cascades/Container>
#include <bb/pim/account/AccountService>

#include <QtCore/QObject>
#include <QtCore/QPointer>

/**
 * @short A controller object to encapsulate the logic of the account editor
 *
 * The AccountEditor class is an UI-independent representation of the account editor,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of creating a new account or updating an existing
 * one.
 */
//! [0]
class AccountEditor : public QObject
{
    Q_OBJECT

    // The ID of the provider the account belongs to
    Q_PROPERTY(QString providerId READ providerId WRITE setProviderId NOTIFY providerIdChanged)

    // The container where all the editor fields are located in
    Q_PROPERTY(bb::cascades::Container* form READ form WRITE setForm NOTIFY formChanged)

    // Defines whether the editor is in 'create' or 'edit' mode
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_ENUMS(Mode)

public:
    /**
     * Describes the mode of the account editor.
     * The mode information are used to adapt the behavior of the editor and
     * provide hints to the UI.
     */
    enum Mode {
        CreateMode,
        EditMode
    };

    AccountEditor(bb::pim::account::AccountService *service, QObject *parent = 0);

    void setMode(Mode mode);
    Mode mode() const;

public Q_SLOTS:
    /**
     * Loads the account with the given ID.
     */
    void loadAccount(bb::pim::account::AccountKey accountId);

    /**
     * Save the currently loaded account if in 'edit' mode or creates a new one
     * if in 'create' mode.
     */
    void saveAccount();

    /**
     * Resets all fields of the account editor.
     */
    void reset();

Q_SIGNALS:
    // The change notification signals of the properties
    void providerIdChanged();
    void formChanged();
    void modeChanged();

private:
    // The accessor methods of the properties
    QString providerId() const;
    void setProviderId(const QString &providerId);
    bb::cascades::Container* form() const;
    void setForm(bb::cascades::Container *form);

    // Loads the account data from persistent storage into fields
    void loadAccount();

    // Updates the fields according to the configured provider
    void updateFields();

    // The central object to access the account service
    bb::pim::account::AccountService *m_accountService;

    // The ID of the currently loaded account (if in 'edit' mode)
    bb::pim::account::AccountKey m_accountId;

    // The property values
    QString m_providerId;
    QPointer<bb::cascades::Container> m_form;

    Mode m_mode;

    QHash<QString, bb::cascades::Control*> m_fields;
};
//! [0]

#endif
