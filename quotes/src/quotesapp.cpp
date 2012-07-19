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

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/QmlDocument>

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
        NavigationPane* navigationPane = qmlDocument->createRootNode<NavigationPane>();

        if (navigationPane) {

            // Load the quotes table from the database.
            QVariantList sqlData = mQuotesDbHelper->loadDataBase("quotes.db", "quotes");

            if (!sqlData.isEmpty()) {
                // The list containing a bunch of people that have expressed clever things
                // when it comes to programming is set up.
                // A GroupDataModel is used for creating a sorted list, the sorting
                // properties are set in QML here the data is loaded.
                mDataModel = navigationPane->findChild<GroupDataModel*>("quotesModel");
                mDataModel->insertList(sqlData);

                // The list view is set up in QML, here we retrieve it to be used
                // when updating the selected item in the deleteRecord funciton.
                mListView = navigationPane->findChild<ListView*>("quotesList");
            }

            // Finally the main scene for the application is set to this Control.
            Application::instance()->setScene(navigationPane);
            return true;
        }
    }

    return false;
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

            // Delete is the only operation where the logics for updating which item
            // is selected is handled in code.
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

                if (!lastIndexPath.isEmpty()) {
                    if (indexPath.first().toInt() <= lastIndexPath.first().toInt()) {
                        mListView->select(indexPath);
                    } else {
                        mListView->select(mDataModel->last());
                    }
                }
            }
        }
    }
}
