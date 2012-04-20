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
#include "quotesapp.h"
#include "quotesdbhelper.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TextArea>

using namespace bb::cascades;

QuotesApp::QuotesApp()
{
}

QuotesApp::~QuotesApp()
{
    delete mQuotesDbHelper;
}

void QuotesApp::onStart()
{
    // Initiate the database helper object.
    mQuotesDbHelper = new QuotesDbHelper();

    if (!loadQMLScene()) {
        qWarning("Failed to load QML scene.");
    }
}

bool QuotesApp::loadQMLScene()
{

    // Here we create a QML object and load it, we are using build patterns.
    QmlDocument *qmlDocument = QmlDocument::create().load("main.qml");

    if (!qmlDocument->hasErrors()) {

        // In order to call invokable function in this object it is set as a context property.
        qmlDocument->setContextProperty("_quoteApp", this);

        // The root Container is created from QML.
        mNav = qmlDocument->createRootNode<NavigationPane>();

        if (mNav) {
            // The list containing a bunch of people that have expressed clever things
            // when it comes to programming is set up.
            mListView = setUpQuotesList();

            // Load the document for the Quotes page where content will be presented.
            QmlDocument *contentQML = QmlDocument::create().load("QuotePage/QuotePage.qml");

            if (!contentQML->hasErrors()) {

                // Get the document context, it is used to bind a property for the ContentPane.
                // and make the navigation pane and application available for QML.
                mQmlContext = contentQML->documentContext();
                mQmlContext->setContextProperty("_navPane", mNav);
                mQmlContext->setContextProperty("_quoteApp", this);

                // Initialization of the bound context properties of the content to empty strings.
                clearContentPaneData();

                mContentPage = contentQML->createRootNode<Page>();

                // Finally the main scene for the application is set to this Control.
                Application::setScene(mNav);

                return true;
            }
        }
    }

    return false;
}

ListView *QuotesApp::setUpQuotesList()
{
    ListView *listView = 0;

    // Load the quotes table from the database.
    QVariantList sqlData = mQuotesDbHelper->loadDataBase("quotes.db", "quotes");

    if (!sqlData.isEmpty()) {

        // A GroupDataModel is used for creating a sorted list.
        mDataModel = new GroupDataModel(QStringList() << "lastname" << "firstname");
        mDataModel->setParent(this);
        mDataModel->setGrouping(ItemGrouping::ByFirstChar);
        mDataModel->insert(sqlData);

        // The list view is set up in QML, here we retrieve it to connect it to
        // a data model.
        listView = mNav->findChild<ListView*>("quotesList");
        listView->setDataModel(mDataModel);

        // By connecting to the selectionChanged signal we can find out when selection has
        // changed and update the content pane information based on the selected item.
        QObject::connect(listView, SIGNAL(selectionChanged(const QVariantList, bool)), this,
                SLOT(onListSelectionChanged(const QVariantList, bool)));

        // Connect to the models item added and updated signals, since we want to 
        // select the item in the list if it has been manipulated.
        QObject::connect(mDataModel, SIGNAL(itemAdded(QVariantList)), this,
            SLOT(onModelUpdate(QVariantList)));

        QObject::connect(mDataModel, SIGNAL(itemUpdated(QVariantList)), this,
            SLOT(onModelUpdate(QVariantList)));
        
    }

    return listView;
}

void QuotesApp::onListSelectionChanged(const QVariantList indexPath, bool selected)
{

    if (selected) {

        // If the indexPath count is larger then 1 the user pressed an item within a group, if it is
        // exactly 1 a header item was pressed and we take no action.
        if(indexPath.count() > 1) {
            QVariantMap map = mDataModel->data(indexPath).toMap();

            // Update the data used in QML for presenting the quote in the content pane (see QuotesPage/ContentPane.qml).
            mQmlContext->setContextProperty("_contentPane", map);

            // Navigate to the content pane to present the quote.
            mNav->push(mContentPage);
        }
    }
}


void QuotesApp::onModelUpdate (QVariantList indexPath )
{
    //mListView->scrollToItem(indexPath);
    mListView->select(indexPath, true);
}

void QuotesApp::addNewRecord(const QString &firstName, const QString &lastName,
        const QString &quote)
{
    QVariantMap map;
    map["firstname"] = QString(firstName);
    map["lastname"] = QString(lastName);
    map["quote"] = QString(quote);

    // Add the new item to the database, the insert id is the unique database id which
    // is used for edit and delete functionality.
    QVariant insertId = mQuotesDbHelper->insert(map);

    if (!insertId.isNull()) {
        map["id"] = insertId;
        mDataModel->insert(map);
    }
}

void QuotesApp::updateSelectedRecord(const QString &firstName, const QString &lastName,
        const QString &quote)
{
    // Get the item at the selected index, the item that has been updated.
    QVariantList indexPath = mListView->selected();

    if (!indexPath.isEmpty()) {
        // Get the data item for the selected item so it can be updated.
        QVariantMap itemMapAtIndex = mDataModel->data(indexPath).toMap();

        // Update the data.
        itemMapAtIndex["firstname"] = QString(firstName);
        itemMapAtIndex["lastname"] = QString(lastName);
        itemMapAtIndex["quote"] = QString(quote);

        // Call the database helper to update the item data and update the model item.
        mQuotesDbHelper->update(itemMapAtIndex);
        mDataModel->updateItem(indexPath, itemMapAtIndex);
    }
}

void QuotesApp::deleteRecord()
{
    QVariantList indexPath = mListView->selected();

    if (!indexPath.isEmpty()) {
        QVariantMap map = mDataModel->data(indexPath).toMap();

        // Delete the item from the database based on id, if successful remove it
        // from the model (which will remove the data from the list).
        if (mQuotesDbHelper->deleteById(map["id"])) {

            // Before the item is removed, we store how many items there are in the
            // category that the item is removed from, we need this to select a new item.
            QVariantList categoryIndexPath;
            categoryIndexPath.append(indexPath.first());
            int childrenInCategory = mDataModel->childCount(categoryIndexPath);

            mDataModel->remove(map);

            // After removing the selected item we want another quote to be shown,
            // here we select the quote that is closest to the removed one in the list.
            if( childrenInCategory > 1) {

                // If the Category still has items select within the category.
                int itemInCategory = indexPath.last().toInt();

                if(itemInCategory < childrenInCategory - 1) {
                    mListView->select(indexPath);
                } else {
                    // The last item in the category was removed select the new last item.
                    indexPath.replace(1, QVariant(itemInCategory - 1));
                    mListView->select(indexPath);
                }
            } else {
                // If no items left in the category, move to the next category, if there are no more
                // categories below, select the previous, if no items left at all navigate to the list.
                QVariantList lastIndexPath = mDataModel->last();

                if(lastIndexPath.isEmpty()) {
                    mNav->pop();
                    return;
                }

                if(indexPath.first().toInt() <= lastIndexPath.first().toInt()) {
                    mListView->select(indexPath);
                } else {
                    mListView->select(mDataModel->last());
                }
            }
        }
    }
}

void QuotesApp::clearContentPaneData()
{
    QVariantMap emptyItem;
    emptyItem["firstname"] = QString("");
    emptyItem["lastname"] = QString("");
    emptyItem["quote"] = QString("");

    // Update the content pane information.
    mQmlContext->setContextProperty("_contentPane", emptyItem);
}
