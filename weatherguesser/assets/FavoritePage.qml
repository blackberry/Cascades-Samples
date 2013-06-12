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

// This is a Page where a list of favorite cities are shown. A NavigationPane
// is used to drill down from the list of favorites into a specific
// city and present the weather report of that city.

NavigationPane {
    id: favNav
    
    Page {
        id: favorites
        
        titleBar: TitleBar {
            visibility: ChromeVisibility.Visible
            title: "Favorites"
        }
        
        Container {
            
            ListView {
                id: favoritesList

                // The favorite list can emit two signals: one for removing a city from the list and
                // another for updating the home city to one of the cities in the favorite list.
                signal removeFavoriteCity(string city, variant removeIndexPath)
                signal updateHomeCity(string city)

                // Spacequota 1 is set so that the list will fit in the space available beneath the title.
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                // The favorite GroupDataModel is set up and bound in C++.
                dataModel: _favoriteModel

                // A temporary XML model can be used for getting data in the preview,
                //dataModel: XmlDataModel {
                //    id: cityModel
                //    source: "models/cities_favorites_model.xml"
                //}
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        StandardListItem {
                            id: cityItem
                            imageSpaceReserved: false
                            title: ListItemData.name

                            // Item context actions are items long-pressed on the menu so
                            // actions will be shown.
                            contextActions: [
                                ActionSet {
                                    title: ListItemData.name
                                    subtitle: qsTr("City actions") + Retranslate.onLanguageChanged
                                    ActionItem {
                                        title: qsTr("Remove from favorites") + Retranslate.onLanguageChanged
                                        imageSource: "asset:///images/menuicons/icon_favorites.png"

                                        onTriggered: {
                                            // Emit signal that will remove the current city from the favorites list.
                                            cityItem.ListItem.view.removeFavoriteCity(ListItemData.name, cityItem.ListItem.indexPath);
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Home city") + Retranslate.onLanguageChanged
                                        imageSource: "asset:///images/menuicons/icon_home.png"

                                        onTriggered: {
                                            // Emit signal that will make the current city the home city.
                                            cityItem.ListItem.view.updateHomeCity(ListItemData.name);
                                        }
                                    }
                                }
                            ]// contextActions
                        }// StandardListItem
                    }// ListItemComponent
                ]// listItemComponents
                
                onRemoveFavoriteCity: {
                    // Remove a city from the favorites list.
                    _favoriteModel.onRemoveFavoriteCity(city, removeIndexPath);
                }
                
                onUpdateHomeCity: {
                    // Update the home city property of the home model, this
                    // triggers a reload of the data for the home city page.
                    _homeModel.city = city;
                }
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);

                    // A city has been selected, the weather model name property change triggers loading of the correct data.
                    _weatherModel.city = chosenItem.name

                    // Then navigate to the weather page and update the title of the page.
                    var weatherPage = weatherPageDefinition.createObject();
                    weatherPage.city = chosenItem.name;
                    weatherPage.weatherData = _weatherModel;

                    // Finally push the new Page.
                    favNav.push(weatherPage);
                }
            }
        }
        
        function resetListFocus() {
            favoritesList.clearSelection();
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: weatherPageDefinition
            source: "WeatherPage/WeatherPage.qml"
        }
    ]
    
    onTopChanged: {
        
        // Clear list selection upon returning to a page.
        if (page != mainTab) {
            page.resetListFocus();
        } else if (page == mainTab) {
            if (mainTab.activeTabPane == favorites || mainTab.activeTabPane == continents) {
                mainTab.activeTabPane.resetListFocus();
            }
        }
    }
}
