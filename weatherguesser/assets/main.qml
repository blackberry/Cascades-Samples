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

NavigationPane {
    id: navigation

    TabbedPane {
        id: mainTab

        // The home weather page, the user can select which city it want to show
        // here by longpressing a city in the lists and setting it as home.
        WeatherPage {
            id: homeCityPage
            objectName: "homeCityPage"
            paneProperties: TabbedPaneProperties {
                title: "Home"
                imageSource: "asset:///images/menuicons/icon_home.png"
            }
        }

        // The city browse page, filtering is done based on continents.
        ContinentsPage {
            id: continents
            objectName: "continents"
            paneProperties: TabbedPaneProperties {
                title: "Browse Cities"
                imageSource: "asset:///images/menuicons/icon_browse.png"
            }
        }

        // A page where a list of favorite cities are shown.
        FavoritePage {
            id: favorites
            objectName: "favorites"
            paneProperties: TabbedPaneProperties {
                title: "Favorites"
                imageSource: "asset:///images/menuicons/icon_favorites.png"
            }
        }

        // A page where the maximum and minimum tempratures are shown (or are they?)
        MaxMinPage {
            paneProperties: TabbedPaneProperties {
                title: "Max/Min"
            }
        }

        // An information page presenting some background for the application.
        InfoPage {
            paneProperties: TabbedPaneProperties {
                title: "Info"
                imageSource: "asset:///images/menuicons/icon_info.png"
            }

            // The info page has an additional action, that is placed in the overflow menu to the right.
            actions: [
                ActionItem {
                    title: "More Info"
                    imageSource: "asset:///images/menuicons/icon_continents.png"
                    onTriggered: {
                        navigation.deprecatedPushQmlByString ("MoreInfoPage.qml");
                    }
                }
            ]
        }
    }

    onTopChanged: {
        // Clear list selection upon returning to a page.
        if (pane != mainTab) {
            pane.resetListFocus ();
        } else if (pane == mainTab) {
            if (mainTab.activeTabPane == favorites || mainTab.activeTabPane == continents) {
                mainTab.activeTabPane.resetListFocus ();
            }
        }
    }
}
