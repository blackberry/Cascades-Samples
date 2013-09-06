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

#ifndef SQLDATAQUERY_HPP
#define SQLDATAQUERY_HPP

#include <bb/cascades/datamanager/DataQuery>

#include <QScopedPointer>
#include <QUrl>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

class SqlDataQueryPrivate;
namespace bb {
namespace cascades {
namespace datamanager {
class DataRevision;
}
}
}

/*!
 * The default DataQuery implementation that uses an SQL select to retrieve data.
 *
 * Note: Once the properties are set they cannot be changed. The query is
 * normally executed in a secondary thread and allowing changes to these
 * properties once the query is in operation would cause unpredictable results.
 *
 */
class SqlDataQuery: public bb::cascades::datamanager::DataQuery {
    Q_OBJECT

    /*!
     * @brief The source URL for the path to the local database. Mandatory.
     *
     * Once the property is set it cannot be changed.
     *
     */
    Q_PROPERTY(QUrl source READ source WRITE setSource)

    /*!
     * @brief The main SQL query statement. Mandatory.
     *
     * Once the property is set it cannot be changed.
     *
     */
    Q_PROPERTY(QString query READ query WRITE setQuery)

    Q_PROPERTY(QString scrollUpQuery READ scrollUpQuery WRITE setScrollUpQuery)
    Q_PROPERTY(QString scrollDownQuery READ scrollDownQuery WRITE setScrollDownQuery)

    /*!
     * @brief The name of the key column in the main query which is returned for each item.
     *
     * This key, if returned for each DataItem by the main query, will uniquely identify
     * the data item. It is used by the data model to signal listeners (usually an associated
     * list) that items have changed location or been deleted.
     *
     * Use of this property is optional. However, without keys, adding and deleting items in
     * the database may result in poor user interface visual updating.
     *
     * Once the property is set it cannot be changed.
     *
     * An example:
     *   The query:     "SELECT key_id, revision_id, lastname, firstname FROM contacts"
     *   The keyColumn: "key_id"
     *
     * @see DataItem
     *
     */
    Q_PROPERTY(QString keyColumn READ keyColumn WRITE setKeyColumn)

    /*!
     * @brief The name of the revision column in the main query which is returned for each item.
     *
     * This revision, if returned for each DataItem by the main query, will identify the
     * current state of that item. It's used in conjunction with the overall revision.
     * When a database item is updated its revision should also be updated as well as the
     * overall database revision.
     *
     * It is used to determine when items must be updated in any cached data in memory.
     * Use of this property is optional. However, without item revisions, database changes
     * may not be reflected in the user interface in a timely manner.
     *
     * Once the property is set it cannot be changed.
     *
     * An example:
     *   The query:          "SELECT key_id, revision_id, lastname, firstname FROM contacts"
     *   The revisionColumn: "revision_id"
     *
     * @see DataItem
     *
     */
    Q_PROPERTY(QString revisionColumn READ revisionColumn WRITE setRevisionColumn)

    /*!
     * @brief An SQL query statement which will return the total count of items.
     *
     * The property is mandatory when the query is being used in async models. It is
     * needed to obtain the total count of database items even when the model retains
     * only a partial cache in memory.
     *
     * An example:  "SELECT count(*) FROM contacts"
     *
     * Once the property is set it cannot be changed.
     *
     */
    Q_PROPERTY(QString countQuery READ countQuery WRITE setCountQuery)

    /*!
     * @brief An SQL query statement to return the current overall revision for the source.
     *
     * This revision represents the current state of the database. It is used to ensure
     * that data for different database states is not mixed in memory. If the data model
     * determines that the overall revision of the data has changed then any cached data
     * is refreshed by querying the data source again.
     *
     * Use of this property is optional. However, without an overall revision, database
     * queries will always be a full refresh of the cache so that the data can be guaranteed
     * to be consistent.
     *
     * An example:  "SELECT revision_id FROM revision"
     *
     * Once the property is set it cannot be changed.
     *
     */
    Q_PROPERTY(QString revisionQuery READ revisionQuery WRITE setRevisionQuery)

    /*!
     * @brief A map of name-to-value bindings.
     *
     * This allows values to be inserted for named placeholders in each
     * query statement. Once the property is set it cannot be changed.
     *
     */
    Q_PROPERTY(QVariantMap bindValues READ valuesToBind WRITE setValuesToBind)

public:
    /*!
     * Constructor.
     *
     * @param parent The parent owner or 0. Optional and will default to 0 if not specified.
     *
     */
    explicit SqlDataQuery(QObject *parent = 0);

