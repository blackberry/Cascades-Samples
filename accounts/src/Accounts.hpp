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

#ifndef ACCOUNTS_HPP
#define ACCOUNTS_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/pim/account/AccountService>

#include <QtCore/QObject>

class AccountEditor;
class AccountViewer;

/**
 * @short The controller class that makes access to accounts available to the UI.
 */
//! [0]
class Accounts : public QObject
{
    Q_OBJECT

    // The model that provides the filtered list of accounts
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

    // The pattern to filter the list of accounts
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged);

    // The editor object for the current account
    Q_PROPERTY(AccountEditor* accountEditor READ accountEditor CONSTANT);

    // The viewer object for the current account
    Q_PROPERTY(AccountViewer* accountViewer READ accountViewer CONSTANT);

public:
    Accounts(QObject *parent = 0);

public Q_SLOTS:
    /**
     * Marks the account with the given @p indexPath as current.
     */
    void setCurrentAccount(const QVariantList &indexPath);

    /**
     * Prepares the account editor to create a new account.
     */
    void createAccount();

    /**
     * Prepares the account editor to edit the current account.
     */
    void editAccount();

    /**
     * Prepares the account viewer to display the current account.
     */
    void viewAccount();

    /**
     * Deletes the current account.
     */
    void deleteAccount();

Q_SIGNALS:
    // The change notification signal for the property
    void filterChanged();

private Q_SLOTS:
    // Filters the accounts in the model according to the filter property
    void filterAccounts();

private:
    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    QString filter() const;
    void setFilter(const QString &filter);
    AccountEditor* accountEditor() const;
    AccountViewer* accountViewer() const;

    // The central object to access the account service
    bb::pim::account::AccountService* m_accountService;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_filter;

    // The controller object for editing an account
    AccountEditor* m_accountEditor;

    // The controller object for viewing an account
    AccountViewer* m_accountViewer;

    // The ID of the current account
    bb::pim::account::AccountKey m_currentAccountId;
};
//! [0]

#endif
