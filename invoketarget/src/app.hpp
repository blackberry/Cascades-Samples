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

#include <bb/system/CardDoneMessage.hpp>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>

#include <QtCore/QObject>

/*!
 * @brief Application GUI object
 */
//! [0]
class App : public QObject
{
    Q_OBJECT

    // The textual representation of the startup mode of the application
    Q_PROPERTY(QString startupMode READ startupMode NOTIFY requestChanged)

    // The values of the incoming invocation request
    Q_PROPERTY(QString source READ source NOTIFY requestChanged)
    Q_PROPERTY(QString target READ target NOTIFY requestChanged)
    Q_PROPERTY(QString action READ action NOTIFY requestChanged)
    Q_PROPERTY(QString mimeType READ mimeType NOTIFY requestChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY requestChanged)
    Q_PROPERTY(QString data READ data NOTIFY requestChanged)

    // The textual representation of the card status
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    // The title of the application
    Q_PROPERTY(QString title READ title NOTIFY requestChanged)

    // Whether the 'Back' button should be visible in the UI
    Q_PROPERTY(bool backButtonVisible READ backButtonVisible NOTIFY requestChanged)

public:
    App(QObject *parent = 0);

public Q_SLOTS:
    // This method is invoked to notify the invocation system that the action has been done
    void cardDone();

Q_SIGNALS:
    // The change notification signal of the properties
    void requestChanged();
    void statusChanged();

private Q_SLOTS:
    // This slot is called whenever an invocation request is received
    void handleInvoke(const bb::system::InvokeRequest&);

    void resized(const bb::system::CardResizeMessage&);
    void pooled(const bb::system::CardDoneMessage&);

private:
    // The accessor methods of the properties
    QString startupMode() const;
    QString source() const;
    QString target() const;
    QString action() const;
    QString mimeType() const;
    QString uri() const;
    QString data() const;
    QString status() const;
    QString title() const;
    bool backButtonVisible() const;

    // The central class to manage invocations
    bb::system::InvokeManager *m_invokeManager;

    // The property values
    QString m_startupMode;
    QString m_source;
    QString m_target;
    QString m_action;
    QString m_mimeType;
    QString m_uri;
    QString m_data;
    QString m_status;
    QString m_title;
    bool m_backButtonVisible;
};
//! [0]

#endif
