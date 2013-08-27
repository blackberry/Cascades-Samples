/*
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

#include "PushContentController.hpp"

#include "vo/Push.hpp"

#include <bb/cascades/Image>

#include <QDebug>

PushContentController::PushContentController(QObject *parent)
    : QObject(parent)
    , m_contentType("plainText")
{
}

void PushContentController::setCurrentPush(const Push &push)
{
    // Fill the property values with the data from the passed Push object
    m_pushDateTime = QString::fromLatin1("%1 - %2").arg(push.pushDateAsString()).arg(push.pushTime());

    if (push.contentType() == CONTENT_TYPE_TEXT || push.fileExtension() == FILE_EXTENSION_XML) {
        m_contentType = QLatin1String("plainText");
        m_textContent = QString::fromUtf8(push.content().data());
    } else if (push.contentType() == CONTENT_TYPE_IMAGE) {
        m_contentType = QLatin1String("image");
        m_imageContent = QVariant::fromValue(bb::cascades::Image(push.content()));
    } else if (push.fileExtension() == FILE_EXTENSION_HTML) {
        m_contentType = QLatin1String("richText");
        m_textContent = QString::fromUtf8(push.content().data());
    }

    // Emit the change notification signal to let the UI update itself
    emit pushContentChanged();
}

QString PushContentController::pushDateTime() const
{
    return m_pushDateTime;
}

QString PushContentController::contentType() const
{
    return m_contentType;
}

QString PushContentController::textContent() const
{
    return m_textContent;
}

QVariant PushContentController::imageContent() const
{
    return m_imageContent;
}
