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
#ifndef GLOBALCONNECTION_H_
#define GLOBALCONNECTION_H_

#include <QObject>

namespace bb
{
    namespace data
    {
        class SqlConnection;
    }
}

/**
 * GlobalConnection Description:
 *
 * A singleton class used for setting up a one global SqlConnection
 * for the entire application.
 */
class GlobalConnection: public QObject
{
Q_OBJECT

public:

    /**
     * Returns the single application UiValues instance.
     *
     * @return An instance of the Global context object
     */
    static GlobalConnection* instance();

    /**
     * Returns the instance SQL connection, only one per app.
     *
     * @return A pointer to an SQL connection.
     */
    bb::data::SqlConnection *sqlConnection();

private:

    /**
     * This function copies the bundled database file to the proper folder.
     * In order to write to a file in a signed application, the file has
     * to reside in the apps data folder.
     *
     * @param databaseName The name of the database.
     * @param fromPath The path in the assets folder (e.g. sql/ if the database is located in assets/sql).
     * @return True if successful, false otherwise.
     */
    bool copyDbToDataFolder(const QString databaseName, const QString fromPath);

    /**
     * Sets up the member variables of the object.
     *
     * @return true if the set up was successful.
     */
    bool initInstance();

    // The SQL connection
    bb::data::SqlConnection *mSqlConnection;

    // Constants pointing the way to the location of the database.
    static const char* const mDatabaseName;
    static const char* const mAssetsPath;
    static const char* const mInitDatabasePath;
};

#endif /* GLOBALCONNECTION_H_ */
