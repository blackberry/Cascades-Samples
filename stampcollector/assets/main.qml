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
    id: nav
    Page {
        id: stampListPage

        content: Container {            
            background: Color.create ("#262626")

            layout: DockLayout {
            }

            ListView {
                id: stampList
                preferredWidth: 700      
                objectName: "stampList"

                // XML model, disable JSON in code and enable this model to use the XML model in models/stamps.xml
                //dataModel: XmlDataModel {
                //    source: "models/stamps.xml"
                //}

                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                // To get a grid like list a multi-level model is used (see models/stamps.xml). The 
                // second level of such a model is layouted as a flow layout. That means that if an 
                // item does not occupy all the width of the list, the list will try to fit more items on that row.
                listItemComponents: [
                    // First level a category item see component in CategoryItem.qml.
                    ListItemComponent {
                        type: "header"
                        CategoryItem {
                        }
                    },
                    // Second level item see the component in StampItem.qml. 
                    ListItemComponent {
                        type: "item"
                        StampItem {
                        }
                    }
                ]
            }
        }
    }
    
    // Signal handler called when the top most control is changed in the NavigationPane.
    onTopChanged: {
        if (pane == stampListPage) {
            // Clear selection when returning to the stamp list page.
            stampList.clearSelection ();
        }
    }
}
