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

#ifndef ARTIFACTREQUEST_HPP
#define ARTIFACTREQUEST_HPP

#include <QtCore/QObject>

/*
 * This class is responsible for making a REST call to the maven central api
 * to retrieve the search results for a given artifact name. It emits the complete()
 * signal when the request has completed.
 */
//! [0]
class ArtifactRequest : public QObject
{
    Q_OBJECT

public:
    ArtifactRequest(QObject *parent = 0);

    /*
     * Makes a network call to retrieve the maven central feed for the specified artifact name
     * @param artifactName - the artifact name of the feed to extract
     * @see onArtifactlineReply
     */
    void requestArtifactline(const QString &artifactName);

Q_SIGNALS:
    /*
     * This signal is emitted when the search request is received
     * @param info - on success, this is the json reply from the request
     *               on failure, it is an error string
     * @param success - true if search request succeed, false if not
     */
    void complete(const QString &info, bool success);

private Q_SLOTS:
    /*
     * Callback handler for QNetworkReply finished() signal
     */
    void onArtifactlineReply();
};
//! [0]

#endif
