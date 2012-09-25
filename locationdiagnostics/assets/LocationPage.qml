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

Page {
//! [0]
    property variant session
//! [0]

    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            imageSource: "asset:///images/background.png"
        }

        Container {
            topPadding: 100

            layout: DockLayout {}

//! [1]
            // Location page
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }

                visible: (pageSelector.selectedIndex == 0)

                Container {
                    topPadding: 50
                    leftPadding: 50
                    rightPadding: 50

                    LabelLabel {
                        label: qsTr ("timestamp")
                        text: session ? session.time : ""
                    }

                    Divider {}

                    LabelLabel {
                        label: qsTr ("latitude")
                        text: session ? session.latitude : ""
                    }

                    LabelLabel {
                        label: qsTr ("longitude")
                        text: session ? session.longitude : ""
                    }

                    LabelLabel {
                        label: qsTr ("altitude")
                        text: session ? session.altitude : ""
                    }

                    Divider {}

                    LabelLabel {
                        label: qsTr ("direction")
                        text: session ? session.direction : ""
                    }

                    LabelLabel {
                        label: qsTr ("ground speed")
                        text: session ? session.groundSpeed : ""
                    }

                    LabelLabel {
                        label: qsTr ("vertical speed")
                        text: session ? session.verticalSpeed : ""
                    }

                    Divider {}

                    LabelLabel {
                        label: qsTr ("magnetic variation")
                        text: session ? session.magneticVariation : ""
                    }

                    LabelLabel {
                        label: qsTr ("horizontal accuracy")
                        text: session ? session.horizontalAccuracy : ""
                    }

                    LabelLabel {
                        label: qsTr ("vertical accuracy")
                        text: session ? session.verticalAccuracy : ""
                    }

                    Divider {}

                    DropDown {
                        id: resetModeSelector

                        title: qsTr ("Reset Mode")

                        Option {
                            text: qsTr ("Cold")
                            value: "cold"
                            selected: true
                        }

                        Option {
                            text: qsTr ("Warm")
                            value: "warm"
                        }

                        Option {
                            text: qsTr ("Hot")
                            value: "hot"
                        }
                    }

                    Button {
                        horizontalAlignment: HorizontalAlignment.Center

                        text: qsTr ("Reset")

                        onClicked: session.resetSession(resetModeSelector.selectedValue)
                    }
                }
            }
//! [1]

            // Satellites page
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }

                visible: (pageSelector.selectedIndex == 1)

                Container {
                    topPadding: 30
                    leftPadding: 30
                    rightPadding: 30

                    TextArea {
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            fontWeight: FontWeight.Normal
                            fontFamily: "courier"
                        }

                        editable: false

                        backgroundVisible: true

                        hintText: qsTr ("Satellite Information")
                    }
                }
            }

//! [2]
            // Logs page
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }

                visible: (pageSelector.selectedIndex == 2)

                Container {
                    topPadding: 30
                    leftPadding: 30
                    rightPadding: 30

                    TextArea {
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            fontWeight: FontWeight.Normal
                            fontFamily: "courier"
                        }

                        editable: false

                        backgroundVisible: true

                        hintText: qsTr ("Logs")
                        text: session ? session.log : ""
                    }
                }
            }
        }
//! [2]

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            leftPadding: 30
            topPadding: 30
            rightPadding: 30

//! [3]
            SegmentedControl {
                id: pageSelector

                Option {
                    text: qsTr ("Location")
                    selected: true
                }

                Option {
                    text: qsTr ("Satellites")
                }

                Option {
                    text: qsTr ("Logs")
                }
            }
//! [3]
        }
    }

//! [4]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr ("Back")
            onTriggered: {
                session.stopUpdates()
                navigationPane.pop()
            }
        }
    }
//! [4]
}
