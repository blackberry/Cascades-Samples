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
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/Page>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/StackListLayout>
#include <bb/cascades/Button>

using namespace bb::cascades;

/**
 * This application shows how to dynamically add list items to a list view created
 * by QML or C++.
 */


/**
 * Constructor
 *
 * Initial Setup:
 * --expose this c++ app object to the QML view so that it can invoke the methods of this object.
 * --create a ListView by C++ code, then add it to the UI view.
 *
 */
App::App()
    : m_cppListDataModel()
    , m_qmlListDataModel()
    , m_navPane(NULL)
    , m_cppListView(NULL)
{
    QmlDocument* qml = QmlDocument::create("asset:///Main.qml");

    if (!qml->hasErrors())
    {
        // The application NavigationPane is created from QML.
        m_navPane = qml->createRootObject<NavigationPane>();

        // set out scroll app context property so we can access native functions from QML
        qml->setContextProperty("_scrollApp", this);

        // get the data model for the qml scroll list
        setupQmlListModelData();

        //set up the topChanged signal for the navigation pane
        connect(m_navPane, SIGNAL(topChanged(bb::cascades::Page*)), this, SLOT(onTopChanged(bb::cascades::Page*)));

        // We create a ListView and it's content.
        setupCppListView();

        // Create the application scene and we are done.
        Application::instance()->setScene(m_navPane);
    }
}

/**
 * void App::setupCppListView()
 *
 * Setup a list view purely by C++ code, then add it to the "cppcontainer" located at the UI.
 *
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
    innerContainer->setTopPadding(15.0f);
    innerContainer->setBottomPadding(15.0f);
    innerContainer->setLayout(layout);
    innerContainer->setHorizontalAlignment(HorizontalAlignment::Center);

    // Setup and create the content view and the list.
    m_cppListView = createCppListView();
    m_cppListView->setHorizontalAlignment(HorizontalAlignment::Center);

    // Add the controls.
    innerContainer->add(m_cppListView);

    listContainer->add(innerContainer);
}

/**
 * void App::setupQmlListModelData()
 *
 * Setup the data model for the list view defined by QML
 */
void App::setupQmlListModelData()
{
    // get the qml listview from the nav pane
    m_qmlListView = m_navPane->findChild<ListView*>("itemList");

    // create the data model for the list view
    // We populate a QVariant map with very simple data just to demonstrate
    addItems(m_qmlListDataModel, 0, 10);

    // add an extra element that will be used for loading more items into the list
    QVariantMap map = QVariantMap();
    map["title"] = QString("+");
    map["image"] = QString("asset:///images/white_photo.png");
    m_qmlListDataModel.append(map);

    // setting the data created above to the list so the list will be populated with our data
    m_qmlListView->setDataModel(&m_qmlListDataModel);
}

/**
 * ListView* App::createCppListView()
 *
 * A Helper function to create a list view by C++
 */
ListView* App::createCppListView()
{
    // Setting up a stacklist layout so the scroller will place elements horizontally
    StackListLayout* sll = new StackListLayout();
    sll->setOrientation(LayoutOrientation::LeftToRight);
    ListView* listView = new ListView();
    listView->setLayout(sll);
    // The list item factory will be used to create the list items
    ListItemFactory* listItemManager = new ListItemFactory();

    // populate the cpp list with data
    addItems(m_cppListDataModel, 0, 10);

    // add an extra element that will be used for loading more items into the list
    QVariantMap map = QVariantMap();
    map["title"] = QString("+");
    map["image"] = QString("asset:///images/white_photo.png");
    m_cppListDataModel.append(map);

    // setting the data created above to the list so the list will be populated with our data
    listView->setDataModel(&m_cppListDataModel);
    // setting the item manager used to create the list items
    listView->setListItemProvider(listItemManager);

    // set how we want the list view to be placed in it's parent container
    listView->setLayoutProperties(StackLayoutProperties::create());
    listView->setVerticalAlignment(VerticalAlignment::Top);

    // Connect to the selection changed signal which allows
    // you to do an action when the selection in the list changes
    connect(listView, SIGNAL(selectionChanged(const QVariantList, bool)), this,
            SLOT(onSelectionChanged(const QVariantList, bool)));

    return listView;
}

/**
 * void App::onTopChanged(bb::cascades::AbstractPane* pane)
 *
 * The handler for the signal of "topChanged".
 */
void App::onTopChanged(bb::cascades::Page* page)
{
    if (page != NULL)
    {
        // Reset list selection when the top Container is not the Content Page.
        m_cppListView->clearSelection();
    }
}

/**
 * void App::onSelectionChanged(const QVariantList indexPath, bool selected)
 *
 * The handler for the signal of selection changes for the items in the cpp list view
 */
void App::onSelectionChanged(const QVariantList indexPath, bool selected)
{
    if (selected)
    {
        // Get the selected item title.
        QVariantMap map = m_cppListDataModel.data(indexPath).toMap();
        QString title = map.value("title").toString();

        // We look for a +, which is the special element that will add 10 more items
        if (title.compare("+") == 0)
        {
            addItems(m_cppListDataModel, m_cppListDataModel.size() - 1, 10);
            m_cppListView->clearSelection();
        }
    }
}

/**
 * void App::addItems(int index)
 *
 * add (10) items to the listView defined by QML
 *
 */
void App::addItems(int index)
{
    addItems(m_qmlListDataModel, index, 10);
}

/**
 * void App::addItems(bb::cascades::QVariantListDataModel& dataModel, int insertIndex, int numElementsToInsert)
 *
 * a helper function to add given number of items to the given data model
 */
void App::addItems(bb::cascades::QVariantListDataModel& dataModel, int insertIndex, int numElementsToInsert)
{
    QVariantMap map = QVariantMap();
    for (int i = 0; i < numElementsToInsert; ++i)
    {
        // buffer overrun protection
        if (insertIndex + i > 9999)
        {
            Q_ASSERT(false);
            return;
        }

        char title[5];
        sprintf(title, "%i", insertIndex + i);
        map["title"] = QString(title);
        map["image"] = QString("asset:///images/white_photo.png");
        dataModel.insert(insertIndex + i, map);
    }
}
