/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import com.weather 1.0
import "../../Common"

// The Weather page; where weather data is presented in a list with custom items.
Page {
    id: weather
    property alias city: titleBar.weatherTitle
    property alias dataModel: weatherList.dataModel
    property bool loadingItems: false

    signal dataRequest(variant item, bool newerItems)
    signal refreshDone()

    titleBar: WeatherTitleBar {
        id: titleBar
    }

    Container {
        background: Color.create("#F1EFE9")

        // The list that present the weather data.
        ListView {
            id: weatherList
            property bool scrolling: listscrollStateHandler.scrolling
            property variant topItemIndexPath: listscrollStateHandler.firstVisibleItem
            
            // An XML model is also available to populate the preview.
            dataModel: XmlDataModel {
                source: "../../models/weather.xml"
            }

            // A pull to refresh control that will realign the data so tomorrows weather is on top 
            // if another set of data is stored in the database from a previous run on another day
            leadingVisual: PullToRefresh {
                id: pullToRefresh
                control: weatherList
                preferredHeight: ui.du(20) 
                
                onRefreshActiveChanged: {
                    if (active) {
                        // Once the user has decided to refresh call the data source to add an item.
                        var firstItem = weatherList.dataModel.data([ 0 ]);
                        weather.dataRequest(firstItem, false);
                    } else {
                        // When the refresh is not active anymore emit signal that trigger the DataModel update.
                        weather.refreshDone();
                    }
                }
            }

            // Snap back to the leading edge if less then the full leading visual is shown.
            leadingVisualSnapThreshold: 1.0

            listItemComponents: [
                ListItemComponent {
                    type: "first_item"
                    
                    // First item in the list more detail, bigger item.
                    WeatherTodayItem {
                        temphi: ListItemData.temphi
                        templo: ListItemData.templo
                        icon: ListItemData.icon
                        date: ListItem.view.julianToEpoch(ListItemData.date)
                        moving: ListItem.view.scrolling
                        indexOffset: ListItem.indexPath[0] -ListItem.view.topItemIndexPath[0]
                    }
                },
                ListItemComponent {
                    type: "item"
                    
                    // Other items in the list are smaller and present less detail.
                    WeatherItem {
                        tempaverage: ListItemData.tempaverage
                        icon: ListItemData.icon
                        date: ListItem.view.julianToEpoch(ListItemData.date)
                        moving: ListItem.view.scrolling
                        indexOffset: ListItem.indexPath[0] -ListItem.view.topItemIndexPath[0]                        
                    }
                },
                ListItemComponent {
                    type: "last_item"
                    
                    // Last item in the list shown while loading new data from network.
                    WeatherLoadItem {
                    }
                }
            ]

            function julianToEpoch(julianDate) {
                // The day is stored in Julian date in sqlite and Java script
                // can create dates from epoch time so we convert and return
                // a string representation of the for presentation.
                var inEpoch = (julianDate - 2440587.5) * 86400000
                var date = new Date(inEpoch)
                return date.toDateString()
            }

            attachedObjects: [
                
                // A ListScrollStateHandler is used to monitor when the end is reached in order to load more data.
                ListScrollStateHandler {
                    id: listscrollStateHandler
                    
                    onAtEndChanged: {
                        if (atEnd && firstVisibleItem.length != 0 && loadingItems == false) {
                            var childCount = weatherList.dataModel.childCount(weatherList.rootIndexPath);
                            if (childCount > 0) {
                                var lastItem = weatherList.dataModel.data([ childCount - 2 ]);
                                weather.dataRequest(lastItem, true);
                            }
                        }
                    }
                }
            ]
        }
    }

    onLoadingItemsChanged: {
        if (loadingItems == false) {
            // When new items have been added, loading is done i.e. false.
            // Either disable the pull to refresh control (if loading at the top)
            // or just emit the done signal if loading at the bottom (this signal will
            // be emitted by the pulltorefresh control once it is off screen and we can 
            // load data into the list without getting a jump in position of the list 
            // when refreshing the data.
            if (pullToRefresh.refreshActive) {                
                pullToRefresh.refreshDone();
            } else {
                weather.refreshDone()
            }
        }
    }

    function resetToTop(){
        weatherList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.None);
    }
}
