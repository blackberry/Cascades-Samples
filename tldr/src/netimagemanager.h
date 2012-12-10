/* Copyright (c) 2012 Research In Motion Limited.
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
#ifndef _NETIMAGECACHE_H_
#define _NETIMAGECACHE_H_

#include <bb/cascades/Image>
#include <QtNetwork/QNetworkRequest>

#include <QObject>
#include <QtGui/QImage>

using namespace bb::cascades;


/**
 * NetImageManager is a cache service for our Internet downloaded images.
 * You can set the size of the cache and an id for the cache.
 * If you want to reuse the cache between different pages it's possible.
 */
class NetImageManager: public QObject
{
    Q_OBJECT

    /**
     * This property sets the name of the image cache, if none is set we will use it to the
     * default "netimagemanager".
     */
    Q_PROPERTY(QString cacheId READ cacheId WRITE setCacheId NOTIFY cacheIdChanged)

    /**
     * Sets the size of the cache in number of files in the directory, if not set, it defaults
     * to 125 files.
     */
    Q_PROPERTY(int cacheSize READ cacheSize WRITE setCacheSize NOTIFY cacheSizeChanged)

public:
    /**
     * This is our constructor which initializes the member variables.
     * @param parent The parent QObject, if not specified, 0 is used.
     */
    NetImageManager(QObject *parent = 0);
    ~NetImageManager();

    /**
     * This function sets the cacheId property.
     *
     * @param cacheId The cacheId used for storing downloaded in a folder.
     */
    void setCacheId(QString cacheId);

    /**
     * This function return the chacheId, that is the location of the folder where images
     * are stored for the cache object.
     *
     * @return The cacheId
     */
    QString cacheId();

    /**
     * This function sets the cacheSize property.
     *
     * @param cacheId The cacheSize used for cleaning the cache folder.
     */
    void setCacheSize(int cacheSize);

    /**
     * This function return the chacheSize.
     *
     * @return The cacheId
     */
    int cacheSize();

    /**
     * Check if the image exists in cache
     *
     * @return the full path to the image if it exists otherwise 0
     */
    QString getNetImage(QString imageName);

    void lookUpImage(const QString imageName);

    /**
     * Check if the cache is full and if so deletes the oldest
     */
    void houseKeep();

    signals:
    /**
     * This signal is emitted when a new cacheId has been set
     */
    void cacheIdChanged(QString cacheId);
    void cacheSizeChanged(int cacheSize);

    void imageReady(const QString filePath, const QString imageName);

private slots:
    /**
     * This Slot function is called when the network request is complete.
     */
    void httpFinished( QNetworkReply * reply );

    /**
     * This Slot function is connected to the mAccessManager sslErrors signal. This function
     * allows us to see what errors we get when connecting to the address given by mWeatherAdress.
     *
     * @param reply The network reply
     * @param errors SSL Error List
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

private:

    // Property variables
    QString mCacheId;
    int mCacheSize;

    // String constant for the default id of the image cache
    static const char* const mDefaultId;

    // The network parameters; used for accessing a file from the Internet
    QNetworkAccessManager mAccessManager;

    QList<QNetworkRequest> mQueue;
};

#endif //  _NETIMAGECACHE_H_
