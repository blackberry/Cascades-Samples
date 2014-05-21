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

// The CityList presents a grouped list of cities.
ListView {
    id: cityList

    property bool actAsFavoriteList: true;

    // The city list can emit two signals: one for removing a city from the list and
    // another for updating the home city.
    signal changeFavoriteCity(variant id)
    signal updateHomeCity(string city)

    dataModel: XmlDataModel {
        source: "asset:///models/cities_europe_model.xml"
    }

    listItemComponents: [
        ListItemComponent {
            type: "header"

            // Custom header see CityHeader.qml
            CityHeader {
                title: ListItemData.header
            }
        },
        ListItemComponent {
            // This is the default type emitted by the AsyncHeaderModel
            type: ""

            CityItem {
                id: cityItem
                title: ListItemData.city

                // Item context actions are items long-pressed on the menu so
                // actions will be shown.
                contextActions: [
                    ActionSet {
                        title: ListItemData.city
                        subtitle: qsTr("City actions") + Retranslate.onLanguageChanged

                        // Set as favorite if the list is not used on the favorite page, in which
                        // case it should remove the item from the list.
                        ActionItem {
                            property bool actAsFavoriteList: cityItem.ListItem.view.actAsFavoriteList
                            property bool isFavorite: (ListItemData.favorite == 1) ? true : false
                            imageSource: "asset:///images/menuicons/icon_favorites.png"

                            // Different titles depending on if in favorite list mode or not.
                            title: (actAsFavoriteList) ? qsTr("Remove from favorites") + Retranslate.onLanguageChanged : qsTr("Add to favorites") + Retranslate.onLanguageChanged

                            // If in the favorite list always enable, if not favorite enable if the item is not already a favorite.
                            enabled: (actAsFavoriteList) ? true : ! isFavorite

                            onTriggered: {
                                cityItem.ListItem.view.changeFavoriteCity(ListItemData.citiesid);
                            }
                        }

                        ActionItem {
                            title: qsTr("Home city") + Retranslate.onLanguageChanged
                            imageSource: "asset:///images/menuicons/icon_home.png"
                            
                            onTriggered: {
                                // Emit signal that will make the current city the home city.
                                cityItem.ListItem.view.updateHomeCity(ListItemData.city);
                            }
                        }
                    }
                ]
            }
        }
    ]
}
