/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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

#include "ArtifactRequest.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

/*
 * Default constructor
 */
ArtifactRequest::ArtifactRequest(QObject *parent)
    : QObject(parent)
{
}

/*
 * ArtifactRequest::requestArtifactline(const QString &artifactName)
 *
 * Makes a network call to retrieve the maven central feed for the specified artifactname
 */
//! [0]
void ArtifactRequest::requestArtifactline(const QString &artifactName)
{
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);

    const QString queryUri = QString::fromLatin1("http://search.maven.org/solrsearch/select?q=%1&rows=20&wt=json").arg(artifactName);

    QNetworkRequest request(queryUri);

    QNetworkReply* reply = networkAccessManager->get(request);

    bool ok = connect(reply, SIGNAL(finished()), this, SLOT(onArtifactlineReply()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]

/*
 * ArtifactRequest::onArtifactlineReply()
 *
 * Callback handler for QNetworkReply finished() signal
 */
//! [1]
void ArtifactRequest::onArtifactlineReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    bool success = false;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray buffer = reply->readAll();
                response = QString::fromUtf8(buffer);
                success = true;
            }
        } else {
            response =  tr("Error: %1 status: %2").arg(reply->errorString(), reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
        }

        reply->deleteLater();
    }

    if (response.trimmed().isEmpty()) {
        response = tr("Artifact request failed. Check internet connection");
    }

    emit complete(response, success);
}
//! [1]
