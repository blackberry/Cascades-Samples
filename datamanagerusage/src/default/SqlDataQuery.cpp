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

#include "NumericRevision.hpp"
#include "SqlDataQuery.hpp"
#include "SqlTransaction.hpp"
#include "SqlQueryUtils.hpp"

#include <QDebug>

using namespace bb::cascades::datamanager;

SqlDataQuery::SqlDataQuery(QObject *parent)
    : DataQuery(parent)
    , m_startOffset(-1)
    , m_endOffset(-1)
    , m_totalCount(0)
    , m_scrollUpCount(0)
    , m_scrollDownCount(0) {
}

SqlDataQuery::SqlDataQuery(const QString &query, QObject *parent)
    : DataQuery(parent) {
    setQuery(query);
}

SqlDataQuery::~SqlDataQuery() {
}

void SqlDataQuery::setSource(const QUrl& source) {
    if(!m_source.isEmpty()) {
        qWarning() << "source has already been set to " << m_source
                    << ".  Can't reset to " << source;
        return;
    }
    m_source = source;
}

QUrl SqlDataQuery::source() const {
    return m_source;
}

void SqlDataQuery::setQuery(const QString &query) {
    if (!m_query.isEmpty()) {
        qWarning() << "Query has already been set to " << m_query
                    << ".  Can't reset to " << query;
        return;
    }
    m_query = query;
}

QString SqlDataQuery::query() const {
    return m_query;
}

void SqlDataQuery::setScrollUpQuery(const QString &scrollUpQuery) {
    if (!m_scrollUpQuery.isEmpty()) {
        qWarning() << "ScrollUpQuery has already been set to "
                    << m_scrollUpQuery << ".  Can't reset to " << scrollUpQuery;
        return;
    }
    m_scrollUpQuery = scrollUpQuery;
}

QString SqlDataQuery::scrollUpQuery() const {
    return m_scrollUpQuery;
}

void SqlDataQuery::setScrollDownQuery(const QString &scrollDownQuery) {
    if (!m_scrollDownQuery.isEmpty()) {
        qWarning() << "scrollDownQuery has already been set to "
                    << m_scrollDownQuery << ".  Can't reset to " << scrollDownQuery;
        return;
    }
    m_scrollDownQuery = scrollDownQuery;
}

QString SqlDataQuery::scrollDownQuery() const {
    return m_scrollDownQuery;
}

void SqlDataQuery::setKeyColumn(const QString& keyColumn) {
    if(!m_keyColumn.isEmpty()) {
        qWarning() << "keyColumn has already been set to " << m_keyColumn
                    << ".  Can't reset to " << keyColumn;
        return;
    }
    m_keyColumn = keyColumn;
}

QString SqlDataQuery::keyColumn() const {
    return m_keyColumn;
}

void SqlDataQuery::setRevisionColumn(const QString& revisionColumn) {
    if(!m_revisionColumn.isEmpty()) {
        qWarning() << "revisionColumn has already been set to " << m_revisionColumn
                    << ".  Can't reset to " << revisionColumn;
        return;
    }
    m_revisionColumn = revisionColumn;
}

QString SqlDataQuery::revisionColumn() const {
    return m_revisionColumn;
}

void SqlDataQuery::setRevisionQuery(const QString& revisionQuery) {
    if (!m_revisionQuery.isEmpty()) {
        qWarning() << "Revision query has already been set to "
                    << m_revisionQuery << ".  Can't reset to " << revisionQuery;
        return;
    }
    m_revisionQuery = revisionQuery;
}

QString SqlDataQuery::revisionQuery() const {
    return m_revisionQuery;
}

void SqlDataQuery::setCountQuery(const QString &countQuery) {
    if (!m_countQuery.isEmpty()) {
        qWarning() << "Count query has already been set to " << m_countQuery
                    << ".  Can't reset to " << countQuery;
        return;
    }
    m_countQuery = countQuery;
}

QString SqlDataQuery::countQuery() const {
    return m_countQuery;
}

void SqlDataQuery::setValuesToBind(const QVariantMap &nameValueMap) {
    if (!m_bindValues.isEmpty()) {
        qWarning() << "Values for bind operation already set!";
        return;
    }
    m_bindValues = nameValueMap;
}

QVariantMap SqlDataQuery::valuesToBind() const {
    return m_bindValues;
}

bool SqlDataQuery::getData(int offset, int limit, DataRevision *revision,
        int *totalCount, QList<DataItem> *results) {

    if (revision == NULL || totalCount == NULL || results == NULL) {
        qCritical() << "Null pointer passed for return parameter.";
        return false;
    }

    // get the open db connection and start a transaction
    bool result = false;
    SqlQueryUtils utils;
    QSqlDatabase connection = utils.connection(m_source, &m_error);
    if (m_error.type() == QSqlError::NoError) {
        SqlTransaction tx(connection);
        // get the overall revision
        if (getDatabaseRevision(connection, revision)) {

            // query and return results
            result = getDataResults(connection, offset, limit, results);

            // Get the data count.
            if (m_countQuery.isEmpty()) {
                qWarning() << "The Count Query is empty."; //can proceed with an empty Count Query.
                *totalCount = 0;
            } else {
                int count = results->size();
                if (count > 0 && (count < limit || limit < 0)) {
                    // We hit the end of the result set, so we can infer the total size. Notice that we should only
                    // infer total count when the offset is not out-of-range. If the results size is greater than zero,
                    // the offset must be within the range.
                    *totalCount = count + offset;
                } else {
                    // If we didn't get all the data due to limit, get the totalCount
                    QVariant resultCount;
                    if (utils.getSingleQueryValue(connection, m_countQuery,
                            m_bindValues, "count(*)", &resultCount, &m_error)) {
                        *totalCount = resultCount.toInt();
                    } else {
                        qWarning()
                                << "Failed to fetch a single value for the data count.";
                        result = false;
                    }
                }
            }
        }
    }

    if (m_error.type() != QSqlError::NoError) {
        Q_EMIT error(m_error.type(), m_error.databaseText());
    }
    return result;
}

