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
import "Cities"
import "Common"

// This is a Page where a list of favorite cities are shown. A NavigationPane
// is used to drill down from the list of favorites into a specific
// city and present the weather report of that city.
NavigationPane {
    id: favNav
    
    Page {
        id: favorites

        titleBar: WeatherTitleBar {
            weatherTitle: qsTr("Favorites") + Retranslate.onLanguageChanged
        }

        Container {
            background: Color.create("#F1EFE9")
            
            // The list of favorite cities see Cities/CityList.qml
            CityList {
                dataModel: favModel
                
                onChangeFavoriteCity: {
                    favoriteDataSource.setFavorite(id, false);
                }
                
                onUpdateHomeCity: {
                    // Update the home city property of the settings object.
                    _appSettings.home = city;
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                    
                    // Create the WeatherPage and set its properties, then push it to the NavigaitonPane.
                    var weatherPage = weatherPageDefinition.createObject();   
                    weatherPage.setLocation(chosenItem.city);
                    favNav.push(weatherPage);
                }
            }
        }
    }
    
    onCreationCompleted: {
        favModel.load();
    }
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
    attachedObjects: [
        // A cached AsyncHeaderDataModel is used in combination with a DataQuery
        // to load data into the model and present in the ListView 
        AsyncHeaderDataModel {
            id: favModel
            
            // A cache size can be set on the Asynchronous data model, one need to 
            // play around with this value until one finds a value that's stable that is
            // does not cause a faulty visual state in the list.
            cacheSize: 50
            
            // A the SQL DataQuery is used to populate the model from a SQL database, see src/common/SqlHeaderDataQueryEx.cpp/.h.
            query: SqlHeaderDataQueryEx {
                id: favQuery
                source: "file:///" + _appSettings.homeDir + "/weatherguesser.db";
                
                // The query and the count of items, here we get all cities that are marked as favorites.
                query: "SELECT * FROM cities WHERE favorite=:markedAsfavorite ORDER BY city"
                countQuery: "SELECT count(*) FROM cities WHERE favorite=:markedAsfavorite"
                
                // The query that sets up the hierarchical ordering of items, here we sort on 
                // first letter.
                headerQuery: "SELECT substr(city, 1, 1) AS header, count(*) FROM cities WHERE favorite=:markedAsfavorite GROUP BY header"
                
                // In order for the list to do visual updates that are not jumpy (full refresh of the list)
                // these properties have to be specified and handled properly in the CityDataSource.
                keyColumn: "citiesid"
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision WHERE table_name=:citiesTableName"
                
                // Parametric binding of values.
                bindValues : {
                    "markedAsfavorite": "1",
                    "citiesTableName": "cities"
                }
            }
        },
        // The city data source is used to handle changes in the city database, see src/data/citydatasource.h/.cpp.
        CityDataSource {
            id: favoriteDataSource
            
            onCitiesChanged: {
                // The entries in the data base have changed, update revision to reload.
                favQuery.emitDataChanged(revision);
            }
        },
        ComponentDefinition {
            id: weatherPageDefinition
            source: "asset:///WeatherPages/CityWeatherPage.qml"
        }
    ]
    
    function refreshFavModel(){
        // There might have been changes to favorites while for example browsing the
        // cities list, a call to this function when reentering the favorite page 
        // will ensure that the latest changes are always reflected in the list.
        favQuery.emitDataChanged(favoriteDataSource.currentRevision());
    }
}
