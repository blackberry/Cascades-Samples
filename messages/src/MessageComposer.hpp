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

#ifndef MESSAGECOMPOSER_HPP
#define MESSAGECOMPOSER_HPP

#include <bb/pim/message/MessageService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the message composer
 *
 * The MessageComposer class is an UI-independent representation of the message composer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of creating a new message or replying to an existing one.
 */
//! [0]
class MessageComposer : public QObject
{
    Q_OBJECT

    // The data properties of the message that is displayed
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString recipient READ recipient WRITE setRecipient NOTIFY recipientChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)

    // Defines whether the composer is in 'create' or 'reply' mode
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_ENUMS(Mode)

public:
    /**
     * Describes the mode of the message composer.
     * The mode information are used to adapt the behavior of the composer and
     * provide hints to the UI.
     */
    enum Mode {
        CreateMode,
        ReplyMode
    };

    MessageComposer(bb::pim::message::MessageService *service, QObject *parent = 0);

    // Sets the ID of the current account.
    void setAccountId(bb::pim::account::AccountKey accountId);

    void setMode(Mode mode);
    Mode mode() const;

public Q_SLOTS:
    /**
     * Loads the message with the given ID.
     */
    void loadMessage(bb::pim::message::MessageKey messageId);

    /**
     * Composes and sends the reply of the currently loaded message if in 'reply' mode or composes and sends
     * a new message if in 'create' mode.
     */
    void composeMessage();

    /**
     * Resets all fields of the message composer.
     */
    void reset();

Q_SIGNALS:
    // The change notification signals of the properties
    void subjectChanged();
    void recipientChanged();
    void bodyChanged();
    void modeChanged();

private:
    // The accessor methods of the properties
    void setSubject(const QString &subject);
    QString subject() const;
    void setRecipient(const QString &recipient);
    QString recipient() const;
    void setBody(const QString &body);
    QString body() const;

    // The central object to access the message service
    bb::pim::message::MessageService* m_messageService;

    // The ID of the message that is displayed
    bb::pim::message::MessageKey m_messageId;

    // The ID of the account of the message that is displayed
    bb::pim::account::AccountKey m_accountId;

    // The property values
    QString m_subject;
    QString m_recipient;
    QString m_body;

    Mode m_mode;
};
//! [0]

#endif
