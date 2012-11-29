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

#include "ExternalIP.hpp"

#include "AppSettings.hpp"

#include <bb/data/JsonDataAccess>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QSslConfiguration>
#include <QUrl>

/**
 * ExternalIP
 *
 * In this class you will learn the following:
 * -- How to use QNetworkAccessManager to make a network request
 * -- How to setup a secure connection with QSslConfiguration
 * -- How to read a network response with QNetworkReply
 * -- How to parse JSON data using JsonDataAccess
 */
//! [0]
ExternalIP::ExternalIP(QObject* parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
}
//! [0]
/**
 * ExternalIP::getIP()
 *
 * Make a network request to httpbin.org/ip to return your external IP address in JSON
 * format
 */
//! [1]
void ExternalIP::getIP()
{
    const QUrl url("http://httpbin.org/ip");
    QNetworkRequest request(url);

    // Check App settings to see if we should use SSL
    if (AppSettings::isUsingHttps()) {
        request.setUrl(QUrl("https://httpbin.org/ip"));

        QSslConfiguration config = request.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1);
        request.setSslConfiguration(config);
    }

    QNetworkReply* reply = m_networkAccessManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(onGetReply()));
}
//! [1]
/**
 * ExternalIP::onGetReply()
 *
 * SLOT
 * Parse the network response if we get one
 */
//! [2]
void ExternalIP::onGetReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer(reply->readAll());

                // The data from reply is in a json format e.g {"origin": "24.127.96.129"}

                bb::data::JsonDataAccess ja;
                const QVariant jsonva = ja.loadFromBuffer(buffer);
                const QMap<QString, QVariant> externalip = jsonva.toMap();

                foreach (const QVariant &value, externalip) {
                    response += value.toString();
                }
            }
        } else {
            // Get http status code
            const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            // Do some error management
            response = tr("Http Error: %1").arg(httpStatus);
        }

        reply->deleteLater();
    }

    if (response.trimmed().isEmpty()) {
        response = tr("Unable to retrieve ip address");
    }

    emit complete(response);
}
//! [2]
