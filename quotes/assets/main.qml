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
import "EditScreen"

NavigationPane {
    id: nav
    
    Page {
        id: quoteListPage
        content: Container {
            background: Color.Black
            layout: DockLayout {
            }
            
            ListView {
                objectName: "quotesList"
                id: quotesList
                
                stickyHeaders: true
                
                // An XML model can be used to prototype the UI until the SQL model is in place
                //dataModel: XmlDataModel { source:"models/quotes.xml" }
                 
                // There are two types of item, header for letteralphabetic groups and stanard items
                // showing the person name. 
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            titleText: {
                                // Some entries only have a last name (we enforce last name always being added)
                                if(ListItemData.firstname == undefined)
                                    ListItemData.lastname
                                else 
                                    ListItemData.firstname + " " + ListItemData.lastname;
                            }
                        }
                    },
                    ListItemComponent {
                        type: "header"
                        HeaderListItem {
                            titleText: ListItemData;
                        }
                    }
                ]
                
                function itemType (data, indexPath) {
                    
                    // Here it is decided which type of item in listItemComponents the item 
                    // should map to. If the item does not have a last name defined it is a header item.
                    if(data.lastname == undefined) {                        
                        return 'header';
                    }
                    
                    return 'item'
                }
            }
            
            // The edit screen is launched when adding or editing a quotes record.
            EditScreen {
                id: addPane
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
        }
        
        actions: [
            ActionItem {
                title: "Add"
                imageSource: "asset:///images/Add.png"
                onTriggered: {
                    addPane.newQuote ();
                }
            }
        ]
    }
    
    onTopChanged: {
        if (pane == quoteListPage) {
            quotesList.clearSelection ();
        }
    }
}
