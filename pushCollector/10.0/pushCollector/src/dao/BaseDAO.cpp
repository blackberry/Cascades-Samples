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

#include "BaseDAO.hpp"
#include <QDebug>

BaseDAO::BaseDAO()
{

}

BaseDAO::~BaseDAO()
{
    if (m_connection.isOpen()) {
        QSqlDatabase::removeDatabase(DATABASE_PATH);
        m_connection.removeDatabase(DATABASE_DRIVER);
    }
}

QSqlDatabase& BaseDAO::SQLConnection()
{
    if (m_connection.isOpen()) {
        return m_connection;
    }

    if (QSqlDatabase::contains(DATABASE_CONNECTION_NAME)){
        // if the database connection already exists, then reuse it
        // Calling this also calls "open"
        m_connection = QSqlDatabase::database(DATABASE_CONNECTION_NAME);

        if (!m_connection.isOpen()){
            qWarning() << "Could not open connection to database: " << m_connection.lastError().text();
        }
    } else {
        // Otherwise, the connection doesn't exist, so create it and open it
        m_connection = QSqlDatabase::addDatabase(DATABASE_DRIVER, DATABASE_CONNECTION_NAME);
        m_connection.setDatabaseName(DATABASE_PATH);

        if (!m_connection.isValid()) {
            qDebug() << "Could not set data base name probably due to invalid driver.";
        } else if (!m_connection.open()) {
            qWarning() << "Could not open connection to database: " << m_connection.lastError().text();
        }
    }

    return m_connection;
}
