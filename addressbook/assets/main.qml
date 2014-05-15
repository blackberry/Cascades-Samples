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

                leftPadding: ui.du(3.3)
                topPadding: ui.du(3.3)
                rightPadding: ui.du(3.3)
                bottomPadding: ui.du(3.3)

                //! [0]
                // The contact list filter input
                TextField {
                    hintText: qsTr ("Filter by...")

                    onTextChanging: _addressBook.filter = text
                }
                //! [0]

                //! [1]
                // The list view with all contacts
                ListView {
                    dataModel: _addressBook.model

                    listItemComponents: ListItemComponent {
                        type: "item"

                        StandardListItem {
                            title: qsTr ("%1, %2").arg(ListItemData.lastName).arg(ListItemData.firstName)
                            description: ListItemData.email
                        }
                    }

                    onTriggered: {
                        clearSelection()
                        select(indexPath)

                        _addressBook.setCurrentContact(indexPath)

                        _addressBook.viewContact();
                        navigationPane.push(contactViewer.createObject())
                    }
                }
                //! [1]
            }
        }

        //! [2]
        actions: [
            ActionItem {
                title: qsTr ("New")
                imageSource: "asset:///images/action_addcontact.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                onTriggered: {
                    _addressBook.createContact()
                    navigationPane.push(contactEditor.createObject())
                }
            }
        ]
        //! [2]
    }

    //! [3]
    attachedObjects: [
        ComponentDefinition {
            id: contactEditor
            source: "ContactEditor.qml"
        },
        ComponentDefinition {
            id: contactViewer
            source: "ContactViewer.qml"
        }
    ]
    //! [3]
}
