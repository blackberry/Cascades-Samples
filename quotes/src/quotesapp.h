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
#ifndef QUOTESAPP_H
#define QUOTESAPP_H

#include <bb/cascades/Application>
#include <bb/cascades/DataModel>
#include <bb/data/SqlDataAccess>
#include <QObject>

using namespace bb::cascades;
using namespace bb::data;

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
    }
}

class QuotesDbHelper;

/**
 * QuotesApp
 *
 * Quotes is an application that shows you how to deal with dynamic data in
 * a list. It displays a list with some of our favorite sayings from clever
 * people when it comes to programming and technology. The list of quotes
 * is set stored in an SQL database, but you will also be able to add your own quotes,
 * delete the once you don't agree with (or edit them to better fit you purpose).
 *
 * You will learn how to:
 * - Set up a ListView with predefined item visuals.
 * - Load data with the SqlDataAccess
 * - Sorting of with the help of a GroupDataModel
 * - Adding, updating and deleting items from a list.
 */
class QuotesApp: public QObject
{
Q_OBJECT

public:
    QuotesApp();
    ~QuotesApp();

    /**
     * Application start up function, initiates the data base helper
     * object and loads the QML scene;
     */
    void onStart();

    /**
     * Function that adds a new quote to the database and updates the ListView.
     *
     * @param firstName first name of the person behind the quote (can be empty)
     * @param lastName last name of the person behind the quote (mandatory)
     * @param quote the actual quote (can be empty)
     */
    Q_INVOKABLE
    void addNewRecord(const QString &firstName, const QString &lastName, const QString &quote);

    /**
     * Function that is called from QML when the user wants to update a quote.
     *
     * @param firstName first name of the person behind the quote (can be empty)
     * @param lastName last name of the person behind the quote (mandatory)
     * @param quote the actual quote (can be empty)
     */
    Q_INVOKABLE
    void updateSelectedRecord(const QString &firstName, const QString &lastName,
            const QString &quote);
    /**
     * Function that is called from QML when the user wants to delete a quote.
     *
     */
    Q_INVOKABLE
    void deleteRecord();


private slots:

    /**
     * Slot function for selection in list, in this application this is where
     * the data binding of the Content Panes data is done.
     *
     * @param indexPath Index path to the item.
     * @param active True if the new state is selected, otherwise false.
     */
    void onListSelectionChanged(const QVariantList indexPath, bool selected);

     /**
     * Slot function for model updates, used when adding and updating items
     * in order to select the corresponding item in the list view.
     *
     * @param indexPath Index path to the item.
     */
    void onModelUpdate (QVariantList indexPath );
private:

    /**
     * Loads the QML and sets the application's scene.
     *
     * @return true if all set up was successful otherwise false is returned.
     */
    bool loadQMLScene();

    /**
     * Helper function for initializing a ListView defined in QML. Data is loaded
     * from a SQL data base and populates a GroupDataModel which in turn is used by 
     * the list DataModel.
     *
     * @param rootContainer
     * @return the ListView if all set up was successful otherwise 0
     */
    ListView *setUpQuotesList();

    /**
     * A function that clears the data bindings to the information to the ContentPane.
     */
    void clearContentPaneData();

    // Data base helper, used for loading items and performing updates in the data base.
    QuotesDbHelper *mQuotesDbHelper;

    // GroupDataModel, where the sql data will be stored.
    GroupDataModel *mDataModel;

    // The declarative context, used to bind properties so they are available in QML.
    QDeclarativeContext *mQmlContext;

    // The Quote list.
    ListView *mListView;

    // NavigationPane used for drill down in the list.
    NavigationPane *mNav;

    // The Page where the details quote is shown.
    Page *mContentPage;
};

#endif // ifndef QUOTESAPP_H
