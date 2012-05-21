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

#ifndef _CITYMODEL_H_
#define _CITYMODEL_H_

#include <bb/cascades/GroupDataModel>
#include <bb/data/SqlConnection>

using namespace bb::data;

namespace bb
{
    namespace data
    {
        class SqlConnection;
    }
}

/* CityModel
 *
 * The city model class extends the group data model with functionality for
 * loading items from an SQL data base. In our case it solves a the particular
 * case of loading setting up a model for a list of cites based on which continent
 * they belong to. It also has the option of loading items that has been marked as
 * favorites in the data base. Manipulating the cities favorites flag can also be done
 * via this data base.
 */
class CityModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT

public:
    /**
     * The constructor is similar to the GroupDataModel constructor but in addition
     * it has a connectionName parameter used for setting up a unique SQL connection.
     *
     * @param keys Value to set on GroupDataModel::sortingKeys.
     * @param connectionName The name of the connection to the SQL database.
     * @param parent The data model owner or @c 0. Optional and defaults to @c 0 if not specified.
     */
    CityModel(const QStringList &keys, const QString& connectionName, QObject *parent = 0);
    ~CityModel();

    /**
     * Loads all items in the data base with favorite=true to the model.
     */
    void loadFavoriteCities();

public slots:
    /**
     * Sets a city as a favorite, updates the database and add it to the model.
     * Only use this function if the CityModel is used for a favorites list or
     * you will get extra items in you list.
     *
     * @param city The name of the city which should be added to favorites.
     */
    void onSetFavoriteCity(QString city);

    /**
     * Sets a city as a not being a favorite, updates the database and add it to the model.
     * Only use this function if the CityModel is used for a favorites list or
     * you will get extra items in you list.
     *
     * @param city The name of the city which should be added to favorites.
     * @param removeIndexPath The index path to the city item in the model.
     */
    void onRemoveFavoriteCity(QString city, QVariant removeIndexPath);

    /**
     * Sets the continent filter for the model. Will load all cities belonging
     * to a particular continent. Can also load all cities if the continent
     * parameter contains "All"
     *
     * @param continent The name of the continent from which the model should be populated.
     */
    void onChangeContinent(QString continent);

private slots:
    /**
     * The reply signal for an asynchronous execute operation.
     *
     * @param reply The reply containing id and data.
     */
    void onLoadAsyncResultData(const bb::data::DataAccessReply& reply);

private:
    /**
     * In order to write to a file in a signed application the file has
     * to reside in the apps data folder. This function copies the bundled
     * data base file to that folder.
     *
     * @param databaseName the name of the data base (in assets/sql).
     */
    bool copyDbToDataFolder(const QString databaseName);

    /**
     * Helper function for updating the data base value for cities that should be
     * favorites.
     *
     * @param city The city that will be updated.
     * @param isFavorite If this parameter is true the city will be a favorite if false it won't.
     */
    bool updateFavoriteCity(QString city, bool isFavorite );

    // Data base connector.
    SqlConnection *mSqlConnector;

    static const char* const mCityDatabase;
};

#endif // ifndef _CITYMODEL_H_
