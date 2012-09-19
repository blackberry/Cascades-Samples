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
import bb.device 1.0

// Page laying out the visual components
Page {
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            layout: DockLayout {}

            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                translationX: -5
                translationY: 10

                imageSource: "asset:///images/yellow_background.png"

                visible: (_battery.chargingState == BatteryChargingState.NotCharging ||
                          _battery.chargingState == BatteryChargingState.Full ||
                          _battery.chargingState == BatteryChargingState.Charging)

                opacity: (_battery.chargingState == BatteryChargingState.NotCharging ? 0.3 : 1.0)
            }

            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                preferredWidth: 470

                translationX: -5
                translationY: 7

                imageSource: "asset:///images/yellow_background_glow.png"

                visible: (_battery.chargingState == BatteryChargingState.Charging)

                animations: SequentialAnimation {
                    id: glowAnimation

                    repeatCount: 1000000
                    FadeTransition {
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        duration: 1000
                    }
                    FadeTransition {
                        fromOpacity: 1.0
                        toOpacity: 0.0
                        duration: 1000
                    }
                }

                function runAnimation()
                {
                    if (_battery.chargingState == BatteryChargingState.Charging)
                        glowAnimation.play();
                    else
                        glowAnimation.stop();
                }

                onCreationCompleted: _battery.levelChanged.connect(runAnimation)
            }

//! [0]
            Container {
                background: Color.Black

                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Top

                translationX: -51

                preferredWidth: (470 - (470.0 * _battery.level)/100.0) + 1 // width depends on current battery level
                preferredHeight: 500

                visible: (_battery.chargingState == BatteryChargingState.Charging)
            }
//! [0]

//! [1]
            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                translationX: -110

                imageSource: "asset:///images/blue_background.png"

                visible: (_battery.chargingState == BatteryChargingState.Discharging)
            }
//! [1]

            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                imageSource: "asset:///images/battery_overlay.png"
            }

            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                translationY: 35

                imageSource: "asset:///images/error.png"

                visible: (_battery.chargingState == BatteryChargingState.Unknown)
            }

            ImageView {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                translationY: 35

                imageSource: "asset:///images/not_charging.png"

                visible: (_battery.chargingState == BatteryChargingState.NotCharging)
            }
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            bottomPadding: 50

//! [2]
            Label {
                id: stateLabel

                horizontalAlignment: HorizontalAlignment.Center

                text: {
                    switch (_battery.chargingState) {
                        case BatteryChargingState.Unknown:
                            return qsTr ("Unknown");
                            break;
                        case BatteryChargingState.NotCharging:
                            return qsTr ("Not Charging");
                            break;
                        case BatteryChargingState.Charging:
                            return qsTr ("Charging");
                            break;
                        case BatteryChargingState.Discharging:
                            return qsTr ("Discharging");
                            break;
                        case BatteryChargingState.Full:
                            return qsTr ("Full");
                            break;
                    }
                }
                textStyle {
                    color: Color.White
                    fontSize: FontSize.XLarge
                }
            }
//! [2]

            Label {
                id: descriptionLabel

                horizontalAlignment: HorizontalAlignment.Center
                bottomMargin: 100

                text: {
                    switch (_battery.chargingState) {
                        case BatteryChargingState.Unknown:
                            return qsTr ("Something is up with the battery");
                            break;
                        case BatteryChargingState.NotCharging:
                            return qsTr ("Plugged in, just no charge");
                            break;
                        case BatteryChargingState.Charging:
                            return qsTr ("Plugged in");
                            break;
                        case BatteryChargingState.Discharging:
                            return qsTr ("Unplugged and discharging");
                            break;
                        case BatteryChargingState.Full:
                            return qsTr ("Plugged in, full");
                            break;
                    }
                }
                textStyle {
                    color: Color.Gray
                    fontSize: FontSize.Large
                }
            }
        }
    }
}
