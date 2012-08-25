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

#ifndef APP_H
#define APP_H

/*
 * Core includes
 */
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <bb/cascades/Event>
#include <bb/cascades/UiObject>
#include <bb/cascades/Control>
#include <bb/cascades/Application>
#include <QObject>
#include <QThread>

using namespace bb::cascades;

/*
 * Forward declaration of cascade types
 */
namespace bb
{
namespace cascades
{
class AbstractPane;
class Container;
class GroupDataModel;
class ListView;
class NavigationPane;
class Page;
class QmlDocument;
class TextArea;
class Button;
class ActionItem;
}
}

class App : public QObject
{
    Q_OBJECT

public:
    /*
     * Default Constructor
     */
    App();

    /*
     * Destructor
     */
    ~App();

    /*
     * Like in QT, signals and slots must be declared under 'signals' or 'slots' sections as shown below.
     */
public slots:

    /*
     * This method will be used as a slot by a button in our application.
     */
    void onButtonClicked();

private:

    /*
     * This method is used to initialize signals and slots for our application.
     */
    bool setupNavigationSignals();

    /*
     * This represents the Cascades construct for stack-like navigation between UI objects.
     */
    NavigationPane* m_Nav;

    /*
     * QML document reference for bottom pane. the BottomPane.qml reference is loaded into this variable.
     */
    QmlDocument* m_BottomPaneContentQML;


    /*
     * QML document reference for side pane. the SidePane.qml reference is loaded into this variable.
     * From this object, we are able to extract the QDeclarativeContext to access constructs defined
     * in the file (see below)
     */
    QmlDocument* m_SidePaneContentQML;

    /*
     * QML Declarative context used to allow binding between QML document and code.
     * In our application, this is required in order to allow toggling the QML navigation pane
     * via use of a button from within the code.
     */
    QDeclarativeContext* m_SidePaneQmlContext;

};

#endif /* ifndef APP_H */




