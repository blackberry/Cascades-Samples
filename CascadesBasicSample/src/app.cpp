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

/*
 * Additional code includes
 */
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/Button>
#include <bb/cascades/ActionItem>
#include <bb/cascades/ForeignWindow>
#include <bb/cascades/AbsoluteLayoutProperties>

#include "app.hpp"

/**
 * App
 *
 * In this class you will learn the following:
 * -- Setup a NavigationPane with various QML documents
 */

/**
 * App::App()
 *
 * Setup NavigationPane and set the scene
 */
App::App()
    : m_Nav(0),
      m_BottomPaneContentQML(0),
      m_SidePaneContentQML(0),
      m_SidePaneQmlContext(0)
{

    // Here we create a QML object and load it.
    QmlDocument* qml = QmlDocument::create("main.qml");
    qml->setContextProperty("cs", this);

    // The root NavigationPane container is created from QML.
    m_Nav = qml->createRootNode<NavigationPane>();

    if (m_Nav)
    {

        //Load our bottom and side panes from QML files
        m_BottomPaneContentQML = QmlDocument::create().load("BottomPane/BottomPane.qml");
        m_SidePaneContentQML = QmlDocument::create().load("SidePane/SidePane.qml");


        if (!m_SidePaneContentQML->hasErrors() && !m_BottomPaneContentQML->hasErrors())
        {

            //Establish Signals & Slots for UI components in the application
            if (setupNavigationSignals())
            {
                // Finally the main scene for the application is set to this Control.
                Application::setScene(m_Nav);

            }
        }
    }
}

App::~App()
{
    // void
}

/**
 * App::setupNavigationSignals()
 *
 * Bind our button click signal to our onButtonClicked() SLOT to trigger the SidePane animation
 */
bool App::setupNavigationSignals()
{

    if (m_Nav)
    {

        //Find our child button handle from the NavigationPane parent (See main.qml)
        Button* button1 = m_Nav->findChild<Button*>("buttonOpenPane1");

        if (button1)
        {

            //If found, connect the onButtonClicked slot to the button's "clicked()"signal.
            return (QObject::connect(button1, SIGNAL(clicked()), this,
                                     SLOT(onButtonClicked())));

        }
    }

    return false;

}

/**
 * App::onButtonClicked()
 *
 * SLOT
 * Push the SidePane document onto the NavigationPane stack
 */
void App::onButtonClicked()
{
    //Obtain a handle on the Side Pane
    //This handle will be used to toggle the side pane from the NavigationPane
    Page* sidePane = m_SidePaneContentQML->createRootNode<Page>();

    // Obtain document contexts, they are used to bind a properties for the ContentPanes.
    // and make the navigation pane and application available for QML.
    //NOTE: The name "_navPane" is used in the QML file. This is where the link is made.
    m_SidePaneQmlContext = m_SidePaneContentQML->documentContext();
    m_SidePaneQmlContext->setContextProperty("_navPane", m_Nav);

    //On button click, we will push the Side Pane in the NavigationPane, causing the slide affect.
    m_Nav->push(sidePane);

   //after push, ownership is transferred to m_nav
}

