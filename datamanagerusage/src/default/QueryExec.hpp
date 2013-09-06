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

#ifndef BB_CASCADES_DATAMANAGER_QUERYEXEC_HPP
#define BB_CASCADES_DATAMANAGER_QUERYEXEC_HPP

#include <bb/Global>
#include <bb/data/AsyncDataAccess>
#include <bb/data/DataAccessReply>
#include <QList>
#include <QMetaType>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QVariant>
#include <QtDeclarative/QtDeclarative>
#include <QtSql/QSqlDatabase>

/*!
 * @headerfile QueryExec.hpp <bb/data/QueryExec>
 *
 * @brief Executes SQL queries from QML for live update testing.
 *
 * A QML example:
 *
 *  QueryExec {
 *      // Start it by calling execute(). Can be stopped by calling stop().
 *      // Each time it will update 10 rows (including both overall revision, item revision and last_change)
 *      // and then notify the data model via its associated query.
 *      // Next time (after 1 second interval) it will go down 100 rows and do the same.
 *      // Query sequence:
 *      property string updateRevision: "update revision set revision_id = revision_id + 1"
 *      property string selectRevision: "select revision_id from revision"
 *      property string updateContact: "update contact set revision_id = (select revision_id from revision), last_change = datetime('now') " +
 *      "where rowid >= :startRow and rowid < (:startRow + 10)"
 *      property int startRow: 0
 *      id:         liveupdate2
 *      source:     "sql/contacts1k.db"
 *      queries:    [ updateRevision, updateContact, selectRevision ]
 *      bindValues: { "startRow" : startRow }
 *      times:      999 // execute x times (default is 1)
 *      interval:   1000 // interval y milliseconds before next execution after first (default is 1000)
 *      onError:    console.log("live update error: " + code + ", " + message)
 *      onExecuted: {
 *          console.log("live query update was performed: startRow=" + startRow + "; revision=" + data["revision_id"]);
 *          // todo - notify query that source revision has changed
 *          startRow = (startRow + 100) % 1024; // next time start further down, wrapping at the bottom of the 1024 row table
 *      }
 *  }
 *
 */
class QueryExec : public QObject {
    Q_OBJECT

    /*!
     * @brief The path to the external data source.
     *
     * In QML, this path is relative to the QML document in which this @c QueryExec
     * is declared. When setting this property from C++, this path is instead relative
     * to the application working directory.
     *
     * @since BlackBerry 10.2.0
     */
    Q_PROPERTY(QUrl source READ source WRITE setSource)

    /*!
     * @brief The queries property contain a list of SQL query statements.
     */
    Q_PROPERTY(QStringList queries READ queries WRITE setQueries)

    /*!
     * @brief A map of name-to-value bindings.
     *
     * This allows values to be inserted for named place holders in each
     * query statement. Once the property is set it cannot be changed.
     */
    Q_PROPERTY(QVariantMap bindValues READ valuesToBind WRITE setValuesToBind)

    /*!
     * @brief Count of times to execute. Default is 1.
     *
     * If times is set to a number greater than 1 then a timer is used to determine interval between executions.
     */
    Q_PROPERTY(int times READ times WRITE setTimes)

    /*!
     * @brief Interval in milliseconds between executions. Only used if times property is greater than 1. Default is 1000 ms.
     *
     * If times is set to a number greater than 1 then a timer is used to determine interval between executions.
     */
    Q_PROPERTY(int interval READ interval WRITE setInterval)

public:

    /*!
     * @brief Constructs a @c QueryExec object with the specified parent.
     *
     * If the parent is not 0, ownership of this object will be transferred to the parent.
     *
     * @param parent The parent owner or 0. Optional and will default to 0 if not specified.
     */
    QueryExec(QObject *parent = 0);

    /*!
     * @brief Virtual destructor.
     */
    virtual ~QueryExec();

    /*!
     * @brief Registers @c QueryExec for use in QML.
     */
    static void registerQmlTypes();

    /*!
     * @brief Gets the current value of the #source property.
     *
     * @return The current path to the external data source.
     */
    QUrl source() const;

    /*!
     * @brief Sets a new path to the external data source.
     *
     * @param source The new path of the external data source.
     */
    Q_SLOT void setSource(const QUrl& source);

    /*!
     * @brief Gets the current value of the #queries property.
     *
     * @return The query list to execute.
     */
    QStringList queries() const;

