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
import "controls"
import wsf.cdyne.com.WeatherWS 1.0

/**
*  This is a simple Page layout that allows you to select a city from a DropDown
*  menu and click a button to get the current temperature for that city
*
*  You will learn how to:
*  -- Use both DockLayout and StackLayout
*  -- Use a DropDown menu
*  -- Execute different code for a Button onClicked based on the selected item in a DropDown menu
*  -- Use a custom control to notify when there is network activity
*  -- Use a C++ object's members
*  -- Use Javascript to control visibility of different parts of the UI
*  -- Style text using textStyle property of a Label
**/

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            imageSource: "asset:///images/background.png"
        }

        Container {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Center

            leftPadding: ui.du(3.3)
            rightPadding: ui.du(3.3)

            Label {
                text: qsTr("Select a city:")
                textStyle {
                    color: Color.Black
                    fontSizeValue: ui.du(1)
                }
            }
            //! [0]
            DropDown {
                id: cityDropDown

                enabled: !weatherService.active

                title: qsTr("City")

                Option {
                    text: qsTr("Beverly Hills")
                    description: qsTr("Beverly Hills, California")
                    value: "90210"
                    selected: true
                }

                Option {
                    text: qsTr("Detroit")
                    description: qsTr("Detroit, Michigan")
                    value: "48201"
                }

                Option {
                    text: qsTr("Pittsburgh")
                    description: qsTr("City of Pittsburgh (South Side), Pennsylvania")
                    value: "15203"
                }

                Option {
                    text: qsTr("Miami")
                    description: qsTr("Miami, Florida")
                    value: "33126"
                }

                Option {
                    text: qsTr("Mordor")
                    description: qsTr("One does not simply walk into Mordor")
                    value: "331261"
                }

                onSelectedIndexChanged: {
                    weatherService.reset()
                }
            }
            //! [0]

            //! [1]
            Button {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Get Weather")
                enabled: !weatherService.active

                onClicked: {
                    weatherService.requestWeatherInformation(cityDropDown.selectedValue);
                }
            }
            //! [1]

            //! [2]
            NetworkActivity {
                horizontalAlignment: HorizontalAlignment.Center

                active: weatherService.active
            }
            //! [2]

            //! [3]
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(11.1)
                topPadding: ui.du(2.2)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                background: Color.create("#aaffffff")

                visible: !weatherService.active && weatherService.succeeded

                Label {
                    text: weatherService.description
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        textAlign: TextAlign.Center
                    }
                }

                Label {
                    topMargin: ui.du(3.3)

                    text: weatherService.temperature
                    textStyle {
                        fontSizeValue: ui.du(2.9)
                        color: Color.Black
                        textAlign: TextAlign.Center
                    }
                }
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(11.1)

                visible: !weatherService.active && !weatherService.succeeded

                TextArea {
                    editable: false
                    backgroundVisible: false

                    text: weatherService.error
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        textAlign: TextAlign.Center
                    }
                }
            }
        }
        //! [3]

        //! [4]
        attachedObjects: [
            WeatherService {
                id: weatherService
            }
        ]
        //! [4]
    }
}
