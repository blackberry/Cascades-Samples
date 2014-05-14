/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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

Container {
    horizontalAlignment: HorizontalAlignment.Center
    verticalAlignment: VerticalAlignment.Center

    layout: DockLayout {
    }

    ImageView {
        id: compFace
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        preferredHeight: ui.du(77.8)
        preferredWidth: ui.du(77.8)

        imageSource: "asset:///images/FACE-512.png"
    }

    ImageView {
        id: needle
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        preferredHeight: compFace.preferredHeight * ui.du(0.93)
        preferredWidth: compFace.preferredWidth * ui.du(0.93)

        scalingMethod: ScalingMethod.AspectFit

        imageSource: "asset:///images/ARROW-Bright.png"
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        preferredHeight: ui.du(62.2)
        preferredWidth: ui.du(64.4)
        bottomPadding: ui.du(1.1)

        layout: DockLayout {
        }

        Label {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("North")
            textStyle {
                fontSize: FontSize.Large
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Left

            rotationZ: -90

            text: qsTr("West")
            textStyle {
                fontSize: FontSize.Large
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right

            rotationZ: 90

            text: qsTr("East")
            textStyle {
                fontSize: FontSize.Large
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center

            scaleY: -1
            scaleX: -1

            text: qsTr("South")
            textStyle {
                fontSize: FontSize.Large
            }
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        preferredHeight: ui.du(52.2)
        preferredWidth: ui.du(52.2)
        bottomPadding: ui.du(1.1)

        layout: DockLayout {
        }

        rotationZ: 45

        Label {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("NE")
            textStyle {
                fontSize: FontSize.XSmall
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Left

            rotationZ: -90

            text: qsTr("NW")
            textStyle {
                fontSize: FontSize.XSmall
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right

            rotationZ: 90

            text: qsTr("SE")
            textStyle {
                fontSize: FontSize.XSmall
            }
        }

        Label {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center

            scaleY: -1
            scaleX: -1

            text: qsTr("SW")
            textStyle {
                fontSize: FontSize.XSmall
            }
        }
    }
}
