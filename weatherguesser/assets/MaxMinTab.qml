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

// A page where the fake maximum and minimum temperatures are shown
Page {
    // Different parts of the max/min XML model are used depending on
    // the selection in the segmented control.
    property variant maxRootIndexPath: [ 0 ]
    property variant minRootIndexPath: [ 1 ]

    titleBar: TitleBar {
        id: maxMinSegment
        kind: TitleBarKind.Segmented
        appearance: TitleBarAppearance.Plain

        options: [
            Option {
                text: qsTr("Max") + Retranslate.onLanguageChanged
                value: "max"
                selected: true
            },
            Option {
                text: qsTr("Min") + Retranslate.onLanguageChanged
                value: "min"
            }
        ]

        onSelectedValueChanged: {

            // The rootindex path decides which part of the model will be
            // shown in the list below.
            if (selectedValue == "max") {
                maxminList.setRootIndexPath(maxRootIndexPath);
            } else {
                maxminList.setRootIndexPath(minRootIndexPath);
            }
        }
    }

    Container {
        background: Color.create("#F1EFE9")

        ListView {
            id: maxminList
            objectName: "maxminList"
            property variant listRootIndexPath: [ 0 ]
            scrollIndicatorMode: ScrollIndicatorMode.None

            // The rootindex path tells the list which part of the data model it
            // should use for population.
            rootIndexPath: listRootIndexPath

            dataModel: XmlDataModel {
                source: "models/max_model.xml"
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"

                    Container {
                        id: cityItem
                        leftPadding: ui.du(5) 
                        bottomPadding: leftPadding
                        topPadding: leftPadding

                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }

                        Label {
                            verticalAlignment: VerticalAlignment.Bottom
                            text: ListItemData.temp + "\u00B0"
                            textStyle.fontSize: FontSize.XLarge
                            textStyle.fontWeight: FontWeight.Normal
                            textStyle.color: {
                                // Label will have different colors depending on where it points in the model (max/min).
                                if (cityItem.ListItem.view.rootIndexPath[0] == 0) {
                                    Color.create("#d02A53")
                                } else {
                                    ui.palette.primaryBase
                                }
                            }
                        }

                        Label {
                            text: ListItemData.name
                            textStyle.fontSize: FontSize.XLarge
                            textStyle.fontWeight: FontWeight.Normal
                        }

                    }
                }
            ]
        }
    }
}
