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
#ifndef _CityDataSource_H_
#define _CityDataSource_H_

#include <bb/data/SqlConnection>

/* CityDataSource Description:
 *
 * Helper class for modifying data related to cities.
 *
 */
class CityDataSource: public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param parent The data model owner or @c 0. Optional parameter; defaults to @c 0, if not specified.
     */
    CityDataSource(QObject *parent = 0);

    /**
     * Destructor.
     */
    ~CityDataSource();

    /**
     * This function changes the favorite status of a city.
     *
     * @param id The data base id of the city that should change.
     * @param favorite Boolean if true city should be favorite otherwise not.
     */
    Q_INVOKABLE void setFavorite(QVariant id, bool favorite);

    /**
     * Returns the current global revision of the database plus one.
     *
     * @return An integer with next revision.
     */
    Q_INVOKABLE int incrementRevision();

    /**
     * Returns the current global revision of the database.
     *
     * @return An integer with the current revision.
     */
    Q_INVOKABLE int currentRevision();

signals:
    /**
     * Signal emitted when a change has happened in the favorites data base.
     *
     * @param the current revision number
     */
    void favoritesChanged(int revision);

    /**
     * Signal emitted when new cities have been added to the database.
     */
    void citiesChanged(int revision);

private slots:
    /**
     * Function used to connect to the SqlConnectors reply signal, this is
     * when data base queries have been executed.
     *
     * @param DataAccessReply reply The Database reply
     */
    void onSqlConnectorReply(const bb::data::DataAccessReply& reply);

private:

    // Data base connector.
    bb::data::SqlConnection *mSqlConnector;

    // The last accessed data revision.
    int mRevision;
};

#endif // ifndef _CityDataSource_H_
