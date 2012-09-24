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
#include <bb/system/InvokeAction>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeTargetReply>

using namespace bb::cascades;
using namespace bb::system;

//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_targetType(0)
    , m_action(QLatin1String("bb.action.OPEN"))
    , m_mimeType(QLatin1String("image/png"))
    , m_model(new GroupDataModel(this))
    , m_invokeManager(new InvokeManager(this))
{
    // Disable item grouping in the targets result list
    m_model->setGrouping(ItemGrouping::None);

    // Create signal/slot connections to handle card status changes
    connect(m_invokeManager, SIGNAL(childCardDone(const bb::system::CardDoneMessage&)), this, SLOT(childCardDone(const bb::system::CardDoneMessage&)));
    connect(m_invokeManager, SIGNAL(peekStarted(bb::system::CardPeek::Type)), this, SLOT(peekStarted(bb::system::CardPeek::Type)));
    connect(m_invokeManager, SIGNAL(peekEnded()), this, SLOT(peekEnded()));

    // Load the UI from the QML file
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

//! [1]
void App::invoke()
{
    // Create a new invocation request
    InvokeRequest request;

    // Setup the request properties according to the current configuration
    if (!m_action.isEmpty()) {
        if (m_action != QLatin1String("__All") && m_action != QLatin1String("__MenuActions")) {
            request.setAction(m_action);
        } else if (m_target.isEmpty()) {
            m_errorMessage = tr("[%1] is not a valid action type for an unbound invocation.").arg(m_action);
            emit errorMessageChanged();
            return;
        }
    }

    if (!m_mimeType.isEmpty()) {
        request.setMimeType(m_mimeType);
    } else {
        m_errorMessage = tr("MIME type must be specified!");
        emit errorMessageChanged();
        return;
    }

    if (!m_uri.isEmpty())
        request.setUri(m_uri);

    if (!m_data.isEmpty())
        request.setData(m_data.toUtf8());

    if (!m_target.isEmpty())
        request.setTarget(m_target);

    // Start the invocation
    const InvokeReply *reply = m_invokeManager->invoke(request);
    if (reply) {
        // Ensure that processInvokeReply() is called when the invocation has finished
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(processInvokeReply()));
    } else {
        m_errorMessage = tr("Invoke Failed! InvokeReply is empty.");
        emit errorMessageChanged();
        return;
    }
}
//! [1]

//! [2]
void App::query()
{
    // Create a new query targets request
    InvokeQueryTargetsRequest request;

    // Setup the request properties according to the current configuration
    if (m_targetType == 0)
        request.setTargetTypes(InvokeTarget::Application | InvokeTarget::Card | InvokeTarget::Viewer | InvokeTarget::Service);
    else if (m_targetType == 1)
        request.setTargetTypes(InvokeTarget::Application);
    else if (m_targetType == 2)
        request.setTargetTypes(InvokeTarget::Viewer);
    else if (m_targetType == 3)
        request.setTargetTypes(InvokeTarget::Service);
    else if (m_targetType == 4)
        request.setTargetTypes(InvokeTarget::Card);

    if (!m_action.isEmpty()) {
        if (m_action == QLatin1String("__All"))
            request.setActionType(InvokeAction::All);
        else if (m_action == QLatin1String("__MenuActions"))
            request.setActionType(InvokeAction::Menu);
        else
            request.setAction(m_action);
    }

    if (m_mimeType.isEmpty() && m_uri.isEmpty()) {
        m_errorMessage = tr("MIME type OR URI must be specified!");
        emit errorMessageChanged();
        return;
    }

    if (!m_mimeType.isEmpty())
        request.setMimeType(m_mimeType);

    if (!m_uri.isEmpty())
        request.setUri(m_uri);

    // Start the query
    const InvokeReply *reply = m_invokeManager->queryTargets(request);

    // Ensure that processQueryReply() is called when the query has finished
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(processQueryReply()));
}
//! [2]

//! [3]
void App::invokeTarget(const QString &target)
{
    // Setup the configuration to invoke the given target
    m_targetType = 0;
    m_target = target;

    emit closeQueryResults();

    // Trigger the invocation
    invoke();
}
//! [3]

