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
import bb.cascades.pickers 1.0

Page {
    onCreationCompleted: {
        personalMessage.text = _profile.personalMessage
        statusMessage.text = _profile.statusMessage
        isBusy.checked = _profile.busy
    }

    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background_blurred.png"
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            topPadding: ui.du(2.2)
            leftPadding: ui.du(2.2)
            rightPadding: ui.du(2.2)

            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Update Profile")
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    color: Color.White
                    fontWeight: FontWeight.Bold
                }
            }

            Divider {
            }

            //! [0]
            TextField {
                id: personalMessage

                hintText: qsTr("Type personal message here")
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Right

                text: qsTr("Save")

                onClicked: {
                    _profile.editor.savePersonalMessage(personalMessage.text)
                    navigationPane.pop()
                }
            }
            //! [0]

            //! [1]
            TextField {
                id: statusMessage
                topMargin: ui.du(4.4)

                hintText: qsTr("Type status message here")
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                layout: DockLayout {
                    //orientation: LayoutOrientation.LeftToRight
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Left
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: qsTr("Mark as busy")
                        textStyle {
                            color: Color.White
                        }
                    }

                    CheckBox {
                        id: isBusy
                    }
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Right

                    text: qsTr("Save")

                    onClicked: {
                        _profile.editor.saveStatus(isBusy.checked, statusMessage.text)
                        navigationPane.pop()
                    }
                }
            }

            //! [1]

            //! [2]
            Button {
                topMargin: ui.du(2.2)
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Set Avatar")

                onClicked: {
                    filePicker.open()
                }
            }
            //! [2]
        }
    }

    //! [3]
    attachedObjects: [
        FilePicker {
            id: filePicker

            title: qsTr("Select Image")

            // We will allow the user to pick a file from available shared files.
            // This should only operate in the personal perimeter.
            directories: ["/accounts/1000/shared/"]

            onFileSelected: {
                _profile.editor.saveDisplayPicture(selectedFiles[0])
                navigationPane.pop()
            }
        }
    ]
    //! [3]
}
