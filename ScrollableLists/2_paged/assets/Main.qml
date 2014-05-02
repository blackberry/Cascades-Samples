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
import "Common"

NavigationPane {
    id: nav
    
    Page {
        id: listPage
        objectName: "listPage"
        
        content: Container {
            
            background: Color.create ("#3a8485")
            preferredWidth: 768
            layout: DockLayout {
            }
            
            Container {
                id: cppContainer
                objectName: "cppContainer"
                overlapTouchPolicy: OverlapTouchPolicy.Allow
                preferredWidth: 760
                preferredHeight: 170
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top 

                layout: DockLayout {   
                }
            }
            
            Container {
                overlapTouchPolicy: OverlapTouchPolicy.Allow
                preferredWidth: 768
                topPadding: 170
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom  

                layout: DockLayout {  
                }
                ListView {
                    id: itemList;
                    objectName: "itemList"
                                        
                    listItemComponents: [
                        ListItemComponent {
                            type: "listitem"
                            ListItem {
                                // see Common/ListItem.qml for all the contents of this list item component
                            }
                        }
                    ]
                    
                    // required function that returns the type of component that will be created
                    function itemType (data, indexPath) {
                        return 'listitem'
                    }
                    
                    onSelectionChanged: {
                        if (selected) {
                            // When an item is selected, we can do something here   
                            var chosenItem = dataModel.data (indexPath);
                            if (chosenItem.title == "+") {
                                _scrollApp.addItems(indexPath);
                                itemList.clearSelection ();
                            }
                        }
                    }
                }
            } 
        }
    }
    onTopChanged: {
        if (pane == listPage) {
            // Clear selection when returning to the list page.
            itemList.clearSelection ();
        }
    }
}
