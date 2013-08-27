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

#include "MessageViewer.hpp"

#include <bb/pim/account/Account>
#include <bb/pim/message/Message>

using namespace bb::pim::account;
using namespace bb::pim::message;

//! [0]
MessageViewer::MessageViewer(MessageService *service, QObject *parent)
    : QObject(parent)
    , m_messageService(service)
    , m_messageId(-1)
    , m_accountId(-1)
{
    // Ensure to invoke the messageUpdated() method whenever a message has been changed
    bool ok = connect(m_messageService, SIGNAL(messageUpdated(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey, bb::pim::message::MessageUpdate)),
                      SLOT(messageUpdated(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey, bb::pim::message::MessageUpdate)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]

//! [1]
void MessageViewer::updateMessage()
{
    // Store previous values
    const QString oldSubject = m_subject;
    const QString oldSender = m_sender;
    const QDateTime oldTime = m_time;
    const QString oldBody = m_body;

    // Fetch new values from persistent storage
    const Message message = m_messageService->message(m_accountId, m_messageId);

    m_subject = message.subject();
    m_sender = message.sender().displayableName();
    m_time = message.serverTimestamp();
    m_body = message.body(MessageBody::PlainText).plainText();
    if (m_body.isEmpty())
        m_body = message.body(MessageBody::Html).plainText();

    // Check whether values have changed
    if (oldSubject != m_subject)
        emit subjectChanged();

    if (oldSender != m_sender)
        emit senderChanged();

    if (oldTime != m_time)
        emit timeChanged();

    if (oldBody != m_body)
        emit bodyChanged();
}
//! [1]

//! [2]
void MessageViewer::messageUpdated(bb::pim::account::AccountKey accountId, bb::pim::message::ConversationKey, bb::pim::message::MessageKey messageId, const bb::pim::message::MessageUpdate&)
{
    /**
     * Call updateMessage() only if the message we are currently displaying
     * has been changed.
     */
    if (m_accountId == accountId && m_messageId == messageId)
        updateMessage();
}
//! [2]

//! [3]
void MessageViewer::setMessage(AccountKey accountId, MessageKey messageId)
{
    if (m_accountId != accountId || m_messageId != messageId) {
        m_accountId = accountId;
        m_messageId = messageId;

        // Trigger a refetch of the message for the new ID
        updateMessage();
    }
}
//! [3]

QString MessageViewer::subject() const
{
    return m_subject;
}

QString MessageViewer::senderContact() const
{
    return m_sender;
}

QString MessageViewer::time() const
{
    if (!m_time.isValid())
        return tr("n/a");
    else
        return m_time.toString();
}

QString MessageViewer::body() const
{
    return m_body;
}
