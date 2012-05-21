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
#ifndef _QUOTESDBHELPER_H_
#define _QUOTESDBHELPER_H_

#include <QtSql/QtSql>
#include <bb/data/SqlDataAccess>

using namespace bb::data;

/**
 * QuotesDbHelper
 *
 * Helper class for SQL database handling in the Quotes Application.
 * The class has functionality for loading a database table in a format
 * that can be used by a GroupDataModel which is an easy way of populating
 * a list in Cascades. There is further functionality for inserting,
 * deleting and updating data base entries. This class is not generic
 * and it's purpose is to handle the quotes data.
 */
class QuotesDbHelper
{
public:
    QuotesDbHelper();
    ~QuotesDbHelper();


    /**
     * Loads all the entries in a specific table using a SqlDataAccess which
     * is suited for setting up a data structure for a GroupDataModel that can be used
     * to populate a list in Cascades.
     *
     * @param table the name of the table.
     * @param databaseName the name of the data base (in assets/sql).
     * @return a list set up to be added to a GroupDataModel.
     */
    QVariantList loadDataBase(const QString databaseName, const QString table);

    /**
     * Deletes an entry from the database based on id.
     *
     * @param id the id of the entry that is to be deleted.
     * @return true on success false on failure
     */
    bool deleteById(QVariant id);

    /**
     *
     * @param map data entry which should be added.
     * @return
     */
    QVariant insert(QVariantMap map);

    /**
     * Update function executes an update on the table the entry that
     * is updated is decided by the id key in the map.
     *
     * @param map data entry that should be updated.
     * @return true on success false on failure
     */
    bool update(QVariantMap map);

private:
    /**
     * In order to write to a file in a signed application the file has
     * to reside in the apps data folder. This function copies the bundled
     * data base file to that folder.
     *
     * @param databaseName the name of the data base (in assets/sql).
     */
    bool copyDbToDataFolder(const QString databaseName);

    /**
     * Helper function for posting a specific query to the database.
     */
    bool queryDatabase(const QString query);

    // The database opened in the loadDatabase function.
    QSqlDatabase mDb;

    // The name of the table loaded in the loadDatabase function.
    QString mTable;
    QString mDbNameWithPath;
};

#endif // ifndef _QUOTESDBHELPER_H_
