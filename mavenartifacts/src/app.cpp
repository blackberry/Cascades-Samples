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

#include "app.hpp"
#include "ArtifactRequest.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <bb/data/JsonDataAccess>
#include <bb/utility/i18n/RelativeDateFormatter>

using namespace bb::cascades;
using namespace bb::data;

/*
 * This application demonstrates how to retrieve a feed and populate a listview with StandardListItem
 * and a ListView with items from the json data. Also demonstrates how to parse json into a GroupDataModel
 */

/*
 * Default constructor
 */
//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_active(false)
    , m_error(false)
    , m_model(new GroupDataModel(QStringList() << "id", this))
{
    m_model->setGrouping(ItemGrouping::None);

    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_artifactline", this);

    AbstractPane* root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

//! [1]
void App::reset()
{
    m_error = false;
    m_errorMessage.clear();

    emit statusChanged();
}
//! [1]

/*
 *  App::requestArtifact(const QString &artifactName)
 *
 *  Initiates an http request to retrieve the artifacts containing the artifact name
 *  referred to by "artifactName" parameter. When the network request is complete
 *  onArtifactsline method is called with the result
 */
//! [2]
void App::requestArtifact(const QString &artifactName)
{
    if (m_active)
        return;

    // sanitize artifactname
    const QStringList list = artifactName.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if (list.isEmpty()) {
        m_errorMessage = "please enter a valid artifact name";
        m_error = true;
        emit statusChanged();
        return;
    }

    const QString artifactId = list.first();

    ArtifactRequest* request = new ArtifactRequest(this);
    bool ok = connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onArtifactsline(QString, bool)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    request->requestArtifactline(artifactId);

    m_active = true;
    emit activeChanged();
}
//! [2]

/*
 * App::onArtifactsline(const QString &info, bool success)
 *
 * Slot handler for receiving the data from the maven central network request
 * made in App::requestArtifact(). On success it navigates to the appropriate
 * ListView and on failure it displays the error string from the failed
 * request.
 *
 * info - contains the json response (success) or error text (failure)
 * success - boolean flag indicating success of failure of the request
 */
//! [3]
void App::onArtifactsline(const QString &info, bool success)
{
    ArtifactRequest *request = qobject_cast<ArtifactRequest*>(sender());

    if (success) {
        parseResponse(info);

        emit artifactsLoaded();
    } else {
        m_errorMessage = info;
        m_error = true;
        emit statusChanged();
    }

    m_active = false;
    emit activeChanged();

    request->deleteLater();
}
//! [3]

/*
 * App::parseResponse()
 *
 * Parses the JSON data from the maven central response and populates the ListView.
 */
//! [4]
void App::parseResponse(const QString &response)
{
    m_model->clear();

    if (response.trimmed().isEmpty())
        return;

    // Parse the json response with JsonDataAccess
    JsonDataAccess dataAccess;
    const QVariantMap variant = dataAccess.loadFromBuffer(response).toMap();

    // The qvariant is a map of searches which is extracted as a list
    const QVariantList feed = variant["response"].toMap()["docs"].toList();

    // For each object in the array, push the variantmap in its raw form
    // into the ListView
    foreach (const QVariant &artifact, feed) {
        m_model->insert(artifact.toMap());
    }
}
//! [4]

bool App::active() const
{
    return m_active;
}

bool App::error() const
{
    return m_error;
}

QString App::errorMessage() const
{
    return m_errorMessage;
}

QString App::dateFromTimestamp(const QString &timestamp) {
	QDateTime date;
	date.setMSecsSinceEpoch(timestamp.toLongLong());
	return date.toString();
}

bb::cascades::DataModel* App::model() const
{
    return m_model;
}
