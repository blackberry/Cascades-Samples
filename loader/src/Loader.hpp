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

#ifndef LOADER_HPP
#define LOADER_HPP

#include <bb/cascades/Container>
#include <bb/cascades/CustomControl>

/**
 * The Loader class is a custom Cascades control that embeds a QDeclarativeComponent.
 * The component can either be an external QML file or a QtQuick.Component declared inside the
 * QML file where the Loader is used.
 * The embedded component can be loaded/unloaded at runtime and allows therefor to unitize the UI
 * in smaller parts.
 */
//! [0]
class Loader : public bb::cascades::CustomControl
{
    Q_OBJECT
    Q_ENUMS(Status)

    // The URL to an external QML file that is used as embedded component
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

    // An internal component that is used as embedded component
    Q_PROPERTY(QDeclarativeComponent *sourceComponent READ sourceComponent WRITE setSourceComponent RESET resetSourceComponent NOTIFY sourceChanged)

    // The Cascades control that represents the embedded component
    Q_PROPERTY(bb::cascades::Control* control READ control NOTIFY controlChanged)

    // The current status of the loader
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

    // The loading progress of the embedded component
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

public:
    /**
     * Describes the status of the Loader
     */
    enum Status
    {
        Null,     ///< No embedded component is loaded
        Ready,    ///< The embedded component has been loaded successfully
        Loading,  ///< The embedded component is still loading
        Error     ///< The embedded component could not be loaded
    };

    Loader(bb::cascades::Container *parent = 0);
    ~Loader();

    // The accessor methods for the properties
    QUrl source() const;
    void setSource(const QUrl &url);

    QDeclarativeComponent *sourceComponent() const;
    void setSourceComponent(QDeclarativeComponent *component);
    void resetSourceComponent();

    Status status() const;
    qreal progress() const;

    bb::cascades::Control *control() const;

Q_SIGNALS:
    // The change notification signals for the properties
    void controlChanged();
    void sourceChanged();
    void statusChanged();
    void progressChanged();

    // This signal is emitted when the embedded component has been loaded
    void loaded();

private Q_SLOTS:
    // This slot is invoked when the declarative component has been loaded
    void sourceLoaded();

private:
    // A helper method to clean up a previously loaded component
    void clear();

    // A helper method that does the actual loading of the embedded component
    void load();

    // The URL to an external QML file that is used as embedded component
    QUrl m_source;

    // The Cascades control that represents the embedded component
    bb::cascades::Control *m_control;

    // The actual declarative component that has been loaded
    QDeclarativeComponent *m_component;

    // A flag that defines whether this class owns the declarative component
    bool m_ownsComponent;
};
//! [0]

#endif
