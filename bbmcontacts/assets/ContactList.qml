/* Copyright (c) 2013 BlackBerry Limited.
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

// The main page displaying the number of contacts that have
// this application installed and a list of the contacts.
NavigationPane {
    id: navigationPane

    onPopTransitionEnded: page.destroy()

    Page {
        Container {

            layout: DockLayout {
            }
            // app background image
            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                imageSource: "asset:///images/background_blurred.png"
            }

            Container {
                // Custom component indicating number of contacts
                //! [0]
                Field {
                    id: contactCount
                    objectName: "contactCount"
                    title: qsTr("BBM Contacts")
                    value: "0"
                }
                //! [0]
                //! [1]
                // The list of contacts
                ListView {
                    objectName: "contactListView"

                    listItemComponents: [
                        ListItemComponent {
                            ContactItem {
                            }
                        }
                    ]
                    // Create Contact page upon the user selecting a contact from the list
                    onTriggered: {
                        //This gives the selected contact.
                        var page = contactPage.createObject();
                        page.contact = dataModel.data(indexPath);
                        navigationPane.push(page);
                    }
                }
                //! [1]
            }
        }
    }
    //! [2]
    attachedObjects: ComponentDefinition {
        id: contactPage
        source: "ContactPage.qml"
    }
    //! [2]
}