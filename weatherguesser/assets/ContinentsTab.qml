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
import "Cities"
import "Common"

// The city browse page, filtering is done based on continents, the first
// control is a NavigationPane this in order to drill down in the lists.
NavigationPane {
    id: continentsNav
    
    Page {
        id: continentsPage
        
        titleBar: WeatherTitleBar {
            weatherTitle: qsTr("Continents") + Retranslate.onLanguageChanged
        }
        
        // Signal that will be emitted when an item in the continents list has been selected.
        signal showContinentCities(string name)
        
        Container {
            background: Color.create("#F1EFE9")
            
            // The list of continents, it uses a basic custom list item and a simple XML model.
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
                        CityItem {
                            title: {
                                ListItemData.name
                            }
                        }
                    }
                ]
                
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);
                       
                    // Then navigation to a list of the cities in the selected continent.
                    var continentCitiesPage = continentCitiesPageDefinition.createObject();
                    continentCitiesPage.nav = continentsNav;
                    continentCitiesPage.setLocation(chosenItem.name);
                    
                    // Finally push the new Page.
                    continentsNav.push(continentCitiesPage);
                }
            }
        }
    }
    
    onPopTransitionEnded: {
        page.destroy()
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: continentCitiesPageDefinition
            source: "asset:///Cities/CitiesPage.qml"
        }
    ]
}
