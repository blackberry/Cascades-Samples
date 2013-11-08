/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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

Page {
    Container {
        layout: DockLayout {}

        //! [0]
        ListView {
            dataModel: _artifactline.model
            
            function date(timestamp) {
                return _artifactline.dateFromTimestamp(timestamp)
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"

                    Container {
                        id: itemRoot

                        preferredWidth: 768
                        preferredHeight: 200

                        layout: DockLayout {}

                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill

                            imageSource: itemRoot.ListItem.selected ? "asset:///images/item_background_selected.png" :
                                                                      "asset:///images/item_background.png"
                        }

                        Container {
                            horizontalAlignment: HorizontalAlignment.Left
                            leftPadding: 20
                            rightPadding: 20

                            Label {
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Center
                                
                                text: ListItemData.id + "[" + ListItemData.latestVersion + "]"
                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    color: Color.Gray
                                }
                            }

                            Label {
                                preferredHeight: 200

                                text: itemRoot.ListItem.view.date(ListItemData.timestamp)
                                textStyle {
                                    base: SystemDefaults.TextStyles.SmallText
                                    color: Color.Gray
                                }

                                multiline: true
                            }
                        }
                    }
                }
            ]

            onTriggered: {
                clearSelection()
                select(indexPath)
            }
        }
        //! [0]
    }
}
