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

#include "app.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;
using namespace bb::system;

//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_invokeManager(new InvokeManager(this))
    , m_backButtonVisible(false)
{
    // Listen to incoming invocation requests
    connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    connect(m_invokeManager, SIGNAL(cardResizeRequested(const bb::system::CardResizeMessage&)), this, SLOT(resized(const bb::system::CardResizeMessage&)));
    connect(m_invokeManager, SIGNAL(cardPooled(const bb::system::CardDoneMessage&)), this, SLOT(pooled(const bb::system::CardDoneMessage&)));

    // Initialize properties with default values
    switch (m_invokeManager->startupMode()) {
        case ApplicationStartupMode::LaunchApplication:
            m_startupMode = tr("Launch");
            break;
        case ApplicationStartupMode::InvokeApplication:
            m_startupMode = tr("Invoke");
            break;
        case ApplicationStartupMode::InvokeViewer:
            m_startupMode = tr("Viewer");
            break;
        case ApplicationStartupMode::InvokeCard:
            m_startupMode = tr("Card");
            break;
    }

    m_source = m_target = m_action = m_mimeType = m_uri = m_data = m_status = tr("--");
    m_title = tr("InvokeClient");

    // Create the UI
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_app", this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

void App::cardDone()
{
    // Assemble message
    CardDoneMessage message;
    message.setData(tr("Card: I am done. yay!"));
    message.setDataType("text/plain");
    message.setReason(tr("Success!"));

    // Send message
    m_invokeManager->sendCardDone(message);
}

//! [1]
void App::handleInvoke(const InvokeRequest& request)
{
    // Copy data from incoming invocation request to properties
    m_source = QString::fromLatin1("%1 (%2)").arg(request.source().installId()).arg(request.source().groupId());
    m_target = request.target();
    m_action = request.action();
    m_mimeType = request.mimeType();
    m_uri = request.uri().toString();
    m_data = QString::fromUtf8(request.data());

    m_backButtonVisible = false;

    if (m_target == "com.example.bb10samples.invocation.openimage1") {
        m_title = tr("Open Image 1");
    } else if (m_target == "com.example.bb10samples.invocation.openimage2") {
        m_title = tr("Open Image 2");
    } else if (m_target == "com.example.bb10samples.invocation.card.previewer") {
        m_title = tr("Previewer");
        m_backButtonVisible = true;
    } else if (m_target == "com.example.bb10samples.invocation.card.composer") {
        m_title = tr("Composer");
    } else if (m_target == "com.example.bb10samples.invocation.card.picker") {
        m_title = tr("Picker");
    }

    // Signal that the properties have changed
    emit requestChanged();
}
//! [1]

void App::resized(const bb::system::CardResizeMessage&)
{
    m_status = tr("Resized");
    emit statusChanged();
}

void App::pooled(const bb::system::CardDoneMessage&)
{
    m_status = tr("Pooled");
    emit statusChanged();
}

QString App::startupMode() const
{
    return m_startupMode;
}

QString App::source() const
{
    return m_source;
}

QString App::target() const
{
    return m_target;
}

QString App::action() const
{
    return m_action;
}

QString App::mimeType() const
{
    return m_mimeType;
}

QString App::uri() const
{
    return m_uri;
}

QString App::data() const
{
    return m_data;
}

QString App::status() const
{
    return m_status;
}

QString App::title() const
{
    return m_title;
}

bool App::backButtonVisible() const
{
    return m_backButtonVisible;
}