    /*!
     * Constructor.
     *
     * @param query The main SQL query statement to use for this data query.
     * @param parent The parent owner or 0. Optional and will default to 0 if not specified.
     *
     */
    explicit SqlDataQuery(const QString &query, QObject *parent = 0);

    /*!
     * Destructor.
     *
     */
    virtual ~SqlDataQuery();

    /*!
     * Set the source url. Mandatory.
     *
     * @param source The source url.
     *
     */
    void setSource(const QUrl& source);

    /*!
     * Get the source.
     *
     * @return The source url.
     *
     */
    QUrl source() const;

    /*!
     * Set the query string. Mandatory.
     *
     * @param query The query string.
     *
     */
    void setQuery(const QString& query);

    void setScrollUpQuery(const QString& scrollUpQuery);
    void setScrollDownQuery(const QString& scrollDownQuery);

    /*!
     * Get the query.
     *
     * @return The query string.
     *
     */
    QString query() const;

    QString scrollUpQuery() const;
    QString scrollDownQuery() const;

    /*!
     * Set the name of the key column in the main query.
     *
     * @param keyColumn The key column name.
     *
     */
    void setKeyColumn(const QString& keyColumn);

    /*!
     * Get the name of the key column in the main query.
     *
     * @return The keyColumn name.
     *
     */
    QString keyColumn() const;

    /*!
     * Set the name of the revision column in the main query.
     *
     * @param revisionColumn The revision column name.
     *
     */
    void setRevisionColumn(const QString& revisionColumn);

    /*!
     * Get the name of the revision column in the main query.
     *
     * @return The revisionColumn name.
     *
     */
    QString revisionColumn() const;

    /*!
     * Set the count query string.
     *
     * @param countQuery The count query string.
     *
     */
    void setCountQuery(const QString& countQuery);

    /*!
     * Get the count query.
     *
     * @return The countQuery string.
     *
     */
    QString countQuery() const;

    /*!
     * Set the overall revision query string.
     *
     * @param revisionQuery The revision query string.
     *
     */
    void setRevisionQuery(const QString& revisionQuery);

    /*!
     * Get the revision query.
     *
     * @return The revisionQuery string.
     *
     */
    QString revisionQuery() const;

    /*!
     * Bind values to this query by placeholder name.
     *
     * @param nameValueMap a map of placeholder name to value.
     *
     */
    void setValuesToBind(const QVariantMap& nameValueMap);

    /*!
     * Retrieve the values bound to the query.
     *
     * @return map of parameter name to value
     *
     */
    QVariantMap valuesToBind() const;

    /*!
     * @see DataQuery::getData
     *
     */
    virtual bool getData(int offset, int limit,
            bb::cascades::datamanager::DataRevision *revision, int *totalCount,
            QList<bb::cascades::datamanager::DataItem> *results);

    /*!
     * @see DataQuery::getDataForRevision
     *
     */
    virtual bool getDataForRevision(int offset, int limit,
            const bb::cascades::datamanager::DataRevision& requestedRevision,
            QList<bb::cascades::datamanager::DataItem> *results);

    /*!
     * @see DataQuery::toString
     *
     */
    virtual QString toString() const;

    /**
     * Retrieve the last sql error.
     */
    QSqlError *sqlError();

    /**
     * Get the current overall database revision if there is a revision query and it returns a numeric revision.
     * Return true to indicate success (continue operation) and false to indicate failure (abort operation).
     */
    bool getDatabaseRevision(QSqlDatabase &connection,
            bb::cascades::datamanager::DataRevision *dbRevision);

    /**
     *  Common method for optimizing query and returning results.
     */
    bool getDataResults(QSqlDatabase &connection, int offset, int limit,
            QList<bb::cascades::datamanager::DataItem> *results);

private:
    QString m_query;
    QString m_countQuery;
    QString m_revisionQuery;
    QString m_scrollUpQuery;
    QString m_scrollDownQuery;
    QString m_keyColumn;
    QString m_revisionColumn;
    QUrl m_source;
    QVariantMap m_bindValues;
    QSqlError m_error;
    bb::cascades::datamanager::DataItem m_startItem;
    bb::cascades::datamanager::DataItem m_endItem;
    int m_startOffset;
    int m_endOffset;
    int m_totalCount;
    int m_scrollUpCount;
    int m_scrollDownCount;

    Q_DECLARE_PRIVATE(SqlDataQuery)
    Q_DISABLE_COPY(SqlDataQuery)
};

#endif /* SQLDATAQUERY_HPP */
