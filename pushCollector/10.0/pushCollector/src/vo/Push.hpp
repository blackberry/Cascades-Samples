/*!
* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef PUSH_HPP
#define PUSH_HPP

#include <bb/network/PushPayload>
#include <QString>
#include <QDate>

// The content types that Push Collector can display (images, HTML/XML, and plain text)
#define CONTENT_TYPE_IMAGE    "image"
#define CONTENT_TYPE_XML     "xml"
#define CONTENT_TYPE_TEXT      "text"

// The various file extensions that are supported
#define FILE_EXTENSION_XML  ".xml"
#define FILE_EXTENSION_HTML ".html"
#define FILE_EXTENSION_TEXT ".txt"
#define FILE_EXTENSION_JPEG ".jpg"
#define FILE_EXTENSION_GIF  ".gif"
#define FILE_EXTENSION_PNG  ".png"

/*!
 * Value object relating to a push.
 */
class Push
{
public:
    Push();
    Push(const bb::network::PushPayload &pushPayload);
    Push(const QVariantMap &map);
    Push(int seqNum, const QByteArray &content, const QString &contentType,
         const QString &fileExtension, const QString &pushDate, const QString &pushTime, bool unread);
    virtual ~Push();

    int seqNum() const;
    QByteArray content() const;
    QString contentType() const;
    QString fileExtension() const;
    QDate pushDate() const;
    QString pushDateAsString() const;
    QString pushTime() const;
    bool unread() const;

    void setSeqNum(int seqNum);
    void setContent(const QByteArray &content);
    void setContentType(const QString &contentType);
    void setFileExtension(const QString &fileExtension);
    void setPushDate(const QDate &pushDate);
    void setPushTime(const QString &pushTime);
    void setUnread(bool unread);

    QVariantMap toMap() const;

private:
    // The unique id of the push (to identify it in the database)
    int m_seqNum;

    // The content/payload of the push as a base64-encoded string
    QByteArray m_content;

    // The content type (i.e. one of "image", "xml", "text")
    QString m_contentType;

    // The file extension associated with the content
    // (i.e. one of ".xml", ".html", ".txt", ".jpg", ".gif", ".png")
    QString m_fileExtension;

    // The date of the push (e.g. Mon, Oct 31, 2011)
    QDate m_pushDate;

    // The time of the push using a 12-hour clock (e.g. 2:38p, e.g. 11:22a)
    QString m_pushTime;

    // Whether or not the push has been previously read/opened
    bool m_unread;

    /*!
     * Retrieves the content type of a push. If the content type header was missing or not recognized a content type of 'text' will be assumed. It
     * is a best practice to always send a Content-Type header with the push message.
     *
     * @param contentTypeHeaderValue the value of the Content-Type header
     * @return the content type based on the Content-Type header value
     */
    QString pushContentType(const QString &contentTypeHeaderValue) const;

    /*!
     * Retrieves the file extension of a push. If the content type header was missing or not recognized an empty string will be returned. It
     * is a best practice to always send a Content-Type header with the push message.
     *
     * @param contentTypeHeaderValue the value of the Content-Type header
     * @return the file extension based on the Content-Type header value
     */
    QString pushContentFileExtension(const QString &contentTypeHeaderValue) const;
};

#endif
