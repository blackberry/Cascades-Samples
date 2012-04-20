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

    // Property used for setting the page title from application code.
    property string continent: "Europe"

    // Signal the when emitted will trigger a navigation to the WeatherPage.
    signal showWeather (string name)

    content: Container {
        background: Color.create ("#272727")

        PageTitle {
            titleText: continent
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }

        ListView {
            id: citiesList
            objectName: "citiesList"

            signal newFavoriteCity(string city);
            signal updateHomeCity(string city)

            stickyHeaders: true

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            // This model is replaced in code by a GroupDataModel containing filtered SQL data.
            dataModel: XmlDataModel {
                id: cityModel
                source: "models/cities_europe_model.xml"
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: cityItem

                        titleText: {
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

            onSelectionChanged: {
                if (selected) {
                    // When an item is selected we push the recipe Page in the chosenItem file attribute.
                    var chosenItem = dataModel.data (indexPath);
                    continentsCitiesPage.showWeather(chosenItem.name);
                }
            }
        }
    }

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Names"
            onTriggered: {
                // _navigation is set in code.
                _navigation.pop ();
            }
        }
    }

    function resetListFocus() {
        citiesList.clearSelection ();
    }
}
