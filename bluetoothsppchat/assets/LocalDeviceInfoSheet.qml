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

// A sheet that shows the information about the local bluetooth device
Sheet {
    id: root

    Page {
        Container {
            topPadding: ui.du(2.2)
            leftPadding: ui.du(2.2)
            rightPadding: ui.du(2.2)
            bottomPadding: ui.du(2.2)

            Label {
                text: qsTr("Local Device Information")
                horizontalAlignment: HorizontalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    color: Color.DarkRed
                }
            }

            Container {
                bottomPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                //! [0]
                LabelLabel {
                    label: qsTr("Name")
                    text: _btController.localDeviceInfo.name
                }
                LabelLabel {
                    label: qsTr("Address")
                    text: _btController.localDeviceInfo.address
                }
                //! [0]
                LabelLabel {
                    label: qsTr("Class")
                    text: _btController.localDeviceInfo.deviceClass
                }
                LabelLabel {
                    label: qsTr("Discoverable")
                    text: _btController.localDeviceInfo.discoverable
                }
                LabelLabel {
                    label: qsTr("Enabled")
                    text: _btController.localDeviceInfo.enabled
                }
                LabelLabel {
                    label: qsTr("Device Type")
                    text: _btController.localDeviceInfo.deviceType
                }
            }

            Container {
                bottomPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                LabelLabel {
                    label: qsTr("API Version")
                    text: _btController.localDeviceInfo.apiVersion
                }
                LabelLabel {
                    label: qsTr("Master/Slave Allowed")
                    text: _btController.localDeviceInfo.masterSlaveAllowed
                }
                LabelLabel {
                    label: qsTr("Max Device Connections")
                    text: _btController.localDeviceInfo.maximumConnections
                }
                LabelLabel {
                    label: qsTr("Max L2CAP Rx MTU")
                    text: _btController.localDeviceInfo.maximumL2capMtu
                }
                LabelLabel {
                    label: qsTr("Max RFCOMM Rx MTU")
                    text: _btController.localDeviceInfo.maximumRfcommMtu
                }
            }

            Container {
                bottomPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                LabelLabel {
                    label: qsTr("Incoming Inquiry Scan while Connected")
                    text: _btController.localDeviceInfo.incomingInquiryScan
                }
                LabelLabel {
                    label: qsTr("Incoming Page Scan while Connected")
                    text: _btController.localDeviceInfo.incomingPageScan
                }
                LabelLabel {
                    label: qsTr("Outgoing Inquiry Scan while Connected")
                    text: _btController.localDeviceInfo.outgoingInquiryScan
                }
                LabelLabel {
                    label: qsTr("Outgoing Page Scan while Connected")
                    text: _btController.localDeviceInfo.outgoingPageScan
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
