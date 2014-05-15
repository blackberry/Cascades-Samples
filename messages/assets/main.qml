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

NavigationPane {
    id: navigationPane

    onPopTransitionEnded: page.destroy()

    // The main page
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

                leftPadding: ui.du(3.3)
                topPadding: ui.du(3.3)
                rightPadding: ui.du(3.3)
                bottomPadding: ui.du(3.3)
                DropDown {
                    id: accounts
                    title: "Account"
                    onSelectedOptionChanged: _messages.setSelectedAccount(selectedOption)
                }
                //! [0]
                // The message list filter input
                TextField {
                    hintText: qsTr ("Filter by...")

                    onTextChanging: _messages.filter = text
                }
                //! [0]

                //! [1]
                // The list view with all messages
                ListView {
                    dataModel: _messages.model

                    listItemComponents: ListItemComponent {
                        type: "item"

                        StandardListItem {
                            title: ListItemData.subject
                            description: ListItemData.time
                        }
                    }

                    onTriggered: {
                        clearSelection()
                        select(indexPath)

                        _messages.setCurrentMessage(indexPath)

                        _messages.viewMessage();
                        navigationPane.push(messageViewer.createObject())
                    }
                }
                //! [1]
            }
        }

        //! [2]
        actions: [
            ActionItem {
                title: qsTr ("New")
                imageSource: "asset:///images/action_composemessage.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                onTriggered: {
                    _messages.composeMessage()
                    navigationPane.push(messageComposer.createObject())
                }
            }
        ]
        //! [2]
    }

    //! [3]
    attachedObjects: [
        ComponentDefinition {
            id: messageComposer
            source: "MessageComposer.qml"
        },
        ComponentDefinition {
            id: messageViewer
            source: "MessageViewer.qml"
        },
        RenderFence {
            raised: true
            onReached: {
                _messages.addAccounts(accounts)
            }
        }
    ]
    //! [3]
}
