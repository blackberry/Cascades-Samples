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
import bb.cascades 1.0

// The Weather page, where weather data is presented in a list with custom items.
Page {
    id: weather
    
    property alias city: titleBar.title
    property alias weatherData: weatherList.dataModel
    
    titleBar: TitleBar {
        id: titleBar
        visibility: ChromeVisibility.Visible
        title: _weatherModel.city
    }
    
    Container {
        background: Color.create ("#f8f8f8")

        // The list showing weather data and the activity indicator (while loading).
        Container {
            layout: DockLayout {
            }

            // Spacequota 1 is set so that the list will fit in the space available beneath the title.
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            // An activity indicator that will be shown when weather data is loaded.
            ActivityIndicator {
                id: dataLoadIndicator
                preferredWidth: 400
                preferredHeight: 400
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
                }
            }

            // The list of weather forecasts.
            ListView {
                id: weatherList
                dataModel: _weatherModel
                
                // An XML model is used for making it easy to develop, use this to populate the preview.                
                /*dataModel: XmlDataModel {
                    source: "models/weather.xml"
                }*/
                listItemComponents: [
                    ListItemComponent {
                        type: "todayItem"
                        WeatherTodayItem {
                        }
                    },
                    ListItemComponent {
                        type: "item"
                        WeatherItem {
                        }
                    },
                    ListItemComponent {
                        type: "header"
                        HeaderListItem {
                            title: {
                                if (ListItem) ListItemData.toDateString ();
                            }
                        }
                    }
                ]

                // Item type mapping
                function itemType (data, indexPath) {
                    if (indexPath.length == 1) {
                        return 'header';
                    } else if (indexPath.length == 2 && indexPath[0] == 0 && indexPath[1] == 0) {
                        // The activity indicator is stopped once the first item is loaded.
                        dataLoadIndicator.stop ();

                        // The top item is a special item so we till the list it should use that item type.
                        return 'todayItem';
                    } else {
                        return 'item';
                    }
                }
            }
        }
    }
    onCityChanged: {
        // When the city is changed an activity indicator is shown.
        dataLoadIndicator.start ();
    }
}
