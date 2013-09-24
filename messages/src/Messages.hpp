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

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/pim/account/Account>
#include <bb/pim/message/MessageService>

#include <QtCore/QObject>

class MessageComposer;
class MessageViewer;

namespace bb {
namespace cascades {
class Option;
}
}

/**
 * @short The controller class that makes access to messages available to the UI.
 */
//! [0]
class Messages : public QObject
{
    Q_OBJECT

    // The model that provides the filtered list of messages
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

    // The pattern to filter the list of messages
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged);

    // The viewer object for the current message
    Q_PROPERTY(MessageViewer* messageViewer READ messageViewer CONSTANT);

    // The composer object for the current message
    Q_PROPERTY(MessageComposer* messageComposer READ messageComposer CONSTANT);

public:
    Messages(QObject *parent = 0);

    /**
     * A helper method to fill the DropDown with available accounts.
     */
    Q_INVOKABLE void addAccounts(QObject* dropDownObject) const;

public Q_SLOTS:
    /**
     * Marks the message with the given @p indexPath as current.
     */
    void setCurrentMessage(const QVariantList &indexPath);

    /**
     * Selects the account to work with.
     */
    void setSelectedAccount(bb::cascades::Option *selectedOption);

    /**
     * Prepares the message composer to compose a new message.
     */
    void composeMessage();

    /**
     * Prepares the message composer to compose a reply to the current message.
     */
    void composeReplyMessage();

    /**
     * Prepares the message viewer to display the current message.
     */
    void viewMessage();

    /**
     * Deletes the current message.
     */
    void deleteMessage();

Q_SIGNALS:
    // The change notification signal for the property
    void filterChanged();

private Q_SLOTS:
    // Filters the messages in the model according to the filter property
    void filterMessages();

private:
    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    QString filter() const;
    void setFilter(const QString &filter);
    MessageViewer* messageViewer() const;
    MessageComposer* messageComposer() const;

    // The central object to access the message service
    bb::pim::message::MessageService* m_messageService;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_filter;

    // The controller object for viewing a message
    MessageViewer* m_messageViewer;

    // The controller object for composing a message
    MessageComposer* m_messageComposer;

    // The ID of the current message
    bb::pim::message::MessageKey m_currentMessageId;

    // The current account
    bb::pim::account::Account m_currentAccount;

    QList<bb::pim::account::Account> m_accountList;
};
//! [0]

#endif
