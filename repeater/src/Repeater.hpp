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

#ifndef REPEATER_HPP
#define REPEATER_HPP

#include <bb/cascades/Container>
#include <bb/cascades/CustomControl>
#include <QtCore/QList>

/**
 * The Repeater has a "delegate" property of type QDeclarativeComponent*, which specifies the component
 * used for creating the QML elements. The "model" property specifies how often the element should be
 * repeated. Apart from using an integer constant here, it is also possible to specify a QVariantList
 * or a DataModel. For the later case, the variant and the map content of the VariantList and the
 * DataModel are made available in the created QML elements by setting them as their context property.
 *
 * The fact that writing "delegate: Label { ... }" in QML creates a QDeclarativeComponent* and not a
 * Label* is a special behavior of the QML engine. Internally, the QML engine checks if the property
 * (in this case "delegate") is of type QDeclarativeComponent*, and if so, treats the assignment as a
 * component assignment, not a item assignment.
 * The Repeater itself just sees a QDeclarativeComponent*, as that is what it gets passed from the QML
 * engine.
 */
//! [0]
class Repeater : public bb::cascades::CustomControl
{
    Q_OBJECT

    /**
     * The property that represents the model.
     * We use QVariant as type here so that the user can pass different type of models here.
     */
    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)

    // The declarative component that is used as 'template' for the repeated elements
    Q_PROPERTY(QDeclarativeComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)

    // Mark the 'delegate' property as default property
    Q_CLASSINFO("DefaultProperty", "delegate")

    public:
        Repeater(bb::cascades::Container *parent = 0);
        ~Repeater();

        // The accessor methods for the properties
        QVariant model() const;
        void setModel(const QVariant &model);

        QDeclarativeComponent *delegate() const;
        void setDelegate(QDeclarativeComponent *delegate);

    Q_SIGNALS:
        // The change notification signals of the properties
        void modelChanged();
        void delegateChanged();

    private:
        // This method clears the previsouly generated controls
        void clear();

        // This method regenerates new controls according to the current model data
        void regenerate();

        // The model data
        QVariant m_model;

        // The declarative component that is used as 'template' for the repeated elements
        QDeclarativeComponent *m_delegate;

        // The list of generated controls
        QList<bb::cascades::Control*> m_controls;
};
//! [0]

#endif
