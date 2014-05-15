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

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        //! [0]
        // The list view that shows the records of the received NDEF message
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            dataModel: _nfcReceiver.messageRecords

            listItemComponents: [
                ListItemComponent {
                    type: ""

                    Container {
                        preferredWidth: ui.du(85.3)
                        leftPadding: ui.du(1.1)
                        rightPadding: ui.du(1.1)

                        Field {
                            title: qsTr("tnf type")
                            value: ListItemData.tnfType == "0" ? qsTr("Empty Tag") :
                                   ListItemData.tnfType == "1" ? qsTr("Well Known Type") :
                                   ListItemData.tnfType == "2" ? qsTr("Media (MIME)") :
                                   ListItemData.tnfType == "3" ? qsTr("Uri") :
                                   ListItemData.tnfType == "4" ? qsTr("External") : ""
                        }

                        Field {
                            title: qsTr("record type")
                            value: ListItemData.recordType == "Sp" ? qsTr("Smart Poster") :
                                   ListItemData.recordType == "T" ? qsTr("Text") :
                                   ListItemData.recordType == "U" ? qsTr("Uri") :
                                   ListItemData.recordType == "Gc" ? qsTr("Generic Control") :
                                   ListItemData.recordType == "Hr" ? qsTr("Handover Request") :
                                   ListItemData.recordType == "Hs" ? qsTr("Handover Select") :
                                   ListItemData.recordType == "Hc" ? qsTr("Handover Carrier") :
                                   ListItemData.recordType == "Sig" ? qsTr("Signature") : ""
                        }

                        Field {
                            title: qsTr("payload")
                            value: ListItemData.payload
                        }

                        Field {
                            title: qsTr("hex")
                            value: ListItemData.hexPayload
                        }
                    }
                }
            ]
        }
        //! [0]
    }
}
