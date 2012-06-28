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

// The city browse page, filtering is done based on continents, the first 
// control is a NavigationPane this in order to drill down in the lists.
NavigationPane {
    id: continentsNav
    
    Page {
        id: continentsPage
        
        titleBar: TitleBar {
            visibility: ChromeVisibility.Visible
            title: "Continents"
        }
        
	    // Signal that will be emitted when an item in the continents list has been selected.
        signal showContinentCities (string name)
        Container {
            background: Color.create ("#f8f8f8")
	
	        // The list of continents, it uses only standard item types and a simple XML model.
            ListView {
                id: continentList
	
	            // The continents is a simple static list so an XML model suffice here.
                dataModel: XmlDataModel {
                    source: "models/continent_model.xml"
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "continent"
                        StandardListItem {
                            reserveImageSpace: false
                            title: {
                                ListItemData.name
                            }
                        }
                    }
                ]
                onTriggered: {
                    // When an item is selected the show Continent Cities signal is emitted,
                    // this will trigger navigation to the weather page and reload of the weather model.
                    var chosenItem = dataModel.data (indexPath);
                    _cityModel.continent = chosenItem.name;
                    var weatherPage = continentsNav.deprecatedPushQmlByString ("ContinentCitiesPage.qml");
                    weatherPage.nav = continentsNav;
                }
            }
        }
        
        function resetListFocus () {
            continentList.clearSelection ();
        }
    }
    
    onTopChanged: {
        // Clear list selection upon returning to a page.
        if (page != mainTab) {
            page.resetListFocus ();
        } else if (page == mainTab) {
            if (mainTab.activeTabPane == favorites || mainTab.activeTabPane == continents) {
                mainTab.activeTabPane.resetListFocus ();
            }
        }
    }
}
