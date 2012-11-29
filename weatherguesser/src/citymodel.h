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

/* CityModel Description:
 *
 * The city model class extends the GroupDataModel with functionality for
 * loading items from an SQL database. In our case, this approach solves the case of 
 * loading and setting up a model for a list of cities based on which continent
 * they belong to. It also has the option of loading items that has been marked as
 * favorites in the database. Manipulating the city's favorites flag can also be done
 * via the database.
 */
class CityModel: public bb::cascades::GroupDataModel
{
    Q_OBJECT

    Q_PROPERTY(QString continent READ continent WRITE setContinent
            NOTIFY continentChanged)

public:
    /**
     * The constructor is similar to the GroupDataModel constructor but in addition
     * it has a connectionName parameter used for setting up a unique SQL connection.
     *
     * @param keys Value to set on GroupDataModel::sortingKeys.
     * @param connectionName The name of the connection to the SQL database.
     * @param parent The data model owner or @c 0. Optional parameter; defaults to @c 0, if not specified.
     */
    CityModel(const QStringList &keys, const QString& connectionName,
            QObject *parent = 0);
    ~CityModel();

    /**
     * This function loads all items in the database where favorite=true to the model.
     */
    void loadFavoriteCities();

    /**
     * This function sets a new continent based on the parameter passed.
     *
     * @param continent A QString of the name of the new continent
     */
    void setContinent(QString continent);

    QString continent();

    signals:
    void continentChanged();

public slots:
    /**
     * This public Slot function sets a city as a favorite, updates the database,
     * and adds it to the model. Only use this function if the CityModel is used for the
     * favorites list otherwise you will get extra items in you list (if this function is used).
     *
     * @param city The name of the city which should be added to favorites.
     */
    void onSetFavoriteCity(QString city);

    /**
     * This public Slot function sets a city as a not being a favorite, updates the database,
     * and adds it to the model. Again, only use this function if the CityModel is used for the
     * favorites list, otherwise you will get extra items in you list.
     *
     * @param city The name of the city which should be added to favorites.
     * @param removeIndexPath The index path to the city item in the model.
     */
    void onRemoveFavoriteCity(QString city, QVariant removeIndexPath);

private slots:
    /**
     * This Slot function uses reply signal for an asynchronous execute operation.
     * The result data from the reply signal is loaded into the model and an execute
     * operation is performed until no more results are available.
     *
     * @param reply The reply containing id and data.
     */
    void onLoadAsyncResultData(const bb::data::DataAccessReply& reply);

private:
    /**
     * This function copies the bundled database file to the proper folder.
     * In order to write to a file in a signed application, the file has
     * to reside in the apps data folder.
     *
     * @param databaseName The name of the database (in assets/sql).
     * @return True if successful, false otherwise.
     */
    bool copyDbToDataFolder(const QString databaseName);

    /**
     * This function sets the continent filter for the model. It will load all cities
     * belonging to a particular continent. Also it can load all cities if the continent
     * parameter contains "All"
     *
     * @param continent The name of the continent from which the model should be populated.
     */
    void changeContinent(QString continent);

    /**
     * This helper function is for updating the database value for cities that should be
     * favorites.
     *
     * @param city The city that will be updated.
     * @param isFavorite If this parameter is true, the city will be a favorite. If false, the city won't be a favorite.
     * @return True if successful, false otherwise.
     */
    bool updateFavoriteCity(QString city, bool isFavorite);

    // Data base connector
    SqlConnection *mSqlConnector;

    static const char* const mCityDatabase;

    QString mContinent;
};

#endif // ifndef _CITYMODEL_H_
