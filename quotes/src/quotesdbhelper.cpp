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
        QSqlDatabase::removeDatabase(mDbNameWithPath);
        mDb.removeDatabase("QSQLITE");
    }
}

bool QuotesDbHelper::copyDbToDataFolder(const QString databaseName)
{
    // Since we need read and write access to the database, it has
    // to be moved to a folder where we have access to it. First
    // we check if the file already exists (previously copied).
    QString dataFolder = QDir::homePath();
    QString newFileName = dataFolder + "/" + databaseName;
    QFile newFile(newFileName);

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        // You should note that on a debug build, you will be able to write to a database
        // in the assets folder. However, for a signed application, it is not possible to 
        // write to the database in the assets folder.
        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/sql/" + databaseName;
        QFile originalFile(originalFileName);

        if (originalFile.exists()) {
            return originalFile.copy(newFileName);
        } else {
            qDebug() << "Failed to copy file data base file does not exists.";
            return false;
        }

    }

    return true;
}

QVariantList QuotesDbHelper::loadDataBase(const QString databaseName, const QString table)
{
    QVariantList sqlData;

    if (copyDbToDataFolder(databaseName)) {

        // Load database entries using a SqlDataAccess object into a QVariantList
        // which can be used in a GroupDataModel to present a sorted list.
        mDbNameWithPath = "data/" + databaseName;

        // Set up a SqlDataAccess object.
        SqlDataAccess sqlDataAccess(mDbNameWithPath);

        // Set a query to obtain all entries in the table and load into our QVariantList.
        sqlData = sqlDataAccess.execute("select * from " + table).value<QVariantList>();

        if (sqlDataAccess.hasError()) {
            DataAccessError err = sqlDataAccess.error();
            qWarning() << "SQL error: type=" << err.errorType() << ": " << err.errorMessage();
            return sqlData;
        }

        // Open the database to enable update/insert/delete functionality (via SQL queries) using 
        // a non-default connection, so we don't conflict with the database connection already setup by SqlDataAccess.
        mDb = QSqlDatabase::addDatabase("QSQLITE", "database_helper_connection");
        mDb.setDatabaseName(mDbNameWithPath);

        if (!mDb.isValid()) {
            qWarning() << "Could not set data base name probably due to invalid driver.";
            return sqlData;
        }

        bool success = mDb.open();

        if (!success) {
            qWarning() << "Could not open database we are in trouble";
            return sqlData;
        }

        // Store the name of the table (used in the insert/update/delete functions).
        mTable = table;
    }

    return sqlData;
}

bool QuotesDbHelper::deleteById(QVariant id)
{
    // Query for deleting an entry in the table which is hardcoded for the quotes database.
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

    // Query for inserting an entry in the table which hardcoded for the quotes database. 
    // We are using the prepare() version of Qt SDK API since it are safer than constructing
    // queries ourselves. For example, a single quote (') inside a quote (") is tricky to
    // handle if you don't bind the values using the prepare() function.
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

    // Query for updating an entry in the table which is hardcoded for the quotes database.
    sqlQuery.prepare(
            "UPDATE " + mTable
                    + " SET firstname=:firstName, lastname=:lastName, quote=:quote WHERE id=:id");

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
