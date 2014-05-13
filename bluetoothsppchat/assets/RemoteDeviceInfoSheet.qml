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

// A sheet that shows the information about a remote bluetooth device
Sheet {
    id: root

    Page {
        Container {
            Label {
                text: _btController.remoteDeviceInfo.name
                horizontalAlignment: HorizontalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    color: Color.DarkRed
                }
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Left
                bottomPadding: ui.du(0.6)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                //! [0]
                LabelLabel {
                    label: qsTr("Address")
                    text: _btController.remoteDeviceInfo.address
                }
                LabelLabel {
                    label: qsTr("Class")
                    text: _btController.remoteDeviceInfo.deviceClass
                }
                //! [0]
                LabelLabel {
                    label: qsTr("Device Type")
                    text: _btController.remoteDeviceInfo.deviceType
                }
                LabelLabel {
                    label: qsTr("Encrypted")
                    text: _btController.remoteDeviceInfo.encrypted
                }
                LabelLabel {
                    label: qsTr("Paired")
                    text: _btController.remoteDeviceInfo.paired
                }
                LabelLabel {
                    label: qsTr("Trusted")
                    text: _btController.remoteDeviceInfo.trusted
                }
                LabelLabel {
                    label: qsTr("RSSI")
                    text: _btController.remoteDeviceInfo.rssi
                }
            }

            Container {
                leftPadding: ui.du(2.2)
                bottomPadding: ui.du(0)
                bottomMargin: ui.du(0)
                rightPadding: ui.du(2.2)

                Label {
                    bottomMargin: ui.du(0)
                    text: qsTr("Low Energy Properties:")
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        fontWeight: FontWeight.Bold
                        color: Color.Blue
                    }
                }

                LabelLabel {
                    label: qsTr("MIN Connection Interval")
                    text: _btController.remoteDeviceInfo.minimumConnectionInterval
                }
                LabelLabel {
                    label: qsTr("MAX Connection Interval")
                    text: _btController.remoteDeviceInfo.maximumConnectionInterval
                }
                LabelLabel {
                    label: qsTr("Latency")
                    text: _btController.remoteDeviceInfo.latency
                }
                LabelLabel {
                    label: qsTr("Supervisory Timeout")
                    text: _btController.remoteDeviceInfo.supervisoryTimeout
                }
                LabelLabel {
                    label: qsTr("Appearance")
                    text: _btController.remoteDeviceInfo.appearance
                }
                LabelLabel {
                    label: qsTr("Flags")
                    text: _btController.remoteDeviceInfo.flags
                }
                LabelLabel {
                    label: qsTr("Connectable")
                    text: _btController.remoteDeviceInfo.connectable
                }
            }

            Divider {
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Center
                text: qsTr("Close")
                onClicked: root.close()
            }
        }
    }
}
