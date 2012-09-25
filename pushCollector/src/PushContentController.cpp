/*
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

#include "PushContentController.hpp"

#include "vo/Push.hpp"

#include <bb/cascades/Image>
#include <bb/ImageData>

#include <QtCore/QDebug>
#include <QtGui/QImage>

// Converts a QImage to ImageData so that it can be used by the ImageView control
static bb::ImageData fromQImage(const QImage &image)
{
    bb::ImageData imageData(bb::PixelFormat::RGBA_Premultiplied, image.width(), image.height());

    unsigned char *dstLine = imageData.pixels();
    for (int y = 0; y < imageData.height(); y++) {
        unsigned char * dst = dstLine;
        for (int x = 0; x < imageData.width(); x++) {
            const QRgb srcPixel = image.pixel(x, y);
            *dst++ = qRed(srcPixel);
            *dst++ = qGreen(srcPixel);
            *dst++ = qBlue(srcPixel);
            *dst++ = qAlpha(srcPixel);
        }

        dstLine += imageData.bytesPerLine();
    }

    return imageData;
}

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
        m_textContent = push.content();
    } else if (push.contentType() == CONTENT_TYPE_IMAGE) {
        m_contentType = QLatin1String("image");

        QImage image;
        image.loadFromData(push.content());

        if (!image.isNull()) {
            m_imageContent = QVariant::fromValue(bb::cascades::Image(fromQImage(image)));
        } else {
            m_imageContent = QVariant();
            qDebug() << "Error loading image";
        }
    } else if (push.fileExtension() == FILE_EXTENSION_HTML) {
        m_contentType = QLatin1String("richText");
        m_textContent = push.content();
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
