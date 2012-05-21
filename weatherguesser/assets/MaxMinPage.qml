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

// A page where the maximum and minimum temperatures are shown (or are they?).
Page {
    content: Container {
        background: Color.create ("#272727")
        layout: StackLayout {
            topPadding: 50
        }

        // To filter the max/min list a segmented control is used.
        SegmentedControl {
            id: maxMinSegment

            // Different parts of the max/min xml model is used depending on
            // the selection in the segmented control.
            property variant maxRootIndexPath: [0]
            property variant minRootIndexPath: [1]

            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }

            Option {
                id: max
                text: "Max"
                value: "max"
                selected: true
            }
            Option {
                id: min
                text: "Min"
                value: "min"
            }

            onSelectedIndexChanged: {

                // The rootindex path decides which part of the model that
                // will be shown in the list below.
                if (maxMinSegment.selectedValue () == "max") {
                    maxminList.setRootIndexPath (maxRootIndexPath);
                } else {
                    maxminList.setRootIndexPath (minRootIndexPath);
                }
            }
        }

        ListView {
            id: maxminList
            objectName: "maxminList"
            property variant listRootIndexPath: [0]
            scrollbarEnabled: false

            // The root index path tells the list which part of the data model it
            // should use for population.
            rootIndexPath: listRootIndexPath

            dataModel: XmlDataModel {
                id: cityModel
                source: "models/max_model.xml"
            }

            listItemComponents: [

                ListItemComponent {
                    type: "item"
                    Container {
                        id: cityItem

                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                            leftPadding: 50
                            bottomPadding: leftPadding
                            topPadding: leftPadding
                        }

                        Label {
                            text: ListItemData.temp + "\u00B0"

                            textStyle {
                                base: SystemDefaults.TextStyles.BigText
                                color: {
                                    // The test will have different colors depending on where it points in the model.
                                    if (cityItem.ListItem.view.rootIndexPath[0] == 0) {
                                        Color.create ("#d8225E")
                                    } else {
                                        Color.create ("#006dba")
                                    }
                                }
                                fontWeight: FontWeight.Light
                            }

                            layoutProperties: StackLayoutProperties {
                                verticalAlignment: VerticalAlignment.Bottom
                            }
                        }

                        Label {
                            text: ListItemData.name
                            textStyle {
                                base: SystemDefaults.TextStyles.BigText
                                color: Color.White
                                fontWeight: FontWeight.Normal
                            }
                        }
                    }
                }
            ]
        }
    }
}
