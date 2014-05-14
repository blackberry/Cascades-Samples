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
import com.example.bb10samples.pim.messages 1.0

Page {
    id: root

    //! [0]
    onCreationCompleted: {
        if (_messages.messageComposer.mode == MessageComposer.ReplyMode) {
            subjectField.text = _messages.messageComposer.subject
            recipientField.text = _messages.messageComposer.recipient
            bodyField.text = _messages.messageComposer.body
        }
    }
    //! [0]

    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar

        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: qsTr ("Send")

            onTriggered: {
                _messages.messageComposer.composeMessage()

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
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: ui.du(3.3)
            topPadding: ui.du(3.3)
            rightPadding: ui.du(3.3)

            //! [2]
            TextField {
                id: recipientField

                hintText: qsTr ("test.person@example.org")
                inputMode: TextFieldInputMode.EmailAddress

                onTextChanging: _messages.messageComposer.recipient = text
            }
            //! [2]

            TextField {
                id: subjectField

                hintText: qsTr ("Subject")

                onTextChanging: _messages.messageComposer.subject = text
            }

            TextArea {
                id: bodyField

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                hintText: qsTr ("Type in the message here ...")

                onTextChanging: _messages.messageComposer.body = text
            }
        }
    }
}
