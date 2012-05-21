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
#include "quotesdbhelper.h"

using namespace bb::data;

QuotesDbHelper::QuotesDbHelper()
{

}

QuotesDbHelper::~QuotesDbHelper()
{
    if (mDb.isOpen()) {
        QSqlDatabase::removeDatabase("app/native/assets/sql/quotes.db");
        mDb.removeDatabase("QSQLITE");
    }
}

QVariantList QuotesDbHelper::loadDataBase(const QString databaseName, const QString table)
{

    // Load data base entries using a SQL data access object into a QVariantList
    // which can be used in a group data model to present a sorted list.
    QString dbNameWithPath = "app/native/assets/sql/" + databaseName;
    
    // Sets up a data access object.
    SqlDataAccess sqlDataAccess(dbNameWithPath);

    // Set the query to load all entries in the table and execute load.
    sqlDataAccess.setQuery("select * from " + table);
    QVariantList sqlData = sqlDataAccess.load().value<QVariantList>();

    if (sqlDataAccess.hasError()) {
        DataAccessError* err = sqlDataAccess.error();
        qWarning() << "SQL error: type=" << err->errorType() << ": " << err->errorMessage();
        return sqlData;
    }

    // Open the data base to enable edit/add/remove via SQL queries, using non default
    // connection to not conflict with the database connection set up by SqlDataAccess.
    mDb = QSqlDatabase::addDatabase("QSQLITE", "database_helper_connection");
    mDb.setDatabaseName(dbNameWithPath);

    if (!mDb.isValid()) {
        qWarning() << "Could not set data base name probably due to invalid driver.";
        return sqlData;
    }

    bool success = mDb.open();

    if (!success) {
        qWarning() << "Could not open database we are in trouble";
        return sqlData;
    }

    // Store the name of the table, this is used in the insert/update/delete functions.
    mTable = table;

    return sqlData;
}

bool QuotesDbHelper::deleteById(QVariant id)
{

    if (id.canConvert(QVariant::String)) {
        QString query = "DELETE FROM " + mTable + " WHERE id=" + id.toString();
        return queryDatabase(query);
    }

    qWarning() << "Failed to delete item with id: " << id;

    return false;
}

QVariant QuotesDbHelper::insert(QVariantMap map)
{

    QSqlQuery sqlQuery(mDb);

    // The Queries are hardcoded for the quotes database. We use the prepare version of
    // the Qt SDK API's since they are safer then constructing queries ourselves.
    // For example a ' sign inside a quote is tricky to handle if one does not bind
    // the values like this.
    sqlQuery.prepare("INSERT INTO " + mTable + " (firstname, lastname, quote)"
            "VALUES(:firstName, :lastName, :quote)");

    sqlQuery.bindValue(":firstName", map["firstname"]);
    sqlQuery.bindValue(":lastName", map["lastname"]);
    sqlQuery.bindValue(":quote", map["quote"]);
    sqlQuery.exec();

    QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "SQL reported an error : " << err.text();
    }

    return sqlQuery.lastInsertId();
}

bool QuotesDbHelper::update(QVariantMap map)
{
    QSqlQuery sqlQuery(mDb);

    // Query hard coded for the quotes database.
    sqlQuery.prepare(
            "UPDATE " + mTable + " SET firstname=:firstName, lastname=:lastName, quote=:quote WHERE id=:id");

    sqlQuery.bindValue(":firstName", map["firstname"]);
    sqlQuery.bindValue(":lastName", map["lastname"]);
    sqlQuery.bindValue(":quote", map["quote"]);
    sqlQuery.bindValue(":id", map["id"].toString());
    sqlQuery.exec();

    QSqlError err = sqlQuery.lastError();

    if (!err.isValid()) {
        return true;
    }

    qWarning() << "SQL reported an error : " << err.text();

    return false;
}

bool QuotesDbHelper::queryDatabase(const QString query)
{

    // Execute the query.
    QSqlQuery sqlQuery(query, mDb);

    QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "SQL reported an error for query: " << query << " error: "
                << mDb.lastError().text();
        return false;
    }

    return true;
}
