/*!
 * Copyright (c) 2012, 2013  BlackBerry Limited.
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

// A custom item for the list view that displays a summary of the push object content
Container {
    id: root

    property string type
    property string extension
    property string content
    property bool unread
    property string pushTime
    property bool selected

    // This signal is emitted whenever the user clicks on the 'Delete' icon
    signal deleteClicked()

    layout: DockLayout {}

    preferredWidth: ui.du(85.3)

    background: (selected ? Color.create("#00A8E9") : SystemDefaults.Paints.ContainerBackground)

    Container {
        topPadding: ui.du(2.1)
        bottomPadding: ui.du(2.7)

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            leftPadding: ui.du(3.3)

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            ImageView {
                id: pushTypeIcon
                imageSource: {
                    if (root.type == "image") {
                        "images/pushlist/pictures.png"
                    } else if (root.type == "text") {
                        "images/pushlist/memo.png"
                    } else {
                        "images/pushlist/browser.png"
                    }
                }
            }
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            leftPadding: ui.du(3.3)
            rightPadding: ui.du(0.1)

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            Label {
                verticalAlignment: VerticalAlignment.Center

                text: {
                    if (root.type == "image") {
                        qsTr("Image: %1").arg(root.extension)
                    } else if (root.type == "text") {
                        root.content
                    } else {
                        if (root.extension == ".html") {
                            qsTr("HTML/XML: .html")
                        } else {
                            qsTr("HTML/XML: .xml")
                        }
                    }
                }

                textStyle {
                    base: {
                        if (root.unread) {
                            textStyleUnread.style
                        } else {
                            textStyleRead.style
                        }
                    }
                }
            }
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            rightPadding: ui.du(3.9)

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            Label {
                rightMargin: ui.du(3.9)

                text: root.pushTime

                textStyle {
                    base: {
                        if (root.unread) {
                            textStyleUnread.style
                        } else {
                            textStyleRead.style
                        }
                    }
                }
            }

            ImageButton {
                id: deleteIcon
                verticalAlignment: VerticalAlignment.Center
                defaultImageSource: "asset:///images/pushlist/trash.png"
                pressedImageSource: "asset:///images/pushlist/trashhighlight.png"
                onClicked: {
                    root.deleteClicked()
                }
            }
        }
    }

    Divider {
        verticalAlignment: VerticalAlignment.Bottom
    }

    attachedObjects: [
        TextStyleDefinition {
            id: textStyleUnread
            base: SystemDefaults.TextStyles.BodyText
            fontStyle: FontStyle.Italic
        },
        TextStyleDefinition {
            id: textStyleRead
            base: textStyleUnread.style
            fontStyle: FontStyle.Normal
        }
    ]
}
