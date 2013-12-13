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
#include "customsqldatasource.h"

int const CustomSqlDataSource::LOAD_EXECUTION = 0;

CustomSqlDataSource::CustomSqlDataSource(QObject *parent) :
        QObject(parent), mSqlConnector(0), mDataSource(0)
{
    mConnection = "";
}

CustomSqlDataSource::~CustomSqlDataSource()
{
    delete mSqlConnector;
}

void CustomSqlDataSource::copyFileToDataFolder(const QString fileName)
{
    // Since we need read and write access to the file, it has
    // to be moved to a folder where we have access to it. First,
    // we check if the file already exists (previously copied).
    QString dataFolder = QDir::homePath();
    QString newFileName = dataFolder + "/" + fileName;
    QFile newFile(newFileName);

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/" + fileName;
        QFile originalFile(originalFileName);

        if (originalFile.exists()) {
            // Create sub folders if any creates the SQL folder for a file path like e.g. sql/quotesdb
            QFileInfo fileInfo(newFileName);
            QDir().mkpath (fileInfo.dir().path());

            if(!originalFile.copy(newFileName)) {
                qWarning() << "CustomSqlDataSource::copyFileToDataFolder Failed to copy file to path: " << newFileName;
            }
        } else {
            qWarning() << "CustomSqlDataSource::copyFileToDataFolder Failed to copy file data base file does not exists.";
        }
    }

    mSourceInDataFolder = newFileName;
}


void CustomSqlDataSource::setSource(const QString source)
{
    if (mSource.compare(source) != 0) {
        // Copy the file to the data folder to get read and write access.
        copyFileToDataFolder(source);
        mSource = source;
        emit sourceChanged(mSource);
    }
}

QString CustomSqlDataSource::source()
{
    return mSource;
}

void CustomSqlDataSource::setConnection(const QString connection)
{
    if (mConnection.compare(connection) != 0) {
        // Copy the file to the data folder to get read and write access.
        mConnection = connection;
        emit connectionChanged(mConnection);
    }
}

QString CustomSqlDataSource::connection()
{
    return mConnection;
}


void CustomSqlDataSource::setQuery(const QString query)
{
    if (mQuery.compare(query) != 0) {
        mQuery = query;
        emit queryChanged(mQuery);
    }
}

QString CustomSqlDataSource::query()
{
    return mQuery;
}

bool CustomSqlDataSource::checkConnection()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    if (mSqlConnector) {
        return true;
    } else if(!mConnection.isEmpty()){
        QFile newFile(mSourceInDataFolder);

        if (newFile.exists()) {

            // Remove the old connection if it exists
            if(mSqlConnector){
                disconnect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)), this,
                        SLOT(onLoadAsyncResultData(const bb::data::DataAccessReply&)));
                delete mSqlConnector;
            }

            // Set up a connection to the data base
            mSqlConnector = new SqlConnection(mSourceInDataFolder, mConnection);

            // Connect to the reply function
            connectResult = connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)), this,
                                                   SLOT(onLoadAsyncResultData(const bb::data::DataAccessReply&)));
            Q_ASSERT(connectResult);

            return true;
        } else {
            qWarning() << "CustomSqlDataSource::checkConnection Failed to load data base, file does not exist.";
        }
    } else {
        qWarning() << "CustomSqlDataSource::checkConnection The connection property has to be set on the source or it won't work.";
    }
    return false;
}

void CustomSqlDataSource::execute (const QString& query, const QVariantMap &valuesByName, int id)
{
    if (checkConnection()) {
        mSqlConnector->execute(query, valuesByName, id);
    }
}

bool CustomSqlDataSource::executeAndWait (const QString& query, const QVariantMap &valuesByName, int id)
{
    if (checkConnection()) {
        DataAccessReply reply = mSqlConnector->executeAndWait(query, valuesByName, id);
        if(reply.hasError()){
            return false;
        } else {
            return true;
        }
    }
    return false;
}


void CustomSqlDataSource::load()
{
	if (mQuery.isEmpty() == false) {
		if (checkConnection()) {
			mSqlConnector->execute(mQuery, LOAD_EXECUTION);
		}
	}
}

void CustomSqlDataSource::onLoadAsyncResultData(const bb::data::DataAccessReply& replyData)
{
    if (replyData.hasError()) {
        qWarning() << "CustomSqlDataSource::onLoadAsyncResultData: " << replyData.id() << ", SQL error: " << replyData;
    } else {

        if(replyData.id() == LOAD_EXECUTION) {
            // The reply belongs to the execution of the query property of the data source
            // Emit the the data loaded signal so that the model can be populated.
            QVariantList resultList = replyData.result().value<QVariantList>();
            emit dataLoaded(resultList);
        } else {
            // Forward the reply signal.
            emit reply(replyData);
            qDebug() << "CustomSqlDataSource::onLoadAsyncResultData A reply was recieved with id: " << replyData.id();
        }
    }
}

