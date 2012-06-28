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
    
    // A variant is used to keep a copy of the currently selected item, the data is used
    // by the ContentPage (see ContenPage.qml) to present a more detailed view of the item.
    property variant _contentView;
    
    Page {
        id: stampListPage
        Container {
            
            // A stylish paper image is used to tile the background.
            background: backgroundPaint.imagePaint   
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/Tile_scribble_light_256x256"
                    repeatPattern: RepeatPattern.XY
                }
            ]
            
            ListView {
                id: stampList
                objectName: "stampList"
                
                layout: GridListLayout {
                    columnCount: 2
                    headerMode:  ListHeaderMode.Standard
                    topPadding: 0
                    cellAspectRatio: 1.4
                    spacingAfterHeader: 40
                    verticalCellSpacing: 0
                }

                // This data model will be replaced by a JSON model when the application starts,
                // an XML model can be used to prototype the UI and for smaller static lists.
                dataModel: XmlDataModel {
                    source: "models/stamps.xml"
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "header"
                        
                        HeaderListItem {
                            title: {
                                if (ListItemData.title) {
                                    // If the data is loaded from XML a property is used.
                                    ListItemData.title
                                } else {
                                    // If it is loaded from JSON and set in a GroupDataModel the header info in ListItemData
                                    ListItemData
                                }
                            }
                        }
                    },
                    ListItemComponent {
                        type: "item"
                        StampItem {
                        }
                    }
                ]
                
                onTriggered: {
                    // When an item is selected we push the recipe Page in the chosenItem file attribute.
                    var chosenItem = dataModel.data(indexPath);

                    // The _contentView property can be resolved in by the ContentPage since it will
                    // share the same context as the main file.
                    _contentView = chosenItem;                    
                    
                    // Push the Content Page on top to drill down and show details about the stamp.
                    nav.deprecatedPushQmlByString("ContentPage.qml");                    
                } 
            }
        }
    }
}
