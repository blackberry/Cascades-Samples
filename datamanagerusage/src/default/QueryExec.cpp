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
#include "QueryExec.hpp"
#include <bb/cascades/datamanager/SqlQueryUtils>
#include <bb/UIToolkitSupport>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

//using namespace bb::data;

//used for import of the QueryExec module into QML
static const char *MODULE_NAME = "bb.cascades.datamanager";
static const int MAJOR_VERSION = 1;
static const int MINOR_VERSION = 2;

//used for naming the query parameters
static const char *QUERIES_KEY = "Q";
static const char *VALUES_KEY = "V";

QueryExec::QueryExec(QObject *parent) :
        QObject(parent), m_times(1), m_interval(1000), m_executionCount(0), m_asyncSql(NULL), m_timer(NULL) {
}

QueryExec::~QueryExec() {
    qDebug() << "QueryExec destructor";
}

void QueryExec::registerQmlTypes() {
    qmlRegisterType<QueryExec>(MODULE_NAME, MAJOR_VERSION, MINOR_VERSION, "QueryExec");
}


/**********************************************************
  *Property getters and setters.
 */
QUrl QueryExec::source() const {
    return m_source;
}

void QueryExec::setSource(const QUrl &newSource) {
    m_source = newSource;
}

QStringList QueryExec::queries() const {
    return m_queries;
}

void QueryExec::setQueries(const QStringList &queryList) {
    m_queries = queryList;
}

QVariantMap QueryExec::valuesToBind() const {
    return m_bindValues;
}

void QueryExec::setValuesToBind(const QVariantMap &nameValueMap) {
    m_bindValues = nameValueMap;
}

int QueryExec::times() const {
    return m_times;
}

void QueryExec::setTimes(int times) {
    m_times = times;
}

int QueryExec::interval() const {
    return m_interval;
}

void QueryExec::setInterval(int interval) {
    m_interval = interval;
}

/**********************************************************
  *Query execution methods.
 */
void QueryExec::execute() {
    // Create an AsyncDataAccess to establish a connection thread to a db
    // and process multiple queries during the life of the application.
    if (m_asyncSql == NULL) {
        // A) Establish connection thread to database
        QueryWorker *qw = new QueryWorker(m_source, NULL);
        m_asyncSql = new bb::data::AsyncDataAccess(qw, this);

        // B) Use signals and slots to get results data when it's ready
        bool res = connect(m_asyncSql, SIGNAL(reply(const bb::data::DataAccessReply&)), this, SLOT(processResult(const bb::data::DataAccessReply&)));
        Q_ASSERT(res); // check connection in debug mode
        Q_UNUSED(res); // not used in release mode
    }

    // execute queries the first time
    m_executionCount = 0;
    executeQueries();

    // If m_times param indicates we wish to execute multiple times
    // then setup a timer to do that.
    if (m_timer == NULL && 1 < m_times) {
        m_timer = new QTimer(this);
        bool res = connect(m_timer, SIGNAL(timeout()), this, SLOT(executeQueries()));
        Q_ASSERT(res); // check connection in debug mode
        Q_UNUSED(res); // not used in release mode
        m_timer->start(m_interval);
    }
}

void QueryExec::stop() {
    m_executionCount = 0;
    if (m_timer != NULL) {
        m_timer->stop();
        m_timer = NULL;
    }
}

void QueryExec::executeQueries() {
    qDebug() << "QueryExec executeQueries()";
    // validate
    if (m_source.isEmpty()) {
        Q_EMIT error(bb::data::DataAccessErrorType::SourceNotFound, "Mandatory source property not defined");
    }
    else if (m_queries.isEmpty()) {
        Q_EMIT error(bb::data::DataAccessErrorType::OperationFailure, "Mandatory queries property not defined");
    }
    else if (m_times <= m_executionCount) {
        stop();
    }
    else {
        // execute
        QVariantMap queriesAndValues;
        queriesAndValues[QUERIES_KEY] = m_queries;
        queriesAndValues[VALUES_KEY] = m_bindValues;
        m_asyncSql->execute(queriesAndValues);
    }
}

