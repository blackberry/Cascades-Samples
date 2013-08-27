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
#include "TwitterRequest.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

/*
 * This application demonstrates how to retrieve a twitter feed and populate a listview with StandardListItem
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
    , m_model(new GroupDataModel(QStringList() << "id_str", this))
{
    m_model->setGrouping(ItemGrouping::None);

    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_timeline", this);

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
 *  App::requestTweets(const QString &screenName)
 *
 *  Initiates an http request to retrieve the timeline belonging to the twitter name
 *  referred to by "screenName" parameter. When the network request is complete
 *  onTwitterTimeline method is called with the result
 */
//! [2]
void App::requestTweets(const QString &screenName)
{
    if (m_active)
        return;

    // sanitize screenname
    const QStringList list = screenName.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if (list.isEmpty()) {
        m_errorMessage = "please enter a valid screen name";
        m_error = true;
        emit statusChanged();
        return;
    }

    const QString twitterId = (list.first().startsWith('@') ? list.first().mid(1) : list.first());

    TwitterRequest* request = new TwitterRequest(this);
    bool ok = connect(request, SIGNAL(complete(QString, bool)), this, SLOT(onTwitterTimeline(QString, bool)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    request->requestTimeline(twitterId);

    m_active = true;
    emit activeChanged();
}
//! [2]

/*
 * App::onTwitterTimeline(const QString &info, bool success)
 *
 * Slot handler for receiving the data from the twitter network request
 * made in App::getTimeline(). On success it navigates to the appropriate
 * ListView and on failure it displays the error string from the failed
 * request.
 *
 * info - contains the json response (success) or error text (failure)
 * success - boolean flag indicating success of failure of the request
 */
//! [3]
void App::onTwitterTimeline(const QString &info, bool success)
{
    TwitterRequest *request = qobject_cast<TwitterRequest*>(sender());

    if (success) {
        parseResponse(info);

        emit tweetsLoaded();
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
 * Parses the JSON data from the twitter response and populates the ListView.
 */
//! [4]
void App::parseResponse(const QString &response)
{
    m_model->clear();

    if (response.trimmed().isEmpty())
        return;

    // Parse the json response with JsonDataAccess
    JsonDataAccess dataAccess;
    const QVariant variant = dataAccess.loadFromBuffer(response);

    // The qvariant is an array of tweets which is extracted as a list
    const QVariantList feed = variant.toList();

    // For each object in the array, push the variantmap in its raw form
    // into the ListView
    foreach (const QVariant &tweet, feed) {
        m_model->insert(tweet.toMap());
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

bb::cascades::DataModel* App::model() const
{
    return m_model;
}
