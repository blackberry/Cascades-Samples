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

#ifndef MESSAGEVIEWER_HPP
#define MESSAGEVIEWER_HPP

#include <bb/pim/message/MessageService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the message viewer
 *
 * The MessageViewer class is an UI-independent representation of the message viewer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of loading a message from the persistent storage,
 * provide its data as properties and update the properties if the message has
 * changed in the storage backend.
 */
//! [0]
class MessageViewer : public QObject
{
    Q_OBJECT

    // The data properties of the message that is displayed
    Q_PROPERTY(QString subject READ subject NOTIFY subjectChanged)
    Q_PROPERTY(QString sender READ senderContact NOTIFY senderChanged)
    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
    Q_PROPERTY(QString body READ body NOTIFY bodyChanged)

public:
    MessageViewer(bb::pim::message::MessageService *service, QObject *parent = 0);

    // Sets the ID of the message that should be displayed.
    void setMessage(bb::pim::account::AccountKey accountId, bb::pim::message::MessageKey messageId);

Q_SIGNALS:
    // The change notification signals of the properties
    void subjectChanged();
    void senderChanged();
    void timeChanged();
    void bodyChanged();

private Q_SLOTS:
    /**
     * This slot is invoked whenever the message service reports that a message has been changed.
     */
    void messageUpdated(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey, const bb::pim::message::MessageUpdate&);

private:
    // The accessor methods of the properties
    QString subject() const;
    QString senderContact() const;
    QString time() const;
    QString body() const;

    // Loads the message from the persistent storage and updates the properties
    void updateMessage();

    // The central object to access the message service
    bb::pim::message::MessageService* m_messageService;

    // The ID of the message that is displayed
    bb::pim::message::MessageKey m_messageId;

    // The ID of the account of the message that is displayed
    bb::pim::account::AccountKey m_accountId;

    // The property values
    QString m_subject;
    QString m_sender;
    QDateTime m_time;
    QString m_body;
};
//! [0]

#endif