void QueryExec::processResult(const bb::data::DataAccessReply &reply) {
    ++m_executionCount;
    //qDebug() << "QueryExec processResult: reply=" << reply << ",m_executionCount=" << m_executionCount;
    if (reply.hasError()) {
        qWarning() << "Error while retrieving data from SQL database: " << reply;
        Q_EMIT error(reply.errorType(), reply.errorMessage());
    }
    else {
        Q_EMIT executed(reply.result());
    }
}


/******************************************************************************
  *A QueryWorker that can perform a list of queries in a secondary thread
  *managed by SqlConnection.
 */
QueryWorker::QueryWorker(const QUrl &source, QObject *parent)
        : AsyncWorker(parent), m_source(source) {
}

QueryWorker::~QueryWorker() {
    qDebug() << "QueryWorker destructor";
}

void QueryWorker::execute(const QVariant &command, bb::data::DataAccessReply *replyData) {
    qDebug() << "QueryWorker::execute";
    // get the open db connection and start a transaction
    bb::cascades::datamanager::SqlQueryUtils squ;
    QSqlError error;
    QSqlDatabase connection = squ.connection(m_source, &error);
    if (error.type() != QSqlError::NoError) {
        populateReply(QVariant(), error, replyData);
        return;
    }
    connection.transaction();

    // command includes a list of queries and a value bindings map
    QVariantMap queriesAndValues = command.toMap();
    QStringList queries = queriesAndValues[QUERIES_KEY].toStringList();
    QVariantMap bindingValues = queriesAndValues[VALUES_KEY].toMap();

    // execute the queries
    QVariantList combinedResults;
    for (int i=0; i < queries.size(); ++i) {
        qDebug() << "executing query:  " << queries[i] << ", bindings: " << bindingValues;
        QSqlQuery sqlQuery(connection);
        squ.prepareQuery(queries[i], bindingValues, &sqlQuery);
        if (!sqlQuery.exec()) {
            populateReply(QVariant(), sqlQuery.lastError(), replyData);
            connection.rollback();
            return;
        }
        populateQueryResults(sqlQuery, &combinedResults);
    }

    if (connection.commit()) {
        populateReply(combinedResults, error, replyData);
    }
    else {
        qWarning() << "Could not commit transaction for database " << connection.databaseName();
        populateReply(QVariant(), connection.lastError(), replyData);
        connection.rollback();
    }
}

void QueryWorker::populateQueryResults(QSqlQuery &sqlQuery, QVariantList *results) {
    QList<QString> field;
    while (sqlQuery.next()) {
        // get the field names
        if (field.isEmpty()) {
            QSqlRecord record = sqlQuery.record();
            for (int i = 0, n = record.count(); i < n; i++) {
                field.append(record.fieldName(i));
            }
        }

        QVariantMap recordMap;
        for (int i = 0, n = field.size(); i < n; i++) {
            QVariant value = sqlQuery.value(i);
            // JavaScript/QML doesn't support 64bit integers (everything is converted to double)
            // so convert to string here to make sure we don't run into trouble.
            // In the long term it will be cleaner to move this code into something like a
            // query decorator instead of the core query code.
            if (value.type() == QVariant::LongLong || value.type() == QVariant::ULongLong) {
                value = value.toString();
            }
            recordMap.insert(field.at(i), value);
        }
        results->append(recordMap);
    }
}

void QueryWorker::populateReply(const QVariant &result, const QSqlError &error, bb::data::DataAccessReply *replyData) {
    // build and return reply
    if (error.type() == QSqlError::NoError) {
        replyData->setResult(result);
    }
    else {
        replyData->setError(bb::data::DataAccessErrorType::OperationFailure,  error.databaseText() + " / " + error.driverText());
    }
    qDebug() << "QueryWorker populateReply: replyData=" << *replyData;
}

