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

#include "MessageComposer.hpp"

#include <bb/pim/account/Account>
#include <bb/pim/message/Message>
#include <bb/pim/message/MessageBuilder>

using namespace bb::pim::account;
using namespace bb::pim::message;

//! [1]
MessageComposer::MessageComposer(MessageService *service, QObject *parent)
    : QObject(parent)
    , m_messageService(service)
    , m_messageId(-1)
    , m_accountId(-1)
    , m_mode(CreateMode)
{
}
//! [1]

void MessageComposer::setAccountId(AccountKey accountId)
{
    m_accountId = accountId;
}

//! [2]
void MessageComposer::loadMessage(MessageKey messageId)
{
    Q_ASSERT(m_accountId != -1);

    m_messageId = messageId;

    // Fetch new values from persistent storage
    const Message message = m_messageService->message(m_accountId, m_messageId);

    m_subject = message.subject();
    m_recipient = message.sender().address();
    m_body = message.body(MessageBody::PlainText).plainText();
    if (m_body.isEmpty())
        m_body = message.body(MessageBody::Html).plainText();

    // Adapt message to be a reply
    m_subject = QString::fromLatin1("Re: %1").arg(m_subject);
    m_body.replace("\n", "\n> ");
    m_body = QString::fromLatin1("> %1").arg(m_body);

    emit subjectChanged();
    emit recipientChanged();
    emit bodyChanged();
}
//! [2]

//! [3]
void MessageComposer::composeMessage()
{
    const MessageContact recipient = MessageContact(-1, MessageContact::To, QString(), m_recipient);
    const QByteArray bodyData = m_body.toUtf8();

    // Create a message builder to create/modify the message
    MessageBuilder *builder = (m_mode == CreateMode ? MessageBuilder::create(m_accountId)
                                                    : MessageBuilder::create(m_accountId, m_messageService->message(m_accountId, m_messageId)));

    builder->subject(m_subject);
    builder->removeAllRecipients();
    builder->addRecipient(recipient);
    builder->body(MessageBody::PlainText, bodyData);

    // Send the new message via current account
    m_messageService->send(m_accountId, *builder);
    delete builder;
}
//! [3]

//! [4]
void MessageComposer::reset()
{
    // Reset all properties
    m_accountId = -1;
    m_messageId = -1;

    m_subject.clear();
    m_recipient.clear();
    m_body.clear();

    // Emit the change notifications
    emit subjectChanged();
    emit recipientChanged();
    emit bodyChanged();
}
//! [4]

void MessageComposer::setSubject(const QString &subject)
{
    if (m_subject == subject)
        return;

    m_subject = subject;
    emit subjectChanged();
}

QString MessageComposer::subject() const
{
    return m_subject;
}

void MessageComposer::setRecipient(const QString &recipient)
{
    if (m_recipient == recipient)
        return;

    m_recipient = recipient;
    emit recipientChanged();
}

QString MessageComposer::recipient() const
{
    return m_recipient;
}

void MessageComposer::setBody(const QString &body)
{
    if (m_body == body)
        return;

    m_body = body;
    emit bodyChanged();
}

QString MessageComposer::body() const
{
    return m_body;
}

void MessageComposer::setMode(Mode mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged();
}

MessageComposer::Mode MessageComposer::mode() const
{
    return m_mode;
}