void App::clearError()
{
    m_errorMessage.clear();
    emit errorMessageChanged();
}

//! [4]
void App::processInvokeReply()
{
    // Get the reply from the sender object
    InvokeReply *reply = qobject_cast<InvokeReply*>(sender());

    // Check for errors during invocation
    switch (reply->error()) {
        case InvokeReplyError::BadRequest:
            m_errorMessage = tr("[ErrorBadRequest] Invoke Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::Internal:
            m_errorMessage = tr("[ErrorInternal] Invoke Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::NoTarget:
            m_errorMessage = tr("[ErrorNoTarget] Invoke Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::TargetNotOwned:
            m_errorMessage = tr("[ErrorTargetNotOwned] Invoke Failed.");
            emit errorMessageChanged();
            break;
        default:
            break;
    }

    // Delete the reply later on
    reply->deleteLater();
}
//! [4]

//! [5]
void App::processQueryReply()
{
    // Get the reply from the sender object
    InvokeQueryTargetsReply *reply = qobject_cast<InvokeQueryTargetsReply*>(sender());

    if (reply->error() == InvokeReplyError::None) { // If no error occurred ...
        // Clear the target result model
        m_model->clear();

        // Iterate over the reported actions and targets
        foreach (const InvokeAction &action, reply->actions()) {
            foreach (const InvokeTarget &target, action.targets()) {
                // Add one entry to the model for each target
                QVariantMap entry;
                entry["label"] = target.label();
                entry["name"] = target.name();
                entry["imageSource"] = target.icon();

                m_model->insert(entry);
            }
        }

        // Signal that the query was successful
        emit queryFinished();
    }

    // Check for errors during invocation
    switch (reply->error()) {
        case InvokeReplyError::BadRequest:
            m_errorMessage = tr("[ErrorBadRequest] Query Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::Internal:
            m_errorMessage = tr("[ErrorInternal] Query Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::NoTarget:
            m_errorMessage = tr("[ErrorNoTarget] Query Failed!");
            emit errorMessageChanged();
            break;
        case InvokeReplyError::TargetNotOwned:
            m_errorMessage = tr("[ErrorTargetNotOwned] Query Failed.");
            emit errorMessageChanged();
            break;
        default:
            break;
    }

    // Delete the reply later on
    reply->deleteLater();
}
//! [5]

void App::peekStarted(bb::system::CardPeek::Type)
{
    m_statusMessage = tr("Peek Status: Started");
    emit statusMessageChanged();
}

void App::peekEnded()
{
    m_statusMessage = tr("Peek Status: Ended");
    emit statusMessageChanged();
}

void App::childCardDone(const bb::system::CardDoneMessage &message)
{
    if (!message.data().isEmpty()) {
        m_statusMessage = message.data();
        emit statusMessageChanged();
    }
}

int App::targetType() const
{
    return m_targetType;
}

void App::setTargetType(int targetType)
{
    if (m_targetType == targetType)
        return;

    m_targetType = targetType;
    emit targetTypeChanged();
}

QString App::action() const
{
    return m_action;
}

void App::setAction(const QString &action)
{
    if (m_action == action)
        return;

    m_action = action;
    emit actionChanged();
}

QString App::mimeType() const
{
    return m_mimeType;
}

void App::setMimeType(const QString &mimeType)
{
    if (m_mimeType == mimeType)
        return;

    m_mimeType = mimeType;
    emit mimeTypeChanged();
}

QString App::uri() const
{
    return m_uri;
}

void App::setUri(const QString &uri)
{
    if (m_uri == uri)
        return;

    m_uri = uri;
    emit uriChanged();
}

QString App::data() const
{
    return m_data;
}

void App::setData(const QString &data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged();
}

QString App::target() const
{
    return m_target;
}

void App::setTarget(const QString &target)
{
    if (m_target == target)
        return;

    m_target = target;
    emit targetChanged();
}

bb::cascades::GroupDataModel* App::model() const
{
    return m_model;
}

QString App::errorMessage() const
{
    return m_errorMessage;
}

QString App::statusMessage() const
{
    return m_statusMessage;
}
