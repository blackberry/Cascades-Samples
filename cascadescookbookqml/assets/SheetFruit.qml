/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
            topPadding: ui.du(2)
            leftPadding: ui.du(2)
            rightPadding: ui.du(2)

            // A grid list with some fruits that can be put in the fruit basket.
            ListView {
                dataModel: XmlDataModel {
                    source: "models/sheetmodel.xml"
                }

                layout: GridListLayout {
                    columnCount: 2
                    cellAspectRatio: 1.6
                }

                layoutProperties: StackLayoutProperties {
                    // Spacequota needs to be set so the list doesn't take precedence
                    // when the Page is laid out (if not set, it will fill the entire screen).
                    spaceQuota: 1
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            id: itemRoot
                            background: Color.create("#f0f0f0")
                            layout: DockLayout {
                            }

                            // Item highlight
                            Container {
                                background: Color.create("#75b5d3")
                                opacity: itemRoot.ListItem.active  ? 0.9 : 0.0
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                            }

                            // Item image, delivered via ListItemData from the models/sheet.xml model
                            ImageView {
                                imageSource: ListItemData.fruit
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                            }
                        }
                    }
                ]
                onTriggered: {
                    // A fruit was selected so send the save signal with the imageSource for the fruit.
                    var chosenItem = dataModel.data (indexPath);
                    sheetFruit.save(chosenItem.fruit);
                }
            }
        }
    }
}
