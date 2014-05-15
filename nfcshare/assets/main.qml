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
import custom 1.0

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            SegmentedControl {
                Option {
                    text: qsTr("Data Share")
                    value: NfcShareHandler.DataShare
                }

                Option {
                    text: qsTr("File Share")
                    value: NfcShareHandler.FileShare
                }

                onSelectedValueChanged: {
                    _nfcShareHandler.shareMode = selectedValue
                }
            }

            // The page for data sharing
            Container {
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                visible: (_nfcShareHandler.shareMode == NfcShareHandler.DataShare)

                TextField {
                    hintText: qsTr("Content")

                    onTextChanging: {
                        // We will update the data content everytime this field changes.
                        // We do this because the use may tap their devices at any given time.
                        _nfcShareHandler.data = text
                    }
                }

                // Guide message to display to the user
                Label {
                    text: qsTr("Type some text into the text field and then tap an NFC tag or device to share content")

                    multiline: true
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.White
                        textAlign: TextAlign.Center
                    }
                }
            }

            // The page for file sharing
            Container {
                topPadding: ui.du(2.2)
                leftPadding: ui.du(1.1)
                rightPadding: ui.du(1.1)
                bottomPadding: ui.du(1.1)

                visible: (_nfcShareHandler.shareMode == NfcShareHandler.FileShare)

                ListView {
                    horizontalAlignment: HorizontalAlignment.Fill

                    dataModel: _nfcShareHandler.fileModel

                    listItemComponents: [
                        ListItemComponent {
                            type: ""

                            Container {
                                id: root

                                leftPadding: ui.du(2.2)
                                preferredHeight: ui.du(16.7)

                                Label {
                                    horizontalAlignment: HorizontalAlignment.Fill

                                    text: ListItemData
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.White
                                    }

                                    contextActions: [
                                        ActionSet {
                                            DeleteActionItem {
                                                onTriggered: {
                                                    // HACK: Use the indirection via the ListView until Cascades allows to
                                                    //       access context properties from within ListItemComponents
                                                    root.ListItem.view.removeFile(ListItemData)
                                                }
                                            }
                                        }
                                    ]
                                }

                                Divider {}
                            }
                        }
                    ]

                    function removeFile(filePath)
                    {
                        _nfcShareHandler.fileModel.removeFile(filePath)
                    }
                }

                Button {
                    horizontalAlignment: HorizontalAlignment.Right
                    topMargin: ui.du(3.3)

                    text: qsTr("Add File...")

                    onClicked: {
                        filePicker.open()
                    }
                }

                attachedObjects: [
                    FilePicker {
                        id: filePicker

                        title: qsTr("Select File")

                        // We will allow the user to pick a file from available shared files.
                        // This should only operate in the personal perimeter.
                        directories: ["/accounts/1000/shared/"]

                        onFileSelected: {
                            _nfcShareHandler.fileModel.addFile(selectedFiles[0])
                        }
                    }
                ]
            }
        }
    }
}
