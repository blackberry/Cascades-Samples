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

#ifndef APP_HPP
#define APP_HPP

#include <bb/cascades/GroupDataModel>

#include <QtCore/QObject>

//! [0]
class App : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(bool error READ error NOTIFY statusChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY statusChanged)

    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

public:
    App(QObject *parent = 0);

public Q_SLOTS:
    /*
     * Called by the QML to get a twitter feed for the screen nane
     */
    void requestTweets(const QString &screenName);

    /*
     * Allows the QML to reset the state of the application
     */
    void reset();

Q_SIGNALS:
    /*
     * This signal is emitted whenever the tweets have been loaded successfully
     */
    void tweetsLoaded();

    /*
     * The change notification signals of the properties
     */
    void activeChanged();
    void statusChanged();

private Q_SLOTS:
    /*
     * Handles the complete signal from TwitterRequest when
     * the request is complete
     * @see TwitterRequest::complete()
     */
    void onTwitterTimeline(const QString &info, bool success);

private:
    /*
     * Parses the JSON response from the twitter request
     */
    void parseResponse(const QString&);

    /*
     * The accessor methods of the properties
     */
    bool active() const;
    bool error() const;
    QString errorMessage() const;
    bb::cascades::DataModel* model() const;

private:
    bool m_active;
    bool m_error;
    QString m_errorMessage;
    bb::cascades::GroupDataModel* m_model;
};
//! [0]

#endif
