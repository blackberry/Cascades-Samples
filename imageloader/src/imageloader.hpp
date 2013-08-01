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

#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <QImage>
#include <QByteArray>

#include <bb/cascades/Image>

/*
 * This class retrieves an image from the web, then converts the binary
 * data into a bb::cascades::Image and makes it available through a property.
 */
//! [0]
class ImageLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant image READ image NOTIFY imageChanged)
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    /*
     * Creates a new image loader.
     *
     * @param imageUrl The url to load the image from.
     */
    ImageLoader(const QString &imageUrl, QObject* parent = 0);

    /*
     * Destroys the image loader.
     */
    ~ImageLoader();

    /*
     * Loads the image.
     */
    void load();

Q_SIGNALS:
    // The change notification signals of the properties
    void imageChanged();
    void labelChanged();
    void loadingChanged();

private Q_SLOTS:
    /*
     * Response handler for the network operation.
     */
    void onReplyFinished();

    /*
     * Response handler for the image process operation.
     */
    void onImageProcessingFinished();

private:
    // The accessor methods of the properties
    QVariant image() const;
    QString label() const;
    bool loading() const;

    // The property values
    bb::cascades::Image m_image;
    QString m_label;
    bool m_loading;

    // The URL of the image that should be loaded
    QString m_imageUrl;

    // The thread status watcher
    QFutureWatcher<QImage> m_watcher;
};
//! [0]

#endif
