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

#include "Loader.hpp"

using namespace bb::cascades;

// A helper method to retrieve the declarative engine that manages a declarative object
static QDeclarativeEngine* qmlEngine(QObject *object)
{
    QDeclarativeContext *context = QDeclarativeEngine::contextForObject(object);
    return (context ? context->engine() : 0);
}

//! [0]
Loader::Loader(bb::cascades::Container *parent)
    : CustomControl(parent)
    , m_control(0)
    , m_component(0)
    , m_ownsComponent(false)
{
    /**
     * The Loader should only be visible if a embedded component has been loaded,
     * so hide it by default.
     */
    setVisible(false);
}
//! [0]

Loader::~Loader()
{
}

QUrl Loader::source() const
{
    return m_source;
}

//! [1]
void Loader::setSource(const QUrl &url)
{
    if (m_source == url)
        return;

    // If a new external source has been defined, clear the previous loaded component
    clear();
    m_source = url;

    if (m_source.isEmpty()) {
        /**
         * If the source is empty, the user only wants to unload the current component,
         * so we simply emit the change notifications and return.
         */
        emit sourceChanged();
        emit statusChanged();
        emit progressChanged();
        emit controlChanged();
        return;
    }

    // Let the QDeclarativeComponent do the actual loading of the external QML file
    m_component = new QDeclarativeComponent(qmlEngine(this), m_source, this);
    m_ownsComponent = true;

    // Start the loading
    load();
}
//! [1]

QDeclarativeComponent* Loader::sourceComponent() const
{
    return m_component;
}

//! [2]
void Loader::setSourceComponent(QDeclarativeComponent *component)
{
    if (m_component == component)
        return;

    // If a new component has been defined, clear the previous loaded component
    clear();

    // Use the passed component as our embedded component ...
    m_component = component;

    // ... however we do not own the component in this case
    m_ownsComponent = false;

    if (!m_component) {
        /**
         * If the component is @c null, the user only wants to unload the current component,
         * so we simply emit the change notifications and return.
         */
        emit sourceChanged();
        emit statusChanged();
        emit progressChanged();
        emit controlChanged();
        return;
    }

    // Start the loading
    load();
}
//! [2]

void Loader::resetSourceComponent()
{
    // Trigger the unload of the currently loaded component
    setSourceComponent(0);
}

Loader::Status Loader::status() const
{
    if (m_component)
        return static_cast<Loader::Status>(m_component->status());

    if (m_control)
        return Ready;

    return (m_source.isEmpty() ? Null : Error);
}

qreal Loader::progress() const
{
    if (m_control)
        return 1.0;

    if (m_component)
        return m_component->progress();

    return 0.0;
}

bb::cascades::Control* Loader::control() const
{
    return m_control;
}

//! [3]
void Loader::sourceLoaded()
{
    if (m_component) { // Sanity check

        /**
         * If an error occured during loading, just emit the change notification
         * signals, the 'status' property will reflect that error state.
         */
        if (!m_component->errors().isEmpty()) {
            emit statusChanged();
            emit progressChanged();
            emit sourceChanged();
            emit controlChanged();
            return;
        }

        // Get the declarative context of the declarative component
        QDeclarativeContext *creationContext = m_component->creationContext();

        // If it has not been set, use the declarative context of the Loader element as fallback
        if (!creationContext)
            creationContext = QDeclarativeEngine::contextForObject(this);

        // Create a new child context that will be used as declarative context for the Cascades object
        QDeclarativeContext *ctxt = new QDeclarativeContext(creationContext);
        ctxt->setContextObject(this);

        // Load the Cascades object
        QObject *object = m_component->create(ctxt);
        if (object) {
            m_control = qobject_cast<bb::cascades::Control*>(object);
            if (m_control) {
                // If the loaded Cascades object is a Control, we use it as root element for the Loader and make it visible
                setRoot(m_control);
                setVisible(true);
            } else {
                // Otherwise report an error
                qmlInfo(this) << tr("Loader does not support loading non-visual elements.");
                delete object;
                delete ctxt;
            }
        } else {
            // Cleanup if the loading failed
            delete object;
            delete ctxt;
            m_source = QUrl();
        }

        emit sourceChanged();
        emit statusChanged();
        emit progressChanged();
        emit controlChanged();
        emit loaded();
    }
}
//! [3]

//! [4]
void Loader::clear()
{
    // If we own the declarative component, we have to delete it
    if (m_ownsComponent) {
        m_component->deleteLater();
        m_component = 0;
        m_ownsComponent = false;
    }

    m_source = QUrl();

    /**
     * Remove and delete the Cascades control that represents the loaded component
     * and hide the Loader.
     */
    if (m_control) {
        setRoot(0);
        setVisible(false);
        m_control->deleteLater();
        m_control = 0;
    }
}
//! [4]

//! [5]
void Loader::load()
{
    // Sanity check
    if (!m_component)
        return;

    if (!m_component->isLoading()) {
        // The component has finished loading already
        sourceLoaded();
    } else {
        // Setup the signal/slot connections to be informed about progress and state changes
        bool ok = connect(m_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
                          this, SLOT(sourceLoaded()));
        Q_ASSERT(ok);
        ok = connect(m_component, SIGNAL(progressChanged(qreal)),
                     this, SIGNAL(progressChanged()));
        Q_ASSERT(ok);
        emit statusChanged();
        emit progressChanged();
        emit sourceChanged();
        emit controlChanged();
    }
}
//! [5]
