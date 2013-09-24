/*!
* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "Push.hpp"

#include <QDateTime>
#include <QDebug>

#define DATE_FORMAT "ddd, MMM d, yyyy"

Push::Push()
    : m_seqNum(0)
    , m_unread(true)
{
}

Push::Push(const bb::network::PushPayload &pushPayload)
    : m_seqNum(0)
    , m_unread(true)
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    const QString contentTypeHeaderVal(pushPayload.headers().value("Content-Type").toString());

    setContentType(pushContentType(contentTypeHeaderVal));
    setFileExtension(pushContentFileExtension(contentTypeHeaderVal));

    setPushDate(currentDateTime.date());

    QString currentTime(currentDateTime.time().toString("h:mma"));
    currentTime.truncate(currentTime.length()-1);
    setPushTime(currentTime);

    setContent(pushPayload.data());
}

Push::Push(const QVariantMap &map)
{
    setSeqNum(map["seqnum"].toInt());
    setContentType(map["type"].toString());
    setFileExtension(map["extension"].toString());
    setPushDate(map["pushdate"].toDate());
    setPushTime(map["pushtime"].toString());
    setContent(map["content"].toByteArray());
    setUnread(map["unread"].toBool());
}

Push::Push(int seqNum, const QByteArray &content, const QString &contentType,
           const QString &fileExtension, const QString &pushDate, const QString &pushTime, bool unread)
{
    setSeqNum(seqNum);
    setContentType(contentType);
    setFileExtension(fileExtension);
    setPushDate(QDate::fromString(pushDate, DATE_FORMAT));
    setPushTime(pushTime);
    setContent(content);
    setUnread(unread);
}

Push::~Push()
{
}

int Push::seqNum() const
{
    return m_seqNum;
}

QByteArray Push::content() const
{
    return m_content;
}

QString Push::contentType() const
{
    return m_contentType;
}

QString Push::fileExtension() const
{
    return m_fileExtension;
}

QDate Push::pushDate() const
{
    return m_pushDate;
}

QString Push::pushDateAsString() const
{
    return m_pushDate.toString(DATE_FORMAT);
}

QString Push::pushTime() const
{
    return m_pushTime;
}

bool Push::unread() const
{
    return m_unread;
}

void Push::setSeqNum(int seqNum)
{
    m_seqNum = seqNum;
}

void Push::setContent(const QByteArray &content)
{
    m_content = content;
}

void Push::setContentType(const QString &contentType)
{
    m_contentType = contentType;
}

void Push::setFileExtension(const QString &fileExtension)
{
    m_fileExtension = fileExtension;
}

void Push::setPushDate(const QDate& pushDate)
{
    m_pushDate = pushDate;
}

void Push::setPushTime(const QString &pushTime)
{
    m_pushTime = pushTime;
}

void Push::setUnread(bool unread)
{
    m_unread = unread;
}

QVariantMap Push::toMap() const
{
    QVariantMap map;
    map["seqnum"] = seqNum();
    map["pushdate"] = pushDate();
    map["type"] = contentType();
    map["pushtime"] = pushTime();
    map["extension"] = fileExtension();
    map["content"] = content();
    map["unread"] = unread();

    return map;
}

QString Push::pushContentType(const QString &contentTypeHeaderValue) const
{
    if (contentTypeHeaderValue.indexOf("image") >= 0) {
        return CONTENT_TYPE_IMAGE;
    } else if (contentTypeHeaderValue.startsWith("text/html") || contentTypeHeaderValue.startsWith("application/xml")) {
        return CONTENT_TYPE_XML;
    } else {
        return CONTENT_TYPE_TEXT;
    }
}

QString Push::pushContentFileExtension(const QString &contentTypeHeaderValue) const
{
    if (contentTypeHeaderValue.startsWith("application/xml")) {
        return FILE_EXTENSION_XML;
    } else if (contentTypeHeaderValue.startsWith("text/html")) {
        return FILE_EXTENSION_HTML;
    } else if (contentTypeHeaderValue.startsWith("image/jpeg")) {
        return FILE_EXTENSION_JPEG;
    } else if (contentTypeHeaderValue.startsWith("image/gif")) {
        return FILE_EXTENSION_GIF;
    } else if (contentTypeHeaderValue.startsWith("image/png")) {
        return FILE_EXTENSION_PNG;
    } else if (contentTypeHeaderValue.startsWith("text/plain")) {
        return FILE_EXTENSION_TEXT;
    } else {
        qWarning() << "File extension is unknown for Content-Type header value: " << contentTypeHeaderValue << ".";
        return "";
    }
}
