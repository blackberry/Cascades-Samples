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
            BackgroundImage {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                leftPadding: ui.du(5)
                topPadding: ui.du(5)
                rightPadding: ui.du(5)
                bottomPadding: ui.du(5)

                //! [0]
                // The accounts list filter
                DropDown {
                    title: qsTr ("Service")

                    Option {
                        text: qsTr ("Calendars")
                        value: "Calendars"
                        selected: true
                    }

                    Option {
                        text: qsTr ("Contacts")
                        value: "Contacts"
                    }

                    Option {
                        text: qsTr ("Notebook")
                        value: "Notebook"
                    }

                    Option {
                        text: qsTr ("Geolocations")
                        value: "Geolocations"
                    }

                    Option {
                        text: qsTr ("Linking")
                        value: "Linking"
                    }

                    Option {
                        text: qsTr ("Memos")
                        value: "Memos"
                    }

                    Option {
                        text: qsTr ("Messages")
                        value: "Messages"
                    }

                    Option {
                        text: qsTr ("Tags")
                        value: "Tags"
                    }

                    Option {
                        text: qsTr ("Tasks")
                        value: "Tasks"
                    }

                    Option {
                        text: qsTr ("Phone")
                        value: "Phone"
                    }

                    onSelectedValueChanged: _accounts.filter = selectedValue
                }
                //! [0]

                //! [1]
                // The list view with all contacts
                ListView {
                    dataModel: _accounts.model

                    listItemComponents: ListItemComponent {
                        type: "item"

                        StandardListItem {
                            title: ListItemData.provider
                            description: ListItemData.displayName
                        }
                    }

                    onTriggered: {
                        clearSelection()
                        select(indexPath)

                        _accounts.setCurrentAccount(indexPath)

                        _accounts.viewAccount();
                        navigationPane.push(accountViewer.createObject())
                    }
                }
                //! [1]
            }
        }

        //! [2]
        actions: [
            ActionItem {
                title: qsTr ("New")
                imageSource: "asset:///images/action_addaccount.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                onTriggered: {
                    _accounts.createAccount()
                    navigationPane.push(accountEditor.createObject())
                }
            }
        ]
        //! [2]
    }

    //! [3]
    attachedObjects: [
        ComponentDefinition {
            id: accountEditor
            source: "AccountEditor.qml"
        },
        ComponentDefinition {
            id: accountViewer
            source: "AccountViewer.qml"
        }
    ]
    //! [3]
}
