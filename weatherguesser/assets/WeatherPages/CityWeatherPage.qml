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
import bb.system 1.2
import com.weather 1.0
import "Weather"

// The CityWeatherPage and the HomeWeatherPage are quite similar with one key difference.
// They both use an AsyncDataModel in combination with a SqlDataQuery, but how the actual query
// is set differs. The Home weather has the same query all the time and can be set at creation
// the CityWeatherPage on the other hand is created dynamically and the query has to be set via
// a function call.
WeatherPage {
    property int currentRevision
    dataModel: loadModelDecorator
    loadingItems: loadModelDecorator.loadingItems

    onDataRequest: {
        if (newerItems) {
            // If the list is asking for newer items request them from the data source.
            weatherDataSource.requestWeatherData(item);
        } else {
            // If not asking for newer items refresh the data to make sure its current.
            weatherDataSource.refreshWeatherData();
        }
    }

    onRefreshDone: {
        // The UI refresh animations are done, refresh the data.
        sqlDataQuery.emitDataChanged(currentRevision);
    }

    attachedObjects: [
        LoadModelDecorator {
            id: loadModelDecorator

            model: AsyncDataModel {
                id: weatherModel

                query: SqlDataQuery {
                    id: sqlDataQuery
                    source: "file:///" + _appSettings.homeDir + "/weatherguesser.db"

                    // To handle updates properly (only refresh relevant parts of the list) these properties
                    // needs to be handled and properly set in the database, here its done via the WeatherDataSource
                    keyColumn: "weatherid"
                    revisionColumn: "revision_id"
                    revisionQuery: "SELECT revision_id FROM revision WHERE table_name=:weather"
                }

                onLoaded: {
                    // If no items by this time make request for loading items.
                    var childCount = weatherModel.childCount([]);
                    if (childCount == 0) {
                        weatherDataSource.requestWeatherData(undefined);
                    }
                }
            }
        },
        WeatherDataSource {
            id: weatherDataSource

            onWeatherChanged: {
                // Switch off loading items and set the revision used to update once the UI has been updated.
                currentRevision = revision;
                loadModelDecorator.loadingItems = false;
            }

            onErrorCodeChanged: {
                if (loadModelDecorator.loadingItems && errorCode != WeatherError.NoError && errorCode != undefined) {
                    if (errorCode == WeatherError.ServerError || errorCode == WeatherError.ServerBusy || errorCode == WeatherError.JsonError) {
                        // Just show the user a hud to reload, an improvement on this would be to show different errors depending on the error.
                        errorToast.body = qsTr("Oh ooh, an error occurred. Restart the app and make sure you are connected to the internets.");
                        errorToast.show();
                        loadModelDecorator.temporaryHideDecorator();
                        loadModelDecorator.loadingItems = false;
                    }
                }
            }

            function refreshWeatherData() {
                refresh(city);
                loadModelDecorator.loadingItems = true;
            }

            function requestWeatherData(item) {
                if (item == undefined) {
                    // If there is no date in the list the item will be undefined and an empty string
                    // is sent with the request, it will be interpreted as "today".
                    requestData(city, "");
                } else {
                    // Request new data for the city starting at the date
                    // (should be the date of the last item if at the end of the list).
                    requestData(city, item.date);
                }
                loadModelDecorator.loadingItems = true;
            }
        },
        SystemToast {
            id: errorToast
        }
    ]

    function setLocation(weatherCity) {
        // The properties of a SqlDataQuery can only be set once, so we do it once we have the city.
        city = weatherCity;

        // The page is showing weather for a specific city and region.
        sqlDataQuery.bindValues = {
            "weather": "weather",
            "city": city
        }
        
        // Set up the query (what to load) and the countQuery (how many items in total are in the model).
        sqlDataQuery.query = "SELECT * FROM weather WHERE city=:city ORDER by date ASC";
        sqlDataQuery.countQuery = "SELECT count(*) from weather WHERE city=:city";
        
        weatherModel.load();
    }
}
