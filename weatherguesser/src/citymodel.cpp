/* Copyright (c) 2012 Research In Motion Limited.
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

#include "citymodel.h"
#include <bb/data/DataAccessReply>
#include <bb/data/SqlConnection>

using namespace bb::cascades;

#define BEGIN_TRANSACTION_ID 1
#define END_TRANSACTION_ID 2
#define ROLLBACK_ID 3
#define UPDATE_FAVORITES 4

#define INITIAL_LOAD_ID 10
#define ASYNCH_LOAD_ID 20

#define ASYNCH_BATCH_SIZE 10

const char* const CityModel::mCityDatabase = "data/weatherguesser.db";

CityModel::CityModel(const QStringList &keys, const QString& connectionName, QObject *parent) :
        GroupDataModel(keys, parent)
{
    // Before we can open the database, we have to make sure it is in the read/write "data" folder.
    copyDbToDataFolder("weatherguesser.db");

    // Set up SQL connection.
    mSqlConnector = new SqlConnection(mCityDatabase, connectionName, this);

    // Connect to the reply signal to onLoadAsyncResultData() slot function,
    // so that we can process the replies that are delivered asynchronously.
    connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)), this,
        SLOT(onLoadAsyncResultData(const bb::data::DataAccessReply&)));

}

CityModel::~CityModel()
{

    if (mSqlConnector->isRunning()) {
        mSqlConnector->stop();
    }
}

bool CityModel::copyDbToDataFolder(const QString databaseName)
{
    // Since we need read and write access to the database, it has
    // to be moved to a folder where we have access to it. First,
    // we check if the file already exists (previously copied).
    QString dataFolder = QDir::homePath();
    QString newFileName = dataFolder + "/" + databaseName;
    QFile newFile(newFileName);

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        // Note: On a debug build, you will be able to write to a data base
        // in the assets folder but that is not possible on a signed application.
        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + "app/native/assets/models/sql/" + databaseName;
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

bool CityModel::updateFavoriteCity(QString city, bool isFavorite)
{
    // Update the SQL table with the new isFavorite boolean for the city.
    QVariantList dataList;
    dataList << isFavorite << city;
    DataAccessReply reply = mSqlConnector->executeAndWait("UPDATE cities SET favorite=:isFavorite WHERE name=:city", dataList, UPDATE_FAVORITES);

    if (reply.hasError()) {
        qWarning() << "updateFavoriteCity error " << reply;
        return false;
    }

    return true;
}

void CityModel::onSetFavoriteCity(QString city)
{
    if (updateFavoriteCity(city, true)) {

        // After setting a new city as favorite, we load the corresponding item data
        // and add it to the model. This will automatically make it appear in the list.
        QVariantList dataList;
        dataList << city;
        DataAccessReply reply = mSqlConnector->executeAndWait("select * from cities WHERE name=:city", dataList, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "onLoadAsyncResultData: " << reply.id() << ", SQL error: " << reply;
        } else {
            QVariantList cityData = reply.result().value<QVariantList>();

            // Check if the city is already in the list, if its not we add it to the model.
            // The first() function used since in our city data there is only one instance of each city.
            QVariantMap addedItem = cityData.first().toMap();
            if (find(addedItem).isEmpty()) {
                insert(addedItem);
            }
        }
    }
}

void CityModel::onRemoveFavoriteCity(QString city, QVariant removeIndexPath)
{
    // Update the database.
    if (updateFavoriteCity(city, false)) {
        // Update model by removing the item with the corresponding city name.
        this->removeAt(removeIndexPath.toList());
    }
}

void CityModel::loadFavoriteCities()
{
    // First of all, we clear the model to make sure no items are lingering.
    this->clear();

    // Request all cities which have their favorite property set to true.
    QVariantList dataList;
    dataList << 1;

    // For the favorites list, we do not load the data asynchronously. We assume that
    // there will not be a huge number of favorite cities, thus executeAndWait is used.
    DataAccessReply reply = mSqlConnector->executeAndWait("select * from cities where favorite=:markedAsFav", dataList, INITIAL_LOAD_ID);

    if (reply.hasError()) {
        qWarning() << "onLoadFavorites load error " << reply;
    } else {
        // If no problem occurred, load the data by calling the slot function directly.
        onLoadAsyncResultData(reply);
    }
}

void CityModel::changeContinent(QString continent)
{
    // Remove previous items from the model, all items will be replaced by
    // cities from the continent given as a parameter.
    this->clear();

    if (continent.contains("All")) {
        // We have one special case where we load "All Cities", if this
        // is selected we do not load all cities at once but instead load
        // an initial batch, then we continue to load the rest in another thread.
        mSqlConnector->beginTransaction(BEGIN_TRANSACTION_ID);

        // We load the first batch in a synchronous fashion, but only request ASYNCH_BATCH_SIZE items.
        // The rest of the cities will be loaded asynchronously in another thread by issuing execute()
        // on the SqlConnection (see the onLoadAsyncResultData function below).
        QVariantList dataList;
        dataList << QVariant(ASYNCH_BATCH_SIZE);
        qDebug() << dataList;
        DataAccessReply reply = mSqlConnector->executeAndWait("select * from cities limit :limit", dataList, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "initial load error: " << reply;
            mSqlConnector->rollbackTransaction(ROLLBACK_ID);
        } else {
            onLoadAsyncResultData(reply);

            // Post a request that will be performed asynchronous.
            QVariantList dataList;
            dataList << ASYNCH_BATCH_SIZE << ASYNCH_BATCH_SIZE;
            mSqlConnector->execute("select * from cities limit :limit offset :offset", dataList, INITIAL_LOAD_ID);

            mSqlConnector->endTransaction(END_TRANSACTION_ID);
        }
    } else {
        // When loading a specific continent, we load the entire list synchronously.
        QVariantList dataList;
        dataList << continent;
        DataAccessReply reply = mSqlConnector->executeAndWait("select * from cities where continent=:continent", dataList, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "onChangeContinent load error for " << continent << " " << reply;
        } else {
            // Load result directly into the model.
            onLoadAsyncResultData(reply);
        }
    }
}

void CityModel::onLoadAsyncResultData(const bb::data::DataAccessReply& reply)
{
    if (reply.hasError()) {
        qWarning() << "onLoadAsyncResultData: " << reply.id() << ", SQL error: " << reply;
    } else {
        if (reply.id() >= INITIAL_LOAD_ID) {
            QVariantList resultList = reply.result().value<QVariantList>();

            if (resultList.size() > 0) {
                // Insert the data into the model.
                this->insertList(resultList);

                if (reply.id() >= ASYNCH_LOAD_ID) {
                    // If the ID belongs to an asynchronous request, we keep querying until there are no more results.
                    // Increase the offset by using the reply IDs.
                    QVariantList dataList;
                    dataList << ASYNCH_BATCH_SIZE << (ASYNCH_BATCH_SIZE * (reply.id() - ASYNCH_LOAD_ID + 2));
                    mSqlConnector->execute("select * from cities limit :limit offset :offset", dataList, (reply.id() + 1));
                }
            }
        }
    }
}

void CityModel::setContinent(QString continent)
{
    qDebug() << "setContinent " << continent;

    if (mContinent.compare(continent) != 0) {
        changeContinent(continent);
        mContinent = continent;
        emit continentChanged();
    }
}

QString CityModel::continent()
{
    return mContinent;
}

