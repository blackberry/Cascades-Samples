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

Page {
    id: root

    titleBar: TitleBar {
        title: qsTr ("Account Details")
    }

    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        //! [0]
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: 30
            topPadding: 30
            rightPadding: 30
            bottomPadding: 30

            dataModel: _accounts.accountViewer.fields

            listItemComponents: ListItemComponent {
                type: ""

                ViewerField {
                    title: ListItemData.title
                    value: ListItemData.value
                }
            }
        }
        //! [0]
    }

    //! [1]
    actions: [
        ActionItem {
            title: qsTr ("Edit")
            imageSource: "asset:///images/action_editaccount.png"

            onTriggered: {
                _accounts.editAccount()
                navigationPane.push(accountEditor.createObject())
            }
        },
        DeleteActionItem {
            onTriggered: {
                _accounts.deleteAccount()

                navigationPane.pop()
            }
        }
    ]
    //! [1]

    //! [2]
    attachedObjects: [
        ComponentDefinition {
            id: accountEditor
            source: "AccountEditor.qml"
        }
    ]
    //! [2]
}
