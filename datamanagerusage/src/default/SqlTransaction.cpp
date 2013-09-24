/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#include "SqlTransaction.hpp"
#include <bb/cascades/datamanager/Global>
#include <QtSql/QSqlDatabase>
#include <QDebug>

SqlTransaction::SqlTransaction(QSqlDatabase &connection)
    : m_dbConnection(connection)
{
    if (!m_dbConnection.transaction()) {
        qDebug() << "SqlTransaction could not begin transaction for database " << m_dbConnection.databaseName();
    }
}

SqlTransaction::~SqlTransaction() {
    SqlTransaction::endTransaction();
}

void SqlTransaction::endTransaction() {
    if (!m_dbConnection.commit()) {
        qDebug() << "SqlTransaction could not commit transaction for database " << m_dbConnection.databaseName();
        m_dbConnection.rollback();
    }
}

QSqlError SqlTransaction::lastError() const {
    return m_dbConnection.lastError();
}

