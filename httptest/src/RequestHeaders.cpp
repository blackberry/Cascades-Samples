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

#include "RequestHeaders.hpp"

#include "AppSettings.hpp"

#include <bb/data/JsonDataAccess>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QUrl>

/**
 * RequestHeaders
 *
 * In this class you will learn the following:
 * -- How to use QNetworkAccessManager to make a network request
 * -- How to setup a secure connection with QSslConfiguration
 * -- How to read a network response with QNetworkReply
 * -- How to parse JSON data using JsonDataAccess
 * -- How to read the headers in an http response
 */

RequestHeaders::RequestHeaders(QObject* parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
}

/**
 * RequestHeaders::getRequest()
 *
 * Setup an http get request using SSL if configured
 */
//! [0]
void RequestHeaders::getRequest()
{
    const QUrl url("http://httpbin.org/get");

    QNetworkRequest request(url);

    if (AppSettings::isUsingHttps()) {
        request.setUrl(QUrl("https://httpbin.org/get"));

        QSslConfiguration config = request.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1);
        request.setSslConfiguration(config);
    }

    QNetworkReply* reply = m_networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onGetReply()));
}
//! [0]

/**
 * RequestHeaders::onGetReply()
 *
 * SLOT
 * Handles the http response by parsing JSON and printing out the response http headers
 */
//! [1]
void RequestHeaders::onGetReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();

            if (available > 0) {
                const QByteArray buffer(reply->readAll());

                // The data from reply is in a json format e.g
                //"args": {},
                //"headers": {
                //  "Accept": "*/*",
                //  "Connection": "close",
                //  "Content-Length": "",
                //  "Content-Type": "",
                //  "Host": "httpbin.org",
                //  "User-Agent": "curl/7.19.7 (universal-apple-darwin10.0) libcurl/7.19.7 OpenSSL/0.9.8l zlib/1.2.3"
                //},
                //"origin": "24.127.96.129",
                //"url": "http://httpbin.org/get"

                bb::data::JsonDataAccess ja;
                const QVariant jsonva = ja.loadFromBuffer(buffer);
                const QMap<QString, QVariant> jsonreply = jsonva.toMap();

                // Locate the header array
                QMap<QString, QVariant>::const_iterator it = jsonreply.find("headers");
                if (it != jsonreply.end()) {
                    // Print everything in header array
                    const QMap<QString, QVariant> headers = it.value().toMap();
                    for (QMap<QString, QVariant>::const_iterator hdrIter = headers.begin(); hdrIter != headers.end(); ++hdrIter) {
                        if (hdrIter.value().toString().trimmed().isEmpty())
                            continue; // Skip empty values

                        response += QString::fromLatin1("%1: %2\r\n").arg(hdrIter.key(), hdrIter.value().toString());
                    }
                }

                // Print everything else
                for (it = jsonreply.begin(); it != jsonreply.end(); it++) {
                    if (it.value().toString().trimmed().isEmpty())
                        continue;  // Skip empty values

                    response += QString::fromLatin1("%1: %2\r\n").arg(it.key(), it.value().toString());
                }
            }

        } else {
            response =  tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
            qDebug() << response;
        }

        reply->deleteLater();
    }

    if (response.trimmed().isEmpty()) {
        response = tr("Unable to retrieve request headers");
    }

    emit complete(response);
}
//! [1]
