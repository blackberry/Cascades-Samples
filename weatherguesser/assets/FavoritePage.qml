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

// A page where a list of favorite cities are shown.
Page {
    id: favorites

    // Signal that will be emitted when an item in the favorites list is selected.
    signal showWeather (string name)

    content: Container {
        background: Color.create ("#272727")

        // Page title.
        PageTitle {
            titleText: "Favorites"
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }

        ListView {
            id: favoritesList
            objectName: "favoritesList"

            // The favorite list can emit two signals, for removing a city from the list and
            // for updating the home city to one of the cities in the favorite list.
            signal removeFavoriteCity (string city, variant removeIndexPath)
            signal updateHomeCity (string city)

            // Spacequota 1 is set so that the list will fit in the space available beneath the title.
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            // A temporary xml model used for getting data in the preview,
            // this model is replaced by a GroupDataModel in code.
            dataModel: XmlDataModel {
                id: cityModel
                source: "models/cities_favorites_model.xml"
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"

                    StandardListItem {
                        id: cityItem
                        titleText: {
                            ListItemData.name
                        }

                        // Item context actions, is an item is longpressed a menu with these
                        // actions will be shown.
                        contextActions: [
                            ActionSet {
                                title: ListItemData.name
                                subtitle: "City actions"

                                ActionItem {
                                    title: "Remove from favorites"
                                    imageSource: "asset:///images/menuicons/icon_favorites.png"
                                    onTriggered: {
                                        // Emit signal that will remove the current city from the favorites list.
                                        cityItem.ListItem.view.removeFavoriteCity (ListItemData.name, cityItem.ListItem.indexPath);
                                    }
                                }
                                ActionItem {
                                    title: "Home city"
                                    imageSource: "asset:///images/menuicons/icon_home.png"
                                    onTriggered: {
                                        console.debug ("Set " + ListItemData.name + " as home");

                                        // Emit signal that will make the current city the home city
                                        cityItem.ListItem.view.updateHomeCity (ListItemData.name);
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

                    // Emit signal that will navigate to the weather page and update it with correct data
                    favorites.showWeather (chosenItem.name);
                }
            }
        }
    }

    function resetListFocus () {
        favoritesList.clearSelection ();
    }
}
