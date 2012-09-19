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
        class GroupDataModel;
        class ListView;
        class NavigationPane;
    }
}

class QuotesDbHelper;

/**
 * QuotesApp Description:
 *
 * QuotesApp is a sample application that shows you how to deal with dynamic data in
 * a list. It displays a list with some of our favorite sayings from clever
 * people when it comes to programming and technology. The list of quotes
 * is stored in an SQL database, but you will also be able to add your own quotes or
 * delete the ones you don't agree with (or edit them to better fit you purpose).
 *
 * You will learn how to:
 * - Set up a ListView with predefined item visuals
 * - Load data with the SqlDataAccess
 * - Sorting of with the help of a GroupDataModel
 * - Adding, updating and deleting items from a list
 */
class QuotesApp: public QObject
{
Q_OBJECT

public:

    // This is our constructor that sets up the recipe.
    QuotesApp();
    ~QuotesApp();

    /**
     * This is the application start-up function that 
     * initiates the database helper object and load the QML scene
     */
    void onStart();

    /**
     * This Invokable function adds a new quote to the database and updates the ListView.
     *
     * @param firstName First name of the person behind the quote (can be empty)
     * @param lastName Last name of the person behind the quote (mandatory)
     * @param quote The actual quote (can be empty)
     */
    Q_INVOKABLE
    void addNewRecord(const QString &firstName, const QString &lastName, const QString &quote);

    /**
     * This Invokable function is called from QML when the user wants to update a quote.
     *
     * @param firstName First name of the person behind the quote (can be empty)
     * @param lastName Last name of the person behind the quote (mandatory)
     * @param quote The actual quote (can be empty)
     */
    Q_INVOKABLE
    void updateSelectedRecord(const QString &firstName, const QString &lastName,
            const QString &quote);
    /**
     * This Invokable function that is called from QML when the user wants to delete a quote.
     */
    Q_INVOKABLE
    void deleteRecord();

private:

    /**
     * This function loads the QML and sets the application scene.
     *
     * @return True if all set up was successful, otherwise false is returned.
     */
    bool loadQMLScene();

    // This Database helper is used for loading items and performing updates in the database.
    QuotesDbHelper *mQuotesDbHelper;

    // This GroupDataModel is here the SQL data will be stored.
    GroupDataModel *mDataModel;

    // The Quote list
    ListView *mListView;
};

#endif // ifndef QUOTESAPP_H
