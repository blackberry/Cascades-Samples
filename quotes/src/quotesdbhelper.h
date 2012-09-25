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
 * QuotesDbHelper Description:
 *
 * This is a helper class for SQL database handling in the Quotes Application.
 * The class has functionality for loading a database table in a format
 * that can be used by a GroupDataModel, which is an easy way of populating
 * a list in Cascades. There is further functionality for inserting,
 * deleting, and updating database entries. This class is not a generic
 * and is intended to handle the quotes data.
 */
class QuotesDbHelper
{
public:
    QuotesDbHelper();
    ~QuotesDbHelper();

    /**
     * This function loads all the entries in a specific table using SqlDataAccess which
     * is designed to help setup a data structure for a GroupDataModel ( which can can be used
     * to populate a list in Cascades).
     *
     * @param table The name of the table.
     * @param databaseName The name of the database (in assets/sql).
     * @return A list set up to be added to a GroupDataModel.
     */
    QVariantList loadDataBase(const QString databaseName, const QString table);

    /**
     * This function deletes an entry from the database based on ID.
     *
     * @param id The ID of the entry that is to be deleted.
     * @return True on success, false on failure
     */
    bool deleteById(QVariant id);

    /**
     * This function inserts a data entry into the database. The ID key
     * of the map is used as the unique primary key for the entry.
     *
     * @param map Data entry which should be added
     * @return True on success, false on failure
     */
    QVariant insert(QVariantMap map);

    /**
     * This function updates an entry in the table. The entry key 
     * is stored in the ID key of the map.
     *
     * @param map Data entry that should be updated.
     * @return True on success, false on failure
     */
    bool update(QVariantMap map);

private:
    /**
     * In order to write to a file in a signed application, the file must
     * reside in the apps data folder(assets). This function copies the bundled
     * database file to that folder.
     *
     * @param databaseName The name of the database (in assets/sql).
     * @return True on success, false on failure
     */
    bool copyDbToDataFolder(const QString databaseName);

    /**
     * This helper function is for running a specific query on the database.
     *
     * @param query The hardcoded query to run.
     * @return True on success, false on failure
     */
    bool queryDatabase(const QString query);

    // The database is opened in the loadDatabase function.
    QSqlDatabase mDb;

    // The name of the table loaded in the loadDatabase function
    QString mTable;
    QString mDbNameWithPath;
};

#endif // ifndef _QUOTESDBHELPER_H_
