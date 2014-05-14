/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
import bb.device 1.0

// Page which organizes the various visual components
Page {
    Container {
        leftPadding: ui.du(2.2)
        topPadding: ui.du(2.2)
        rightPadding: ui.du(2.2)

        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            // A standard Label used for the application title
            Label {
                horizontalAlignment: HorizontalAlignment.Center

                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    color: Color.White
                }

                preferredHeight: ui.du(16.7)
                text: qsTr ("LED Tester")
            }

            //! [0]
            DropDown {
                id: colorChooser

                topMargin: ui.du(11.1)
                horizontalAlignment: HorizontalAlignment.Center

                title: qsTr ("LED Color")

                Option {
                    text: qsTr ("Red")
                    value: LedColor.Red
                    imageSource: "asset:///images/red.png"
                    selected: true
                }

                Option {
                    text: qsTr ("Green")
                    value: LedColor.Green
                    imageSource: "asset:///images/green.png"
                }

                Option {
                    text: qsTr ("Blue")
                    value: LedColor.Blue
                    imageSource: "asset:///images/blue.png"
                }

                Option {
                    text: qsTr ("Yellow")
                    value: LedColor.Yellow
                    imageSource: "asset:///images/yellow.png"
                }

                Option {
                    text: qsTr ("Cyan")
                    value: LedColor.Cyan
                    imageSource: "asset:///images/cyan.png"
                }

                Option {
                    text: qsTr ("Magenta")
                    value: LedColor.Magenta
                    imageSource: "asset:///images/magenta.png"
                }

                Option {
                    text: qsTr ("White")
                    value: LedColor.White
                    imageSource: "asset:///images/white.png"
                }
            }
            //! [0]

            // A standard Label displaying the slider value
            Label {
                topMargin: ui.du(11.1)
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr ("Flash Count: %1").arg(slider.normalizedValue == -1 ? "∞" : slider.normalizedValue)
                textStyle {
                    fontSize: FontSize.Medium
                    color: Color.White
                }
            }

            //! [1]
            // Slider which allows you to choose flash counts
            Slider {
                id: slider

                property int normalizedValue: (Math.floor (value) == toValue ? -1 : Math.floor (value))

                horizontalAlignment: HorizontalAlignment.Fill

                fromValue: 1
                toValue: 10
                value: 1

                enabled: !ledID.active
            }
            //! [1]

            //! [2]
            // A standard Button to activate LED flash
            Button {
                topMargin: ui.du(11.1)
                horizontalAlignment: HorizontalAlignment.Center

                text: ledID.active ? qsTr ("Cancel") : qsTr ("Flash")

                onClicked: {
                    if (ledID.active) {
                        ledID.cancel ();
                    } else {
                        if (slider.normalizedValue == -1) {
                            ledID.flash ()
                        } else {
                            ledID.flash (slider.normalizedValue)
                        }
                    }
                }
            }
            //! [2]

            // A standard Label
            Label {
                topMargin: ui.du(11.1)
                horizontalAlignment: HorizontalAlignment.Center

                visible: ledID.active

                text: qsTr ("Remaining flash count: %1").arg(slider.normalizedValue == -1 ? "∞" : ledID.remainingFlashCount)
                textStyle {
                    fontSize: FontSize.Medium
                    color: Color.White
                }
            }
        }
    }

    attachedObjects: [
        //! [3]
        // Led component
        Led {
            id: ledID

            color: colorChooser.selectedValue
        }
        //! [3]
    ]
}

