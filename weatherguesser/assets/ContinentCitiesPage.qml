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

// A page used to present a list of cities on a specific continent.
// The real data is managed in code and read from a SQL database.
Page {
    id: continentsCitiesPage
    property variant nav
    
    titleBar: TitleBar {
        id: title
        visibility: ChromeVisibility.Visible
        title: _cityModel.continent
    }
    
    Container {
        background: Color.create ("#f8f8f8")

        ListView {
            id: citiesList

            signal newFavoriteCity(string city);
            signal updateHomeCity(string city)

            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            // The city GroupDataModel is set up and bound in C++.
            dataModel: _cityModel
            
            // This model can be used instead of the one above to populate the preview.
            /*
            dataModel: XmlDataModel {
                id: cityModel
                source: "models/cities_europe_model.xml"
            }
            */

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: cityItem
                        reserveImageSpace: false

                        title: {
                            ListItemData.name
                        }

                        contextActions: [
                            ActionSet {
                                id: itemActionSet
                                title: ListItemData.name
                                subtitle: "City actions"

                                ActionItem {
                                    title: "Favorite"
                                    imageSource: "asset:///images/menuicons/icon_favorites.png"

                                    onTriggered: {
                                        // Add the current city to the favorite list.
                                        cityItem.ListItem.view.newFavoriteCity(ListItemData.name);
                                    }
                                }

                                ActionItem {
                                    title: "Home city"
                                    imageSource: "asset:///images/menuicons/icon_home.png"

                                    onTriggered: {
                                        // Update the home city to the currently selected item.
                                        cityItem.ListItem.view.updateHomeCity(ListItemData.name);
                                    }
                                }
                            }
                        ]

                    }
                }
            ]
            
            onNewFavoriteCity: {
                // Add a new city to the favorite GroupDataModel. 
                _favoriteModel.onSetFavoriteCity(city);
            }
            
            onUpdateHomeCity: {
				// Update the home city property of the home model, this 
				// triggers a reload of the data for the home city page.
                _homeModel.city = city;
            }
            

            onTriggered: {
                // When an item is selected we push the recipe Page in the chosenItem file attribute.
                var chosenItem = dataModel.data (indexPath);
                
                // A city has been selected the weather model name property change triggers loading of the correct data.
                _weatherModel.city = chosenItem.name;
                
                // Then navigation to the weather  page takes place and the title of the page is updated.
                var weatherPage = nav.deprecatedPushQmlByString ("WeatherPage.qml");
                weatherPage.city = chosenItem.name;
            }
        }
    }
    
    function resetListFocus() {
        citiesList.clearSelection ();
    }    
}
