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

#include "netimagemanager.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QDir>

using namespace bb::cascades;

const char* const NetImageManager::mDefaultId = "netimagemanager";

NetImageManager::NetImageManager(QObject *parent) :
        QObject(parent)
{
    mCacheId = mDefaultId;
    mCacheSize = 125;

    QString diskPath = QDir::homePath() + "/" + mCacheId;

    if (!QDir(diskPath).exists()) {
        QDir().mkdir(diskPath);
    }

    // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
    // we get when connecting to the address given by mWeatherAdress.
    connect(&mAccessManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & )), this,
            SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));

    // Connect to the reply finished signal to httpFinsihed() Slot function.
    connect(&mAccessManager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(httpFinished(QNetworkReply *)));
}

NetImageManager::~NetImageManager()
{
}

void NetImageManager::lookUpImage(const QString imageName)
{
    QUrl url = QUrl(imageName);

    // Check if image is stored on disc
    QString diskPath = QDir::homePath() + "/" + mCacheId + "/"
            + QString::number(qHash(url.toString())) + ".JPG";

    QFile imageFile(diskPath);

    // If the file exists, send a signal the image is ready
    if (imageFile.exists()) {
        emit imageReady(diskPath, url.toString());
    }
    else {
        // otherwise let's download the file, but first we show a loading image
        emit imageReady(imageName, "loading");

        QNetworkRequest request(url);
        if (mQueue.isEmpty()) {
            mAccessManager.get(request);
        }

        mQueue.append(request);
    }

}

void NetImageManager::setCacheId(QString cacheId)
{
    if (mCacheId != cacheId) {
        mCacheId = cacheId;

        QString diskPath = QDir::homePath() + "/" + mCacheId;

        if (!QDir(diskPath).exists()) {
            QDir().mkdir(diskPath);
        }

        emit cacheIdChanged(mCacheId);
    }
    houseKeep();
}

QString NetImageManager::cacheId()
{
    return mCacheId;
}

void NetImageManager::setCacheSize(int cacheSize)
{
    if (mCacheSize != cacheSize) {
        mCacheSize = cacheSize;
        emit cacheSizeChanged(mCacheSize);
    }
    houseKeep();
}

int NetImageManager::cacheSize()
{
    return mCacheSize;
}

void NetImageManager::houseKeep()
{
    QString diskPath = QDir::homePath() + "/" + mCacheId + "/";

    QDir directory(diskPath);

    if (directory.count() > (uint) mCacheSize) {
        //Find the oldest file and delete it.
        QFileInfoList list = directory.entryInfoList(QDir::Files, QDir::Time);
        QFile::remove(list.at(list.size() - 1).absoluteFilePath());
        //maybe there are more then the permitted amount of files here, let's call housekeeping again
        houseKeep();
    }
}

void NetImageManager::httpFinished(QNetworkReply * reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QImage qImage;
        qImage.loadFromData(reply->readAll());

        if (qImage.isNull()) {
            return;
        }

        // When the download is finished we make a hash-tag for the image out of it's url so we can find it again,
        // then we save it as a .JPG
        QString diskPath = QDir::homePath() + "/" + mCacheId + "/"
                + QString::number(qHash(reply->url().toString())) + ".JPG";

        if (qImage.save(diskPath)) {
            // houseKeep() is called to see that we don't save more then we are allowed in the cache
            houseKeep();
            emit imageReady(diskPath, reply->url().toString());
         }

        //we remove the first item in the download queue
        if (!mQueue.isEmpty()) {
            mQueue.removeFirst();

            if (!mQueue.isEmpty()) {
                QNetworkRequest request = mQueue.first();
                mAccessManager.get(request);
            }
        }
    } else {
        //Handle error
        qDebug() << "Could Not access image" << reply->url().toString();
    }
    reply->deleteLater();
}

void NetImageManager::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
// Ignore all SSL errors to be able to load from JSON file from the secure address.
// It might be a good idea to display an error message indicating that security may be compromised.
//
// The errors we get are:
// "SSL error: The issuer certificate of a locally looked up certificate could not be found"
// "SSL error: The root CA certificate is not trusted for this purpose"
// Seems to be a problem with how the server is set up and a known QT issue QTBUG-23625

    reply->ignoreSslErrors(errors);
}
