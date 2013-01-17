/*!
* Copyright (c) 2012 Research In Motion Limited.
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

#include "PushHistoryDAO.hpp"

PushHistoryDAO::PushHistoryDAO()
{
}

PushHistoryDAO::~PushHistoryDAO()
{
}

bool PushHistoryDAO::createPushHistoryTable()
{
    const QString query("CREATE TABLE IF NOT EXISTS pushhistory (rownum INTEGER PRIMARY KEY AUTOINCREMENT, itemid TEXT);");

    // Execute the query.
    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

bool PushHistoryDAO::add(const PushHistoryItem &item)
{
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("INSERT INTO pushhistory (rownum, itemid) VALUES(:rownum, :itemid)");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":itemid", item.itemId());
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

bool PushHistoryDAO::removeOldest()
{
    const QString query("DELETE FROM pushhistory WHERE rownum = (SELECT min(rownum) FROM pushhistory);");

    // Execute the query.
    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

bool PushHistoryDAO::removeAll()
{
	const QString query("DROP TABLE pushhistory;");

	// Execute the query.
	QSqlQuery sqlQuery(query, SQLConnection());

	const QSqlError err = sqlQuery.lastError();

	if (err.isValid()) {
		qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
		return false;
	}

	return true;
}

PushHistoryItem PushHistoryDAO::pushHistoryItem(const QString &pushHistoryItemId)
{
    PushHistoryItem pushHistoryItem("");

    QSqlQuery sqlQuery(SQLConnection());
    const QString query("SELECT rownum, itemid FROM pushhistory WHERE itemid = :itemId;");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":itemid", pushHistoryItemId);
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
    } else if (sqlQuery.next()){
        pushHistoryItem.setSeqNum(sqlQuery.value(0).toInt());
        pushHistoryItem.setItemId(sqlQuery.value(1).toString());
    }

    return pushHistoryItem;
}

int PushHistoryDAO::pushHistoryCount()
{
    int count = -1;
    const QString query("SELECT COUNT(*) AS count FROM pushhistory;");

    // Execute the query.
    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
    } else if (sqlQuery.next()) {
        count = sqlQuery.value(0).toInt();
    }

    return count;
}
