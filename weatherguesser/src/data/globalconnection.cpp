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
#include "globalconnection.h"
#include <bb/data/SqlConnection>
#include <QDir>

using namespace bb::data;

// Static global instance, there will be only one for the application.
static GlobalConnection* mInstance = NULL;

const char* const GlobalConnection::mDatabaseName       = "weatherguesser.db";
const char* const GlobalConnection::mInitDatabasePath   = "models/sql/";
const char* const GlobalConnection::mAssetsPath         = "/app/native/assets/";

GlobalConnection* GlobalConnection::instance()
{
    if (mInstance == NULL) {
        mInstance = new GlobalConnection();
        if(mInstance->initInstance() == false){
            qWarning() << "Initialization of SQL connection failed.";
        }
    }

    // Return the instance.
    return mInstance;
}


bool GlobalConnection::initInstance()
{
    bool result = true;

    // Before we can open the database, we have to make sure it is in the read/write "data" folder.
    result = copyDbToDataFolder(mDatabaseName, mInitDatabasePath);
    QString dataFolder = "data/";

    // Create the SQL connection to the data base in the application data folder.
    mSqlConnection = new SqlConnection(dataFolder + mDatabaseName, "global_connection", mInstance);

    return result;
}


bool GlobalConnection::copyDbToDataFolder(const QString databaseName, const QString fromPath)
{
    // Since we need read and write access to the database, it has
    // to be moved to a folder where we have access to it.
    QString dataFolder = QDir::homePath();
    QString newFileName = dataFolder + "/" + databaseName;
    QFile newFile(newFileName);

    //if (newFile.exists()) newFile.remove(); // for testing overwrite the db every time

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        QString dbAssetFolder(QDir::currentPath() + mAssetsPath + fromPath);
        QString originalFileName = dbAssetFolder + databaseName;
        QFile originalFile(originalFileName);

        if (originalFile.exists()) {
            return originalFile.copy(newFileName);
        } else {
            qDebug() << "Failed to copy file data base file does not exists.";
            return false;
        }
    }

    return true;
}

bb::data::SqlConnection* GlobalConnection::sqlConnection()
{
    return mSqlConnection;
}
