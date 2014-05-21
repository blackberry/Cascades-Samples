/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
import bb.cascades 1.3
import bb.cascades.datamanager 1.2
import com.weather 1.0
import "../Common"

// A page used to present a list of cities for a specific continent.
// The real data is managed in code and read from an SQL database.
Page {
    id: continentsCitiesPage
    property NavigationPane nav
    property string continent

    titleBar: WeatherTitleBar {
        id: title
        weatherTitle: continent
    }

    Container {
        background: Color.create("#F1EFE9")

        layout: DockLayout {
        }

        CityList {
            id: cityList
            actAsFavoriteList: false
            dataModel: cityModel

            onChangeFavoriteCity: {
                // Set the city with id as favorite.
                cityDataSource.setFavorite(id, true);
            }

            onUpdateHomeCity: {
                // Update the home city property of the settings object.
                _appSettings.home = city;
            }

            onTriggered: {
                if (indexPath.length > 1) {
                    // When a non header item is selected, we push the Weather page for the city.
                    var chosenItem = dataModel.data(indexPath);
                    var weatherPage = weatherPageDefinition.createObject();
                    weatherPage.setLocation(chosenItem.city);
                    nav.push(weatherPage);
                }
            }
        }
    }

    attachedObjects: [
        // A cached AsyncHeaderDataModel is used in combination with a DataQuery
        // to load data into the model and present in the ListView
        AsyncHeaderDataModel {
            id: cityModel

            // A cache size can be set on the Asynchronous data model, one need to
            // play around with this value until one finds a value that's stable that is
            // does not cause a faulty visual state in the list.
            cacheSize: 50

            // A SQL DataQuery is used to populate the model from a SQL database, see src/common/SqlHeaderDataQueryEx.cpp/.h.
            // Since properties of the Query can only be set once we set them in the setLocation function to make sure we have the
            // name of the city when the database query is constructed.
            query: SqlHeaderDataQueryEx {
                id: cityQuery
                source: "file:///" + _appSettings.homeDir + "/weatherguesser.db";

                // In order for the list to do visual updates that are not jumpy (full refresh of the list)
                // these properties have to be specified and handled properly in the CityDataSource.
                keyColumn: "citiesid"
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision WHERE table_name='cities'"
            }
        },
        CityDataSource {
            id: cityDataSource

            onCitiesChanged: {
                // The entries in the data base have changed update revision to reload.
                cityQuery.emitDataChanged(revision);
            }
        },
        ComponentDefinition {
            id: weatherPageDefinition
            source: "asset:///WeatherPages/CityWeatherPage.qml"
        }
    ]

    function setLocation(cityContinent) {
        // This page is created via a ComponentDefinition so we wait until we know the continent
        // before we can set up the SqlDataQuery properties (they can only be set once).
        continent = cityContinent;

        // The page is showing weather for a specific city and region.
        cityQuery.bindValues = {
            "continent": continent
        }

        if (continent == "All cities") {
            // Special case, the all continents category was selected.
            cityQuery.query = "SELECT * FROM cities ORDER by city";
            cityQuery.countQuery = "SELECT count(*) FROM cities";
            cityQuery.headerQuery = "SELECT substr(city, 1, 1) AS header, count(*) FROM cities GROUP BY header";
            
        } else {
            // Set up a query to get cities belonging to a specific continent.
            cityQuery.query = "SELECT * FROM cities WHERE region=:continent ORDER by city";
            cityQuery.countQuery = "SELECT count(*) FROM cities WHERE region=:continent";
            cityQuery.headerQuery = "SELECT substr(city, 1, 1) AS header, count(*) FROM cities WHERE region=:continent GROUP BY header";            
        }

		// Load the data.        
        cityModel.load();
    }
}
