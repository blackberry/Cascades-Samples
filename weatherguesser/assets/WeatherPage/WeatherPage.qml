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
import bb.cascades 1.2

// The Weather page; where weather data is presented in a list with custom items.
Page {
    id: weather
    property alias city: titleBar.title
    property alias weatherData: weatherList.dataModel
    
    titleBar: TitleBar {
        id: titleBar
        visibility: ChromeVisibility.Visible
        title: "Göteborg"
    }
    Container {

        // The list showing weather data and the activity indicator (while loading)
        Container {
            layout: DockLayout {
            }

            // Spacequota 1 is set so that the list will fit in the space available beneath the title.
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            // An activity indicator that will be shown when weather data is loaded
            InlineActivityIndicator {
            	id: dataLoadIndicator
                indicatorText: qsTr("Loading...") + Retranslate.onLanguageChanged
            }
            
            // The list of weather forecasts
            ListView {
                id: weatherList

                // An XML model is also available to populate the preview.
                dataModel: XmlDataModel {source: "../models/weather.xml"}

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
                        type: "loadItem"
                        WeatherLoadItem {
                            property bool itemInitilized: ListItem.initialized;
                            onItemInitilizedChanged: {
                                if(itemInitilized){
                                    // When the item has been initilized request more data
                                    ListItem.view.dataModel.requestData();
                                }
                            }
                        }
                    }
                ]                

                // Item type-mapping
                function itemType(data, indexPath) {
                    if (indexPath.length == 1) {
                        if (indexPath[0] < weatherList.dataModel.childCount(0) - 1) {
                            if (indexPath[0] == 0) {
                                // The activity indicator is stopped once the first item is loaded.
                                dataLoadIndicator.stopIndicator();

                                // The top item is a special item so we tell the list to use that item type.
                                return 'todayItem';
                            } else {
                                return 'item';
                            }
                        } else {
                            // The last item is replaced by a special item while more data is fetched.
                            return 'loadItem'
                        }
                    }
                    return 'item';
                }
            }
        }
    }
    onCityChanged: {
        // When the city is changed, an activity indicator is shown.
        dataLoadIndicator.startIndicator();
    }
    
    function resetToTop(){
        weatherList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
    }
}
