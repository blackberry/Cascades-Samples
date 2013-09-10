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

#ifndef SQLQUERYUTILS_HPP
#define SQLQUERYUTILS_HPP

#include <bb/cascades/datamanager/Global>
#include <bb/cascades/datamanager/DataItem>
#include <bb/cascades/datamanager/HeaderDataItem>
#include <QList>
#include <QScopedPointer>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>


/*!
 * @brief Utility methods for performing common operations in SQL data queries.
 *
 * @see SqlDataQuery
 * @see SqlHeaderDataQuery
 *
 *
 */
class SqlQueryUtils
{
public:
    /*!
     * Constructor.
     *
     */
    SqlQueryUtils();

    /*!
     * Destructor.
     *
     */
    virtual ~SqlQueryUtils();

    /*!
     * Return an existing or new database connection for the database at the source URL.
     *
     * @param source The path to the local database.
     * @param[out] error The error object to update with the status. Pointer must not be null.
     * If OK, then QSqlError::type() will be QSqlError::NoError.
     *
     * @return The QSqlDatabase object to use as an open connection to the database.
     *
     */
    QSqlDatabase connection(const QUrl &source, QSqlError *error);

    /*!
     * Execute the supplied SQL query after binding any values and return a single result value.
     *
     * @param connection The open database connection.
     * @param query The SQL count query.
     * @param bindValues A map used to replace any named placeholders in the query with values.
     * @param resultName The name of the result column. If not provided then the first value is returned.
     * @param[out] resultValue The query result value being returned. Pointer must not be null.
     * @param[out] error The error object to update with the status. Pointer must not be null.
     * @return Returns true if the data could be successfully retrieved, else returns false.
     *
     */
    bool getSingleQueryValue(QSqlDatabase &connection, const QString &query, const QVariantMap &bindValues,
            const QString &resultName, QVariant *resultValue, QSqlError *error);

    /*!
     * Execute the supplied SQL data query after applying limit and offset and binding any values.
     *
     * @param connection The open database connection.
     * @param query The SQL data query.
     * @param offset The index offset within the view. A negative offset is ignored.
     * @param limit The number of items to retrieve. A negative limit means both limit and offset are ignored.
     * @param bindValues A map used to replace any named placeholders in the query with values.
     * @param keyColumn The key column name used in the query. May be empty.
     * @param revisionColumn The revision column name used in the query. May be empty.
     * @param[out] results The list of data items which is the return data. Pointer must not be null.
     * @param[out] error The error object to update with the status. Pointer must not be null.
     * @return Returns true if the data could be successfully retrieved, else returns false.
     *
     */
    bool getQueryData(QSqlDatabase &connection, const QString &query, int offset, int limit,
         const QVariantMap &bindValues, const QString &keyColumn, const QString &revisionColumn,
         QList<bb::cascades::datamanager::DataItem> *results, QSqlError *error);

    /*!
     * Extend the given query string with offset and/or limit and return the modified string.
     * Offset cannot be used without limit.
     *
     * @param query The SQL query to extend.
     * @param offset The index offset within the view. A negative offset is ignored.
     * @param limit The number of items to retrieve. A negative limit means both limit and offset are ignored.
     * @return Returns true if the data could be successfully retrieved, else returns false.
     *
     */
    QString extendQuery(const QString &query, int offset, int limit);

    /*!
     * Use this method to normalize DataItem into HeaderDataItem. If the
     * header child count is zero, the item will be removed.
     */
    QList<bb::cascades::datamanager::HeaderDataItem> normalizeHeaderData(const QList<bb::cascades::datamanager::DataItem> &data);

    /**
     * Prepare the given QSqlQuery object using the query string and bindValues map.
     *
     * NOTE: This currently does a better job of preparing the query and binding values
     * than using the equivalent QT code. It will perform correctly:
     * 1. When a query has no parameters but bindValues *is* supplied.
     * 2. When a query uses the same named parameter more than once in query string.
     */
    void prepareQuery(const QString &query, const QVariantMap &bindValues, QSqlQuery *sqlQuery);

};

#endif /* SQLQUERYUTILS_HPP */
