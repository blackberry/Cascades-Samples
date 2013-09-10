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

#include "SqlHeaderDataQuery.hpp"
#include "SqlTransaction.hpp"
#include "SqlHeaderDataQuery.hpp"
#include "SqlQueryUtils.hpp"

#include <bb/cascades/datamanager/DataItem>
#include <bb/cascades/datamanager/DataRevision>

#include <QVariantMap>
#include <QStringList>
#include <QTime>
#include <QSharedData>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <QStringBuilder>
#include <QTextStream>
#include <QDebug>

using namespace bb::cascades::datamanager;

SqlHeaderDataQuery::SqlHeaderDataQuery(QObject *parent)
    : HeaderDataQuery(parent)
    , m_dataQuery(new SqlDataQuery(parent))
{
}

SqlHeaderDataQuery::SqlHeaderDataQuery(const QString &query,
        const QString &headerQuery, QObject *parent)
    : HeaderDataQuery(parent)
    , m_headerQuery(headerQuery)
    , m_dataQuery(new SqlDataQuery(query, parent))
{
}

SqlHeaderDataQuery::~SqlHeaderDataQuery() {
}

void SqlHeaderDataQuery::setHeaderQuery(const QString &query) {
    if (m_headerQuery.isEmpty()) {
        m_headerQuery = query;
    } else {
        qDebug() << "Query has already been set to " << m_headerQuery
                  << ".  Can't reset to " << query;
    }
}

QString SqlHeaderDataQuery::headerQuery() const {
    return m_headerQuery;
}

void SqlHeaderDataQuery::setSource(const QUrl& source) {
    m_dataQuery->setSource(source);
}

QUrl SqlHeaderDataQuery::source() const {
    return m_dataQuery->source();
}

void SqlHeaderDataQuery::setQuery(const QString &query) {
    m_dataQuery->setQuery(query);
}

QString SqlHeaderDataQuery::query() const {
    return m_dataQuery->query();
}

void SqlHeaderDataQuery::setScrollUpQuery(const QString &scrollUpQuery) {
    m_dataQuery->setScrollUpQuery(scrollUpQuery);
}

QString SqlHeaderDataQuery::scrollUpQuery() const {
    return m_dataQuery->scrollUpQuery();
}

void SqlHeaderDataQuery::setScrollDownQuery(const QString &scrollDownQuery) {
    m_dataQuery->setScrollDownQuery(scrollDownQuery);
}

QString SqlHeaderDataQuery::scrollDownQuery() const {
    return m_dataQuery->scrollDownQuery();
}

void SqlHeaderDataQuery::setKeyColumn(const QString& keyColumn) {
    m_dataQuery->setKeyColumn(keyColumn);
}

QString SqlHeaderDataQuery::keyColumn() const {
    return m_dataQuery->keyColumn();
}

void SqlHeaderDataQuery::setRevisionColumn(const QString& revisionColumn) {
    m_dataQuery->setRevisionColumn(revisionColumn);
}

QString SqlHeaderDataQuery::revisionColumn() const {
    return m_dataQuery->revisionQuery();
}

void SqlHeaderDataQuery::setRevisionQuery(const QString& revisionQuery) {
    m_dataQuery->setRevisionQuery(revisionQuery);
}

QString SqlHeaderDataQuery::revisionQuery() const {
    return m_dataQuery->revisionQuery();
}

void SqlHeaderDataQuery::setCountQuery(const QString &countQuery) {
    m_dataQuery->setCountQuery(countQuery);
}

QString SqlHeaderDataQuery::countQuery() const {
    return m_dataQuery->countQuery();
}

void SqlHeaderDataQuery::setValuesToBind(const QVariantMap &nameValueMap) {
    m_dataQuery->setValuesToBind(nameValueMap);
}

QVariantMap SqlHeaderDataQuery::valuesToBind() const {
    return m_dataQuery->valuesToBind();
}

bool SqlHeaderDataQuery::getData(int offset, int limit, DataRevision *revision,
        int *totalCount, QList<DataItem> *results) {

    if (!m_dataQuery->getData(offset, limit, revision, totalCount, results)) {
        if (m_dataQuery->sqlError()->type() != QSqlError::NoError) {
            Q_EMIT error(m_dataQuery->sqlError()->type(),
                         m_dataQuery->sqlError()->databaseText());
        }
        return false;
    }
    return true;
}

bool SqlHeaderDataQuery::getDataForRevision(int offset, int limit,
        const DataRevision& requestedRevision, QList<DataItem> *results) {

    if (!m_dataQuery->getDataForRevision(offset, limit, requestedRevision,
            results)) {
        if (m_dataQuery->sqlError()->type() != QSqlError::NoError) {
            Q_EMIT error(m_dataQuery->sqlError()->type(),
                         m_dataQuery->sqlError()->databaseText());
        }
        return false;
    }
    return true;
}

bool SqlHeaderDataQuery::getHeaderData(int offset, int limit,
        DataRevision *revision, int *totalCount, QList<DataItem> *results,
        QList<HeaderDataItem> *headerResults) {
    bool result = false;
    // get the open db connection and start a transaction
    SqlQueryUtils utils;
    QSqlDatabase connection = utils.connection(m_dataQuery->source(),
            m_dataQuery->sqlError());
    if (m_dataQuery->sqlError()->type() == QSqlError::NoError) {
        SqlTransaction tx(connection);
        if (m_dataQuery->getDatabaseRevision(connection, revision)) {

            bool headerResult = false;
            // Get all headers
            if (m_headerQuery.isEmpty()) {
                qWarning() << "The Header Query is empty.";
            } else {
                QList<DataItem> headerData;
                // note that the key column name for headers can be either the defined m_keyColumn or "header"
                if (utils.getQueryData(connection, m_headerQuery, 0, -1,
                        m_dataQuery->valuesToBind(), m_dataQuery->keyColumn(),
                        "", &headerData, m_dataQuery->sqlError())) {
                    // populate the <childCount> and possible <keyId> in each HeaderDataItem
                    *headerResults = utils.normalizeHeaderData(headerData);
                    //qDebug() << "*****Headers****** " << *headerResults;
                    headerResult = true;
                } else {
                    qDebug() << "Failed to load the headers.";
                }

            }
            if (headerResult) {
                result = m_dataQuery->getDataResults(connection, offset, limit,
                        results);

                // Get the data count.
                if (m_dataQuery->countQuery().isEmpty()) {
                    qWarning() << "The Count Query is empty.";
                    *totalCount = 0;
                    result = false;
                } else {
                    int count = results->size();
                    if (count > 0 && (count < limit || limit < 0)) {
                        // We hit the end of the result set, so we can infer the total size. Notice that we should only
                        // infer total count when the offset is not out-of-range. If the results size is greater than zero,
                        // the offset must be within the range.
                        *totalCount = count + offset;
                    } else {
                        QVariant resultCount;
                        if (utils.getSingleQueryValue(connection,
                                m_dataQuery->countQuery(),
                                m_dataQuery->valuesToBind(), "count(*)",
                                &resultCount, m_dataQuery->sqlError())) {
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
    }

    if (m_dataQuery->sqlError()->type() != QSqlError::NoError) {
        Q_EMIT error(m_dataQuery->sqlError()->type(),
                m_dataQuery->sqlError()->databaseText());
    }
    return result;
}

QString SqlHeaderDataQuery::toString() const {
    return "SqlHeaderDataQuery(\"" + headerQuery() + "\",\"" + query() + "\")";
}
