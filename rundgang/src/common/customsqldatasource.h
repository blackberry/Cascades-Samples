/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef QUOTESSQLDATASOURCE_H_
#define QUOTESSQLDATASOURCE_H_

#include <QObject>
#include <bb/data/DataSource>
#include <bb/data/SqlConnection>

using namespace bb::data;

namespace bb
{
    namespace data
    {
        class SqlConnection;
    }
}
/**
 * QuotesSqlDataSource Description:
 *
 * A custom data source for accessing a SQL data source, with the ability
 * to execute SQL queries. Can for example be used to populate a model
 * in QML.
 */
class CustomSqlDataSource: public QObject
{
    Q_OBJECT

    /**
     * The path to the SQL database.
     */
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

    /**
     * The connection name to the SQL database, if having many connections to the same
     * database its important that this name is different for the different connections.
     */
    Q_PROPERTY(QString connection READ connection WRITE setConnection NOTIFY connectionChanged)

    /**
     * The initial query that will be run on the database.
     */
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)

public:
    CustomSqlDataSource(QObject *parent = 0);
    ~CustomSqlDataSource();

    /**
     * Sets the path to the SQL database.
     *
     * @param source the new string.
     */
    void setSource(const QString source);

    /**
     * Returns the current source path used by the data source.
     *
     * @return A string for the current source path.
     */
    QString source();

    /**
     * Sets the connection name that should be used when connecting to the SQL
     * database.
     *
     * @param connection the new string.
     */
    void setConnection(const QString connection);

    /**
     * Returns the current connection name used by the SQLConnection.
     *
     * @return The connection name.
     */
    QString connection();

    /**
     * Sets the SQL query that should be executed.
     *
     * @param query The new SQL query.
     */
    void setQuery(const QString query);

    /**
     * The query property contains an SQL query.
     *
     * @return A string containing the query.
     */
    QString query();


    /**
     * Loads the data from the data source.
     */
    Q_INVOKABLE void load();

    /**
     * Executes a SQL query on the database the execution is non blocking the result will be delivered
     * in the dataLoaded signal. The id has to greater than or equal to 1 (0 is reserved
     * for loading data using the query property)
     *
     * @param A parameterized query which should be executed (see execute function on SqlConnection)
     * @param valuesByName The data corresponding to the parameterized query
     * @param An id that can be used to match requests
     */
    Q_INVOKABLE void execute (const QString& query, const QVariantMap &valuesByName, int id = 1);

    /**
     * Executes a SQL query on the database the execution is blocking and suitable only if
     * it is known that it will be fast, for example when addressing a single item in the data base.
     *
     * @param A parameterized query which should be executed (see execute function on SqlConnection)
     * @param valuesByName The data corresponding to the parameterized query
     * @param An id that can be used to match requests
     */
    Q_INVOKABLE bool executeAndWait (const QString& query, const QVariantMap &valuesByName, int id = 1);

signals:

    /**
     * Emitted when the source path changes.
     *
     * @param A string containing the new source.
     */
    void sourceChanged(QString source);

    /**
     * Emitted when the connection name changes.
     *
     * @param The new connection name.
     */
    void connectionChanged(QString connection);

    /**
     * Emitted when the query changes.
     *
     * @param A string containing the new query.
     */
    void queryChanged(QString query);

    /**
     * Emitted when data has been received.
     *
     * @param A variant containing the new data.
     */
    void dataLoaded (const QVariant &data);

    /**
     * Emitted when an asynchronous execute operation has completed and has results to return.
     *
     * @param replyData The reply data from the execute operation.
     */
    void reply (const bb::data::DataAccessReply &replyData);

private slots:
    /**
     * Function that is connected to the SqlConnection reply signal.
     *
     * @param reply The reply data delivered from the asynchronous request to the SqlConnection
     */
    void onLoadAsyncResultData(const bb::data::DataAccessReply& reply);

private:
    /**
     * Helper function for moving a file from the assets folder to the
     * read writable application data folder.
     *
     * @param fileName The name of the file that should be moved.
     */
    void copyFileToDataFolder(const QString fileName);
    bool checkConnection();

    // Property variables.
    QString mSource;
    QString mQuery;
    QString mSourceInDataFolder;
    QString mConnection;

    // Data base connector
    SqlConnection *mSqlConnector;

    DataSource *mDataSource;

    static int const LOAD_EXECUTION;

};

#endif /* QUOTESDATASOURCE_H_ */
