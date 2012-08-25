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

#include "App.hpp"
#include "ListItemFactory.hpp"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ActionItem>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/StackListLayout>

using namespace bb::cascades;

/**
 * This app demonstrate how to construct a list view by QML or C++
 */


/**
 *  Constructor
 *
 *  Initial setup so that C++ code can access the components defined in the UI by QML.
 *
 *  also, construct a List View by C++ code to add it to the UI.
 */
App::App()
    : m_listDataModel()
    , m_navPane(NULL)
    , m_listView(NULL)
{
    QmlDocument* qml = QmlDocument::create(this, "Main.qml");

    if (!qml->hasErrors())
    {
        // The application NavigationPane is created from QML.
        m_navPane = qml->createRootNode<NavigationPane>();
        //set up the topChanged signal for the navigation pane
        connect(m_navPane, SIGNAL(topChanged(bb::cascades::AbstractPane*)), this, SLOT(onTopChanged(bb::cascades::AbstractPane*)));

        // We create a ListView and it's content.
        setupCppListView();

        // Create the application scene and we are done.
        Application::setScene(m_navPane);
    }
}

/**
 * void App::setupCppListView()
 *
 * Set up a List view by C++, then add it to the UI to display.
 */
void App::setupCppListView()
{
    // The list view created in c++ will be added to an empty container in the QML
    Container* listContainer = m_navPane->findChild<Container*>("cppContainer");

    // The background of the UI is set up by coloring the background Container,
    // two decoration images at the top and bottom and a paper crease creating the illusion of a book.
    listContainer->setBackground(Color::fromARGB(0xff3a8485));

    // Create a Container for the list and the content view
    Container* innerContainer = new Container();
    DockLayout* layout = new DockLayout();
    layout->setTopPadding(15.0f);
    layout->setBottomPadding(15.0f);
    innerContainer->setLayout(layout);
    innerContainer->setLayoutProperties(
        DockLayoutProperties::create().horizontal(HorizontalAlignment::Center));

    // Setup and create the content view and the list.
    m_listView = createCppListView();
    m_listView->setLayoutProperties(DockLayoutProperties::create().horizontal(HorizontalAlignment::Center));

    // Add the controls.
    innerContainer->add(m_listView);

    listContainer->add(innerContainer);
}
/**
 * ListView* App::createCppListView()
 *
 * A helper function to create a list view with populated list items.
 *
 */
ListView* App::createCppListView()
{
    // Setting up a stacklist layout so the scroller will place elements horizontally
    StackListLayout* sll = new StackListLayout();
    sll->setLayoutDirection(LayoutDirection::LeftToRight);
    ListView* listView = new ListView();
    listView->setLayout(sll);
    // The list item factory will be used to create the list items
    ListItemFactory* listItemManager = new ListItemFactory();

    // We populate a QVariant map with very simple data just to demonstrate
    QVariantMap map = QVariantMap();
    for (int i = 0; i < 25; ++i)
    {
        char title[3];
        sprintf(title, "%i", i);
        map["title"] = QString(title);
        map["image"] = QString("asset:///images/white_photo.png");
        m_listDataModel << map;
    }

    // setting the data created above to the list so the list will be populated with our data
    listView->setDataModel(&m_listDataModel);
    // setting the item manager used to create the list items
    listView->setListItemManager(listItemManager);

    // set how we want the list view to be placed in it's parent container
    listView->setLayoutProperties(StackLayoutProperties::create().vertical(VerticalAlignment::Top));

    // Connect to the selection changed signal which allows
    // you to do an action when the selection in the list changes
    connect(listView, SIGNAL(selectionChanged(const QVariantList, bool)), this,
            SLOT(onSelectionChanged(const QVariantList, bool)));

    return listView;
}

/**
 * void App::onTopChanged(bb::cascades::AbstractPane* pane)
 *
 * The handler for the "tapChanged" signal
 */
void App::onTopChanged(bb::cascades::AbstractPane* pane)
{
    if (pane != NULL)
    {
        // Reset list selection when the top Container is not the Content Page.
        m_listView->clearSelection();
    }
}

/**
 * void App::onSelectionChanged(const QVariantList indexPath, bool selected)
 *
 * The handler for the signal indicating an selection change happened.
 */
void App::onSelectionChanged(const QVariantList indexPath, bool selected)
{
    Q_UNUSED(indexPath);

    if (selected)
    {
        // do something with the selected
    }
}
