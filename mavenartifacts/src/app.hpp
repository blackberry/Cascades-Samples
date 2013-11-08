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
     * Called by the QML to get a artifact feed for the artifact search name
     */
    void requestArtifact(const QString &artifactName);

    /*
     * Allows the QML to reset the state of the application
     */
    void reset();

    /**
     * Called to get date string from a timestamp.
     */
    Q_INVOKABLE QString dateFromTimestamp(const QString &timestamp);

Q_SIGNALS:
    /*
     * This signal is emitted whenever the artifacts have been loaded successfully
     */
    void artifactsLoaded();

    /*
     * The change notification signals of the properties
     */
    void activeChanged();
    void statusChanged();

private Q_SLOTS:
    /*
     * Handles the complete signal from ArtifactRequest when
     * the request is complete
     * @see ArtifactRequest::complete()
     */
    void onArtifactsline(const QString &info, bool success);

private:
    /*
     * Parses the JSON response from the artifact request
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
