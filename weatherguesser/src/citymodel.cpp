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
#include <bb/data/SqlDataAccess>

using namespace bb::cascades;

#define BEGIN_TRANSACTION_ID 1
#define END_TRANSACTION_ID 2
#define ROLLBACK_ID 3
#define UPDATE_FAVORITES 4

#define INITIAL_LOAD_ID 10
#define ASYNCH_LOAD_ID 20

#define ASYNCH_BATCH_SIZE 10

const char* const CityModel::mCityDatabase = "app/native/assets/models/sql/weatherguesser.db";

CityModel::CityModel(const QStringList &keys, const QString& connectionName, QObject *parent) :
        GroupDataModel(keys, parent)
{
    //Set up SQL connection, create function that loads new result into the model
    mSqlConnector = new SqlConnection(mCityDatabase, connectionName, this);

    // Connect to the reply signal, so that we can process the replies that are delivered asynchronously.
    connect( mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)),
        this, SLOT(onLoadAsyncResultData(const bb::data::DataAccessReply&)) );
}

CityModel::~CityModel()
{

    if (mSqlConnector->isRunning()) {
        mSqlConnector->stop();
    }
}

bool CityModel::updateFavoriteCity(QString city, bool isFavorite)
{
    // Update the SQL table.
    QString query;

    QTextStream(&query) << "UPDATE cities SET favorite='" << isFavorite << "' WHERE name='" << city << "'";
    DataAccessReply reply = mSqlConnector->executeAndWait(query, UPDATE_FAVORITES);

    if (reply.hasError()) {
        qWarning() << "updateFavoriteCity error " << reply;
        return false;
    }

    return true;
}

void CityModel::onSetFavoriteCity(QString city)
{
    if (updateFavoriteCity(city, true)) {

        // After setting a new city as favorite we load the corresponding item data
        // and add it to the model, this will automatically make it appear in the list.
        QString query = "select * from cities WHERE name='" + city + "'";
        DataAccessReply reply = mSqlConnector->executeAndWait(query, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "onLoadAsyncResultData: " << reply.id() << ", Sql error: " << reply;
        } else {
            QVariantList cityData = reply.result().value<QVariantList>();

            // Check if the city is already in the list, if its not we add it to the model, first is 
            // used since in our city data there is only one instance of each city.
            QVariantMap addedItem = cityData.first().toMap();
            if (this->find(addedItem).isEmpty()) {
                this->insert(addedItem);
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
    // First of all we clear the model to make sure no items are lingering.
    this->clear();

    // Request all cities which has the favorite property set to true.
    QString query = "select * from cities where favorite='1'";

    // For the favorites list we do not load the data asynchronously, we assume that
    // there will not be a huge number of favorite cities, thus executeAndWait is used.
    DataAccessReply reply = mSqlConnector->executeAndWait(query, INITIAL_LOAD_ID);

    if (reply.hasError()) {
        qWarning() << "onLoadFavorites load error " << reply;
    } else {
        // If no problem occurred load the data by calling the slot function directly.
        onLoadAsyncResultData(reply);
    }
}

void CityModel::onChangeContinent(QString continent)
{
    QString query;

    // Remove previous items from the model, all items will be replaced by
    // cities from the continent given as a parameter.
    this->clear();

    if (continent.contains("All")) {
        // We have one special case where we load "All Cities", if this 
        // is selected we do not load all cities at once but instead load 
        // an initial batch, then we continue to load the rest in another thread.
        mSqlConnector->beginTransaction(BEGIN_TRANSACTION_ID);

        // We load the first batch in a synchronous fashion, but only request ASYNCH_BATCH_SIZE items.
        // the rest will be loaded asynchronously in another thread by issuing execute() on the SqlConnection
        // see the onLoadAsyncResultData function below.
        QTextStream(&query) << "select * from cities limit " << ASYNCH_BATCH_SIZE;
        DataAccessReply reply = mSqlConnector->executeAndWait(query, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "initial load error: " << reply;
            mSqlConnector->rollbackTransaction(ROLLBACK_ID);
        } else {
            onLoadAsyncResultData(reply);

            // Post a request that will be performed asynchronous.
            query.clear();
            QTextStream(&query) << "select * from cities limit " << ASYNCH_BATCH_SIZE << " offset " << +ASYNCH_BATCH_SIZE;
            mSqlConnector->execute(query, ASYNCH_LOAD_ID);

            mSqlConnector->endTransaction(END_TRANSACTION_ID);
        }
    } else {
        // When loading a specific continent, we load the entire list synchronously.
        query = "select * from cities where continent='" + continent + "'";
        DataAccessReply reply = mSqlConnector->executeAndWait(query, INITIAL_LOAD_ID);

        if (reply.hasError()) {
            qWarning() << "onChangeContinent load error for " << continent << " " << reply;
        } else {
            // Load result directly.
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
                this->insert(resultList);

                if (reply.id() >= ASYNCH_LOAD_ID) {
                    // If the id belongs to an asynchronous request we keep asking until there are no more results.
                    QString query;

                    // We increase the offset by using the reply id:s
                    QTextStream(&query) << "select * from cities limit " << ASYNCH_BATCH_SIZE <<
                    " offset " << +(ASYNCH_BATCH_SIZE * (reply.id() - ASYNCH_LOAD_ID + 2));
                    mSqlConnector->execute(query, (reply.id() + 1));
                }
            }
        }
    }
}
