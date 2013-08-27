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

#include "Repeater.hpp"

#include <bb/cascades/DataModel>

using namespace bb::cascades;

//! [0]
Repeater::Repeater(Container *parent)
    : CustomControl(parent)
    , m_model(0)
    , m_delegate(0)
{
    // The Repeater itself is invisible
    setVisible(false);
}
//! [0]

Repeater::~Repeater()
{
}

QVariant Repeater::model() const
{
    return m_model;
}

//! [1]
void Repeater::setModel(const QVariant &model)
{
    if (m_model == model)
        return;

    // If the model has changed, clear all previously generated controls and regenerate them
    m_model = model;
    clear();
    regenerate();

    emit modelChanged();
}
//! [1]

QDeclarativeComponent* Repeater::delegate() const
{
    return m_delegate;
}

//! [2]
void Repeater::setDelegate(QDeclarativeComponent *delegate)
{
    if (m_delegate == delegate)
        return;

    // If the delegate has changed, clear all previously generated controls and regenerate them
    m_delegate = delegate;
    clear();
    regenerate();

    emit delegateChanged();
}
//! [2]

//! [3]
void Repeater::clear()
{
    // First remove all generated controls from their parent container ...
    Container *container = qobject_cast<Container*>(parent());
    foreach (Control *control, m_controls) {
        container->remove(control);
    }

    // ... and delete them afterwards.
    qDeleteAll(m_controls);
    m_controls.clear();
}
//! [3]

//! [4]
void Repeater::regenerate()
{
    // Sanity check
    if (!m_delegate)
        return;

    /**
     * Find the position of the Repeater inside its parent container, so that we can insert
     * the newly generated items there.
     */
    Container *container = qobject_cast<Container*>(parent());
    const int repeaterPosition = container->indexOf(this);

    /**
     * Now we check what the type of the model is and generate the controls.
     */
    if (m_model.type() == QVariant::Int || m_model.type() == QVariant::Double) {
        /**
         * If a numeric value (N) was passed as model, we create a new control for each entry
         * in the sequence [1 - N] and make the index available as context property.
         */
        const int counter = m_model.toInt();

        for (int i = 1; i <= counter; ++i) {
            // Create a child context that is specific for this new control
            QDeclarativeContext *context = new QDeclarativeContext(m_delegate->creationContext(), this);

            // Make the current index available as context properties
            context->setContextProperty("index", i);
            context->setContextProperty("modelData", i);

            // Create the control from the delegate component
            QObject *object = m_delegate->create(context);
            Control *control = qobject_cast<Control*>(object);

            // Insert the new control into the parent container
            container->insert(repeaterPosition + i, control);

            // Keep a reference to the control, so that we can delete it later on
            m_controls.append(control);
        }
    } else if (m_model.type() == QVariant::List) {
        /**
         * If a QVariantList was passed as model, we create a new control for each entry of the list
         * and make the value at that position available as context property.
         */
        const QVariantList list = m_model.toList();

        for (int i = 0; i < list.count(); ++i) {
            // Create a child context that is specific for this new control
            QDeclarativeContext *context = new QDeclarativeContext(m_delegate->creationContext(), this);

            // Make the current index and value available as context properties
            context->setContextProperty("index", i + 1);
            context->setContextProperty("modelData", list.at(i));

            // Create the control from the delegate component
            QObject *object = m_delegate->create(context);
            Control *control = qobject_cast<Control*>(object);

            // Insert the new control into the parent container
            container->insert(repeaterPosition + i + 1, control);

            // Keep a reference to the control, so that we can delete it later on
            m_controls.append(control);
        }
    } else {
        QObject *model = m_model.value<QObject*>();
        bb::cascades::DataModel *dataModel = qobject_cast<bb::cascades::DataModel*>(model);

        if (dataModel) {
            /**
             * If a DataModel was passed as model, we create a new control for each entry of the model
             * and make all key/value pairs of the entry available as context properties.
             */
            for (int i = 0; i < dataModel->childCount(QVariantList()); ++i) {
                // Create a child context that is specific for this new control
                QDeclarativeContext *context = new QDeclarativeContext(m_delegate->creationContext(), this);

                // Make the current index available as context properties
                context->setContextProperty("index", i + 1);

                // Make all key/value pairs of the current model entry available as context properties
                const QVariantMap map = dataModel->data(QVariantList() << i).toMap();
                foreach (const QString &key, map.keys()) {
                    context->setContextProperty(key, map.value(key));
                }

                // Create the control from the delegate component
                QObject *object = m_delegate->create(context);
                Control *control = qobject_cast<Control*>(object);

                // Insert the new control into the parent container
                container->insert(repeaterPosition + i + 1, control);

                // Keep a reference to the control, so that we can delete it later on
                m_controls.append(control);
            }
        }
    }
}
//! [4]
