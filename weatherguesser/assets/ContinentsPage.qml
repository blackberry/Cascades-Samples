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

// The city browse page, filtering is done based on continents.
Page {
    id: continentsPage

    // Signal that will be emitted when an item in the continents list has been selected.
    signal showContinentCities (string name)

    content: Container {
        background: Color.create ("#272727")

        // Page title.
        PageTitle {
            titleText: "Continents"
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }

        // The list of continents, it uses only standard item types and a simple xml model.
        ListView {
            id: continentList

            // The continents is a simple static list so an XML model suffice here.
            dataModel: XmlDataModel {
                source: "models/continent_model.xml"
            }

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            onSelectionChanged: {
                if (selected) {
                    // When an item is selected the show Continent Cities signal is emitted,
                    // this will trigger navigation to the weather page and reload of the weather model.
                    var chosenItem = dataModel.data (indexPath);
                    continentsPage.showContinentCities (chosenItem.name)
                }
            }
        }
    }

    function resetListFocus () {
        continentList.clearSelection ();
    }
}
