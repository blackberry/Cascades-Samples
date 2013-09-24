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

#ifndef SQLTRANSACTION_HPP
#define SQLTRANSACTION_HPP

#include <bb/cascades/datamanager/Global>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

/*!
 * A simple SQL transaction object which starts a database transaction when
 * it is created and ends it when it goes out of scope and is deleted.
 *
 * Example usage:
 *     doWork() {
 *         QSqlDatabase connection = <get open connection>;
 *         SqlTransaction tx(connection);
 *         ... do one more database operations within the same transaction ...
 *     }
 *
 * Regardless of how many ways the code might exit the doWork() method this
 * guarantees that the transaction will be ended.
 *
 */
class SqlTransaction
{
public:
    /*!
     * Constructor.
     *
     * @param connection The open database connection.
     *
     */
    explicit SqlTransaction(QSqlDatabase &connection);

    /*!
     * Destructor.
     *
     */
    virtual ~SqlTransaction();

    /*!
     * Explicitly end the transaction. Note that its ended automatically when
     * this transaction object is destroyed.
     *
     */
    void endTransaction();

    /*!
     * The last database error if any.
     *
     */
    QSqlError lastError() const;

private:
    QSqlDatabase m_dbConnection;
};


#endif /* SQLTRANSACTION_HPP */
