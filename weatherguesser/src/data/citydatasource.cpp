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
#include "citydatasource.h"
#include "globalconnection.h"

#include <bb/data/DataAccessReply>
#include <bb/data/SqlConnection>

using namespace bb::data;

CityDataSource::CityDataSource(QObject *parent) :
        QObject(parent), mRevision(0)
{
    // Get the global SQL connection.
    mSqlConnector = GlobalConnection::instance()->sqlConnection();

    bool connectResult = connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)),
            SLOT(onSqlConnectorReply(const bb::data::DataAccessReply&)));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

CityDataSource::~CityDataSource()
{
}

int CityDataSource::incrementRevision()
{
    // Increment the revision stored in the revision table.
    QVariantList dataList;
    dataList << "cities";
    mSqlConnector->executeAndWait("UPDATE revision SET revision_id = revision_id + 1 WHERE table_name=:cities", dataList);

    // Get the new value.
    mRevision = currentRevision();
    return mRevision;
}


int CityDataSource::currentRevision()
{
    // Get the current revision for the cities table, stored in the revision table.
    QVariantList dataList;
    dataList << "cities";
    DataAccessReply reply = mSqlConnector->executeAndWait("SELECT revision_id FROM revision WHERE table_name=:cities", dataList);

    QVariantList resultList = reply.result().toList();
    QVariantMap resultMap = resultList.at(0).toMap();
    int revision = resultMap["revision_id"].toInt();

    return revision;
}

void CityDataSource::onSqlConnectorReply(const bb::data::DataAccessReply& reply)
{
    if (reply.hasError()) {
        qWarning() << "WeatherModel: Database error, " << reply.errorType() << " "
                << reply.errorMessage() << " id: " << reply.id();
    } else {
        if (reply.id() == 0) {
            // The cities data base has changed emit a signal telling what the revision of the change is.
            emit citiesChanged(mRevision);
        }
    }
}

void CityDataSource::setFavorite(QVariant id, bool favorite)
{
    if (id.canConvert(QVariant::Int)) {
        int revision = incrementRevision();
        int isFavorite = favorite == true ? 1 : 0;

        // Update the SQL table with the new isFavorite boolean for the city.
        QVariantList dataList;
        dataList << isFavorite << revision << id.toInt();

        QString query = "UPDATE cities SET favorite=:favorite, revision_id=:revision_id WHERE citiesid=:id";
        mSqlConnector->execute(query, dataList);
    }
}

