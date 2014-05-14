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
import com.example.bb10samples.pim.addressbook 1.0

Page {
    id: root

    //! [0]
    onCreationCompleted: {
        if (_addressBook.contactEditor.mode == ContactEditor.EditMode) {
            // Fill the editor fields after the UI has been created
            firstNameField.text = _addressBook.contactEditor.firstName
            lastNameField.text = _addressBook.contactEditor.lastName
            birthdayField.value = _addressBook.contactEditor.birthday
            emailField.text = _addressBook.contactEditor.email
        }
    }
    //! [0]

    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar

        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: (_addressBook.contactEditor.mode == ContactEditor.CreateMode ? qsTr ("Create" ) : qsTr ("Save"))

            onTriggered: {
                _addressBook.contactEditor.saveContact()

                navigationPane.pop()
            }
        }

        // The 'Cancel' action
        dismissAction: ActionItem {
            title: qsTr ("Cancel")

            onTriggered: navigationPane.pop()
        }
    }
    //! [1]

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
                verticalAlignment: VerticalAlignment.Fill

                leftPadding: ui.du(3.3)
                topPadding: ui.du(3.3)
                rightPadding: ui.du(3.3)

                //! [2]
                TextField {
                    id: firstNameField

                    hintText: qsTr("First Name")

                    onTextChanging: _addressBook.contactEditor.firstName = text
                }
                //! [2]

                TextField {
                    id: lastNameField

                    hintText: qsTr("Last Name")

                    onTextChanging: _addressBook.contactEditor.lastName = text
                }

                DateTimePicker {
                    id: birthdayField

                    title: qsTr("Birthday")

                    onValueChanged: _addressBook.contactEditor.birthday = value
                }

                TextField {
                    id: emailField

                    hintText: qsTr("Email")

                    inputMode: TextFieldInputMode.EmailAddress

                    onTextChanging: _addressBook.contactEditor.email = text
                }
            }
        }
    }
}
