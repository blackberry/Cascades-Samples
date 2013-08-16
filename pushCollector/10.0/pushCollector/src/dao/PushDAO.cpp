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

#include "PushDAO.hpp"
#include <QDebug>

PushDAO::PushDAO()
{
}

PushDAO::~PushDAO()
{
}

bool PushDAO::createPushTable()
{
    const QString query("CREATE TABLE IF NOT EXISTS push (seqnum INTEGER PRIMARY KEY AUTOINCREMENT, pushdate TEXT, type TEXT, pushtime TEXT, extension TEXT, content BLOB, unread INTEGER);");

    // Execute the query.
    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

int PushDAO::add(const Push &push)
{
    int insertId = -1;
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("INSERT INTO push (seqnum, pushdate, type, pushtime, extension, content, unread)"
                        "VALUES(:seqnum, :pushdate, :type, :pushtime, :extension, :content, :unread)");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":pushdate", push.pushDateAsString());
    sqlQuery.bindValue(":type", push.contentType());
    sqlQuery.bindValue(":pushtime", push.pushTime());
    sqlQuery.bindValue(":extension", push.fileExtension());
    sqlQuery.bindValue(":content", push.content().toBase64(), QSql::In | QSql::Binary);
    sqlQuery.bindValue(":unread", push.unread());
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
    } else {
        if (sqlQuery.lastInsertId().isValid()) {
            insertId = sqlQuery.lastInsertId().toInt();
        }
    }

    return insertId;
}

bool PushDAO::remove(int pushSeqNum)
{
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("DELETE FROM push WHERE seqnum = :seqNum;");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":seqNum", pushSeqNum);
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

bool PushDAO::removeAll()
{
    const QString query("DROP TABLE push;");

    // Execute the query.
    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

Push PushDAO::push(int pushSeqNum)
{
    Push push;
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("SELECT seqnum, pushdate, type, pushtime, extension, content, unread FROM push WHERE seqnum = :seqNum;");

    sqlQuery.prepare(query);
    sqlQuery.bindValue(":seqNum", pushSeqNum);
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
    } else {
        if (sqlQuery.next()){
            push = retrievePush(sqlQuery);
        }
    }

    return push;
}

QVariantList PushDAO::pushes()
{
    QVariantList data;

    const QString query("SELECT seqnum, pushdate, type, pushtime, extension, content, unread FROM push ORDER BY seqnum desc;");

    QSqlQuery sqlQuery(query, SQLConnection());

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
    } else {
        while (sqlQuery.next()){
            data.append(retrievePush(sqlQuery).toMap());
        }
    }

    return data;
}

Push PushDAO::retrievePush(const QSqlQuery& sqlQuery)
{
    return Push(sqlQuery.value(0).toInt(),QByteArray::fromBase64(sqlQuery.value(5).toByteArray()),
            sqlQuery.value(2).toString(),sqlQuery.value(4).toString(), sqlQuery.value(1).toString(),
            sqlQuery.value(3).toString(), sqlQuery.value(6).toBool());
}

bool PushDAO::markAsRead(int pushSeqNum)
{
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("UPDATE push SET unread = :unread WHERE seqnum = :seqNum;");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":unread", false);
    sqlQuery.bindValue(":seqNum", pushSeqNum);
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}

bool PushDAO::markAllAsRead()
{
    QSqlQuery sqlQuery(SQLConnection());

    const QString query("UPDATE push SET unread = :unread;");

    sqlQuery.prepare(query);

    sqlQuery.bindValue(":unread", false);
    sqlQuery.exec();

    const QSqlError err = sqlQuery.lastError();

    if (err.isValid()) {
        qWarning() << "Error executing SQL statement: " << query << ". ERROR: " << err.text();
        return false;
    }

    return true;
}
