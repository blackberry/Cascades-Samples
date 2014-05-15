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
import bb.cascades.maps 1.0

Page {
    //! [0]
    property variant session
    //! [0]
    Container {
        layout: DockLayout {
        }

        // The background image
        ImageView {
            imageSource: "asset:///images/background.png"
        }
        Container {
            topPadding: ui.du(16.7)

            //! [1]
            // Location page
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                visible: (pageSelector.selectedIndex == 0)
                Container {
                    topPadding: ui.du(5.6)
                    leftPadding: ui.du(5.6)
                    rightPadding: ui.du(5.6)
                    LabelLabel {
                        label: qsTr("timestamp")
                        text: session ? session.time : ""
                    }
                    LabelLabel {
                        label: qsTr("method")
                        text: session ? session.method : ""
                    }
                    Divider {
                    }
                    LabelLabel {
                        label: qsTr("latitude")
                        text: session ? session.latitude : ""
                    }
                    LabelLabel {
                        label: qsTr("longitude")
                        text: session ? session.longitude : ""
                    }
                    LabelLabel {
                        label: qsTr("altitude")
                        text: session ? session.altitude : ""
                    }
                    Divider {
                    }
                    LabelLabel {
                        label: qsTr("direction")
                        text: session ? session.direction : ""
                    }
                    LabelLabel {
                        label: qsTr("ground speed")
                        text: session ? session.groundSpeed : ""
                    }
                    LabelLabel {
                        label: qsTr("vertical speed")
                        text: session ? session.verticalSpeed : ""
                    }
                    Divider {
                    }
                    LabelLabel {
                        label: qsTr("magnetic variation")
                        text: session ? session.magneticVariation : ""
                    }
                    LabelLabel {
                        label: qsTr("horizontal accuracy")
                        text: session ? session.horizontalAccuracy : ""
                    }
                    LabelLabel {
                        label: qsTr("vertical accuracy")
                        text: session ? session.verticalAccuracy : ""
                    }
                    LabelLabel {
                        label: qsTr("horizontal dilution")
                        text: session ? session.horizontalDilution : ""
                    }
                    LabelLabel {
                        label: qsTr("vertical dilution")
                        text: session ? session.verticalDilution : ""
                    }
                    LabelLabel {
                        label: qsTr("position dilution")
                        text: session ? session.positionDilution : ""
                    }
                    Divider {
                    }
                    LabelLabel {
                        label: qsTr("ttff")
                        text: session ? session.ttff : ""
                    }
                    LabelLabel {
                        label: qsTr("gps week")
                        text: session ? session.gpsWeek : ""
                    }
                    LabelLabel {
                        label: qsTr("gps time of week")
                        text: session ? session.gpsTimeOfWeek : ""
                    }
                    LabelLabel {
                        label: qsTr("is propagated")
                        text: session ? session.isPropagated : ""
                    }
                    DropDown {
                        id: resetModeSelector
                        title: qsTr("Reset Mode")
                        Option {
                            text: qsTr("Cold")
                            value: "cold"
                            selected: true
                        }
                        Option {
                            text: qsTr("Warm")
                            value: "warm"
                        }
                        Option {
                            text: qsTr("Hot")
                            value: "hot"
                        }
                    }
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: qsTr("Reset")
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
                    topPadding:  ui.du(3.3)
                    leftPadding:  ui.du(3.3)
                    rightPadding:  ui.du(3.3)
                    TextArea {
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.White
                            fontFamily: "courier"
                        }
                        editable: false
                        backgroundVisible: true
                        text: session.satellitesInView
                        hintText: qsTr("Satellite Information")
                    }
                    TextArea {
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.White
                            fontFamily: "courier"
                        }
                        editable: false
                        backgroundVisible: true
                        text: session.satellitesInUse
                        hintText: qsTr("Satellite Information")
                    }
                }
            }

            //! [2]
            // Map page
            Container {
                leftPadding:  ui.du(3.3)
                topPadding: ui.du(3.3)
                rightPadding: ui.du(3.3)
                bottomPadding: ui.du(3.3)
                preferredWidth: maxWidth
                preferredHeight: maxHeight

                layout: DockLayout {
                }

                visible: (pageSelector.selectedIndex == 2)

                onVisibleChanged: {
                    if (visible)
                        session.mapView = myMap
                }

                MapView {
                    id: myMap
                    altitude: 5000
                    latitude: 0
                    longitude: 0
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }

            //! [2]
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            leftPadding: ui.du(3.3)
            topPadding: ui.du(3.3)
            rightPadding: ui.du(3.3)

            //! [3]
            SegmentedControl {
                id: pageSelector
                Option {
                    text: qsTr("Location")
                    selected: true
                }
                Option {
                    text: qsTr("Satellites")
                }
                Option {
                    text: qsTr("Map")
                }
            }
            //! [3]
        }
    }

    //! [4]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr("Back")
            onTriggered: {
                navigationPane.pop()
            }
        }
    }


    //! [4]
}
