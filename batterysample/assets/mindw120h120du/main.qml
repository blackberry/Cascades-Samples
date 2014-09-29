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

// Page laying out the visual components
Page {
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///background.png"
        }

        Battery {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            bottomPadding: ui.du(5.5)

            //! [0]
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
            //! [0]

            Label {
                id: descriptionLabel

                horizontalAlignment: HorizontalAlignment.Center
                bottomMargin: ui.du(11.1)

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