    /*!
     * @brief Sets a new list of queries to execute.
     *
     * @param queryList The new queries to use.
     */
    Q_SLOT void setQueries(const QStringList& queryList);

    /*!
     * Retrieve the map of place holder name to value bindings.
     * This set of value bindings are used for all queries.
     *
     * @return map of place holder name to value.
     */
    QVariantMap valuesToBind() const;

    /*!
     * Bind values to the queries by place holder name.
     * This set of value bindings are used for all queries.
     *
     * @param nameValueMap a map of place holder name to value.
     */
    Q_SLOT void setValuesToBind(const QVariantMap& nameValueMap);

    /*!
     * @brief Gets the current value of the #times property.
     *
     * @return The count of times to execute. Default is 1.
     */
    int times() const;

    /*!
     * @brief Sets the count of times to execute the queries. Default is 1.
     *
     * If it is set to a value greater than 1 then a timer is used to execute
     * the queries repeatedly controlled by the #times and #interval properties.
     *
     * @param times The count of times to execute.
     */
    Q_SLOT void setTimes(int times);

    /*!
     * @brief Gets the current value of the #interval property.
     *
     * @return The interval between executions in milliseconds. Default is 1000 ms.
     */
    int interval() const;

    /*!
     * @brief Sets the interval between executions in milliseconds. Default is 1000 ms.
     *
     * This is only used if the times property is greater than 1. If it is then
     * a timer is used to execute the queries repeatedly controlled by the #times
     * and #interval properties.
     *
     * @param times The count of times to execute.
     */
    Q_SLOT void setInterval(int interval);

    /*!
     * @brief Execute the list of queries defined by the #queries property.
     *
     * The set of #queries will be executed asynchronously. Connect to the error and
     * executed signals to determine the results. If times is greater than 1 then
     * the queries will be executed again after a interval in milliseconds based on the
     * #interval property. This will continue until the set of queries has been executed
     * the number of times defined by the #times property.
     */
    Q_SLOT void execute();

    /*!
     * @brief Stop the execution.
     *
     * This will stop execution after the current execution operation (if any) is done.
     * The execute() method can be called again to start execution over again.
     */
    Q_SLOT void stop();

Q_SIGNALS:
    /*!
     * @brief Emitted when the queries have been executed successfully.
     *
     * The data returned by this signal will be the combined output from the set of
     * queries. For non-select queries there is no output. For select queries the output
     * will be a @c QVariantList containing a @c QVariantMap for each item.
     *
     * @param data The results (if any) from executing the queries.
     */
    void executed(const QVariant& data);

    /*!
     * @brief Emitted when any of the queries has failed execution.
     *
     * The errorType and errorMessage will be taken from @c QSqlError.
     *
     * @param errorType The type of error that was generated.
     * @param errorMessage The detailed error message.
     */
    void error(int errorType, const QString& errorMessage);

private:
    /**
     * Internal query execution method.
     */
    Q_SLOT void executeQueries();

    /**
     * Internal result from sql query execution.
     */
    Q_SLOT void processResult(const bb::data::DataAccessReply& reply);

    /**
     * Private data.
     */
    QUrl m_source;
    QStringList m_queries;
    QVariantMap m_bindValues;
    int m_times;
    int m_interval;
    int m_executionCount;
    bb::data::AsyncDataAccess  *m_asyncSql;
    QTimer *m_timer;

    Q_DISABLE_COPY(QueryExec)
};


/**
 * A QueryWorker that can perform a list of queries in a secondary thread
 * managed by SqlConnection.
 */
class QueryWorker : public bb::data::AsyncWorker {
public:
    /**
     * Constructor.
     */
    QueryWorker(const QUrl &source, QObject* parent);

    /**
     * Destructor.
     */
    virtual ~QueryWorker();

    /**
     * Called to execute queries on secondary thread.
     */
    virtual void execute(const QVariant& command, bb::data::DataAccessReply* replyData);

private:
    /**
     * Add to the results from this executed query. None unless its a select.
     */
    void populateQueryResults(QSqlQuery &sqlQuery, QVariantList *results);

    /**
     * Build result to return to caller in main thread.
     */
    void populateReply(const QVariant &result, const QSqlError &error, bb::data::DataAccessReply *replyData);

    /**
     * Private data.
     */
    QUrl m_source;
};

QML_DECLARE_TYPE(QueryExec)

#endif // BB_CASCADES_DATAMANAGER_QUERYEXEC_HPP