bool SqlDataQuery::getDataForRevision(int offset, int limit,
        const DataRevision& requestedRevision, QList<DataItem> *results) {

    if (results == NULL)  {
        qCritical() << "Null pointer passed for return parameter.";
        return false;
    }
    // get the open db connection and start a transaction
    bool result = false;
    QSqlDatabase connection = SqlQueryUtils().connection(m_source, &m_error);

    if (m_error.type() == QSqlError::NoError) {
        SqlTransaction tx(connection);
        DataRevision dbRevision;
        if (getDatabaseRevision(connection, &dbRevision)) {
            if (requestedRevision == dbRevision) {
                // query and return results
                result = getDataResults(connection, offset, limit, results);
            }
        }
    }
    if (m_error.type() != QSqlError::NoError) {
        Q_EMIT error(m_error.type(), m_error.databaseText());
    }
    return result;
}

QString SqlDataQuery::toString() const {
    return "SqlDataQuery(\"" + query() + "\")";
}

QSqlError *SqlDataQuery::sqlError() {
    return &m_error;
}

bool SqlDataQuery::getDatabaseRevision(QSqlDatabase &connection,
        DataRevision *revision) {
    bool revisionOK = false;
    quint64 numericRevision = 0L;
    if (!m_revisionQuery.isEmpty()) {
        QVariant resultRevision;
        if (SqlQueryUtils().getSingleQueryValue(connection, m_revisionQuery,
                m_bindValues, "", &resultRevision, &m_error)) {
            // this code only supports numeric revisions; empty DataRevision returned otherwise.
            numericRevision = resultRevision.toULongLong(&revisionOK);
        } else {
            qWarning() << "Failed to fetch the database revision.";
            return false;
        }
    }
    *revision =
            revisionOK ?
                    DataRevision(new NumericRevision(numericRevision)) :
                    DataRevision();
    return true;
}

bool SqlDataQuery::getDataResults(QSqlDatabase &connection, int offset,
        int limit, QList<DataItem> *results) {

    if (!m_query.isEmpty()) {
        // are we using caching to optimize scrolling queries?
        // ???? how to deal with overlapping names between valueBinding and DataItem ???????
        QString queryToUse = m_query;
        QVariantMap valuesToUse = m_bindValues;
        int offsetToUse = offset;
        bool useCache = !m_scrollUpQuery.isEmpty()
                         && !m_scrollDownQuery.isEmpty();
        if (useCache && 0 < m_endOffset) {
            // we have previously cached data
            if (offset < m_startOffset) {
                // we're moving up the list
                if (m_startOffset == (offset + limit)) {
                    // we have a cached anchor item we can use for optimized query
                    queryToUse = m_scrollUpQuery;
                    valuesToUse.unite(m_startItem.payload().toMap());
                    ++m_scrollUpCount;
                }
            } else if (offset > m_startOffset) {
                // moving down the list
                if (m_endOffset == (offset - 1)) {
                    // we have a cached anchor item we can use for optimized query
                    queryToUse = m_scrollDownQuery;
                    valuesToUse.unite(m_endItem.payload().toMap());
                    offsetToUse = 0; // do not use offset
                    ++m_scrollDownCount;
                }
            }
        }
        qDebug() << "Current query statistics: total queries = " << ++m_totalCount
                 << ", queries using scrollUp optimization = " << m_scrollUpCount
                 << ", queries using scrollDown optimization = " << m_scrollDownCount;

        // Get the data.
        if (!SqlQueryUtils().getQueryData(connection, queryToUse, offsetToUse,
                limit, valuesToUse, m_keyColumn, m_revisionColumn, results,
                &m_error)) {
            qWarning() << "Failed to load the data.";
            qDebug() << "queryToUse=" << queryToUse << ",offsetToUse="
                     << offsetToUse << ",valuesToUse=" << valuesToUse
                     << ",m_error=" << m_error;
        } else {
            // Success - update cache if we're using it
            if (useCache) {
                int resultCount = results->size();
                if (resultCount < 2) {
                    // next time - this will indicate that we do not have cached data
                    m_startOffset = -1;
                    m_endOffset = -1;
                } else {
                    m_startOffset = offset;
                    m_endOffset = offset + resultCount - 1;
                    m_startItem = results->at(0);
                    m_endItem = results->at(resultCount - 1);
                    //TDEBUG << "Caching: start=" <<  m_startOffset << ", " << m_startItem;
                    //TDEBUG << "Caching: end=" <<  m_endOffset << ", " << m_endItem;
                }
            }
            return true;
        }
    } else {
        qWarning() << "The Query is empty.";
    }
    return false;
}

