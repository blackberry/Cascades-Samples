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
    id: root

    titleBar: TitleBar {
        title: qsTr ("Contact Details")
    }

    Container {
        layout: DockLayout {}

        // The background image
        BackgroundImage {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }

        AddressbookScrollView {
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                leftPadding: ui.du(15)
                topPadding: ui.du(50)
                rightPadding: ui.du(15)

                //! [0]
                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    title: qsTr("first name")
                    value: _addressBook.contactViewer.firstName
                }

                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)

                    title: qsTr("last name")
                    value: _addressBook.contactViewer.lastName
                }
                //! [0]

                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)

                    title: qsTr("birthday")
                    value: _addressBook.contactViewer.formattedBirthday
                }

                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)

                    title: qsTr("email")
                    value: _addressBook.contactViewer.email
                }
            }
        }
    }

    //! [1]
    actions: [
        ActionItem {
            title: qsTr ("Edit")
            imageSource: "asset:///images/action_editcontact.png"

            onTriggered: {
                _addressBook.editContact()
                navigationPane.push(contactEditor.createObject())
            }
        },
        DeleteActionItem {
            onTriggered: {
                _addressBook.deleteContact()

                navigationPane.pop()
            }
        }
    ]
    //! [1]

    //! [2]
    attachedObjects: [
        ComponentDefinition {
            id: contactEditor
            source: "ContactEditor.qml"
        }
    ]
    //! [2]
}
