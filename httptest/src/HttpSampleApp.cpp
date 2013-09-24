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

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <QSettings>

#include "HttpSampleApp.hpp"
#include "ExternalIP.hpp"
#include "RequestHeaders.hpp"
#include "PostHttp.hpp"
#include "AppSettings.hpp"

using namespace bb::cascades;

/**
 * HttpSampleApp
 *
 * In this class you will learn the following:
 * -- Setup Organization and Application name to use QSettings
 * -- Register C++ classes for use in QML documents
 */

/**
 * HttpSampleApp::HttpSampleApp()
 *
 * Initial setup for your App
 * Setting up the PostHttp, ExternalIP and RequestHeaders classes for use as a context in QML
 * Create initial root Pane
 */
//! [0]
HttpSampleApp::HttpSampleApp()
{
    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("HttpSample");

    //add Post class as qml type
    qmlRegisterType<PostHttp>("Network.PostHttp", 1, 0, "PostHttp");

    //add ExternalIP class as a qml type
    qmlRegisterType<ExternalIP>("Network.ExternalIP", 1, 0, "ExternalIP");

    //add custom object RequestHeaders class as a qml type
    qmlRegisterType<RequestHeaders>("Network.RequestHeaders", 1, 0, "RequestHeaders");

    //add a QTimer class as a qml type
    qmlRegisterType<QTimer>("my.library", 1, 0, "QTimer");

    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_httpsample", this);

    AbstractPane* root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

void HttpSampleApp::setPostBody(const QString &body)
{
    if (m_postBody == body)
        return;

    m_postBody = body;
    emit postBodyChanged();
}

QString HttpSampleApp::postBody() const
{
    return m_postBody;
}
//! [1]
void HttpSampleApp::setUseHttps(bool value)
{
    if (AppSettings::isUsingHttps() == value)
        return;

    AppSettings::setHttps(value);
    emit useHttpsChanged();
}

bool HttpSampleApp::useHttps() const
{
    return AppSettings::isUsingHttps();
}
//! [1]

