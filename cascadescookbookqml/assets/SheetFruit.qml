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

// A sheet containing a list of fruits to select from. This is the
// second Sheet in the drill-down navigation from the Sheet recipe.
Page {
    id: sheetFruit
    signal cancel();
    signal save(string newFruit);

    titleBar: TitleBar {
        id: addBar
        title: "Select"
        visibility: ChromeVisibility.Visible
        
        dismissAction: ActionItem {
            title: "Cancel"
            onTriggered: {
                sheetFruit.cancel();
            }
        }
    }
    
    RecipeContainer {
        Container {
            layout: StackLayout {
                topPadding: 25
                leftPadding: 25
                rightPadding: 25
            }            

            // A grid list with some fruits that can be put in the fruit basket.
            ListView {
                topMargin: 50
                dataModel: XmlDataModel {
                    source: "models/sheetmodel.xml"
                }

                layout: GridListLayout {
                    columnCount: 2
                    cellAspectRatio: 1.6
                }

                layoutProperties: StackLayoutProperties {
                    // Spacequota needs to be set in order for the list not to take precedence
                    // in the layouting of the page (if not set it will fill the entire screen).
                    spaceQuota: 1
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: itemRoot

                            layout: DockLayout {
                            }

                            // Item background.
                            ImageView {
                                imageSource: "asset:///images/title_gui_buffet_empty_box"
                                layoutProperties: DockLayoutProperties {
                                    verticalAlignment: VerticalAlignment.Fill
                                    horizontalAlignment: HorizontalAlignment.Fill
                                }
                            }

                            // Item highlight.
                            Container {
                                background: Color.create("#75b5d3")
                                opacity: itemRoot.ListItem.active  ? 0.9 : 0.0
                                layoutProperties: DockLayoutProperties {
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Fill
                                }
                            }

                            // Item image, delivered via ListItemData from the models/sheet.xml model.
                            ImageView {
                                imageSource: ListItemData.fruit
                                layoutProperties: DockLayoutProperties {
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                }
                            }
                        }
                    }
                ]
                onTriggered: {
                    // A fruit was selected send the save signal with the imageSource for the
                    // fruit.
                    var chosenItem = dataModel.data (indexPath);
                    sheetFruit.save(chosenItem.fruit);
                }
            }
        }
    }
}
