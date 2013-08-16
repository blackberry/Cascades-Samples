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

import bb.cascades 1.2

NavigationPane {
    id: navigationPane

    onPopTransitionEnded: page.destroy()

    Page {
        //! [0]
        attachedObjects: [
            ComponentDefinition {
                id: createBoxPage
                source: "NewProfileBoxPage.qml"
            }
        ]
        //! [0]

        //! [1]
        actions: [
            ActionItem {
                title: qsTr("New Profile Box")

                onTriggered: {
                    navigationPane.push(createBoxPage.createObject())
                }
            },
            DeleteActionItem {
                onTriggered: {
                    _profileBoxManager.removeProfileBoxItem(listView.selectedItemId)
                }
            }
        ]
        //! [1]

        Container {
            layout: DockLayout {}
            background: Color.create("#272727")

            //! [2]
            ListView {
                id: listView

                property string selectedItemId

                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                dataModel: _profileBoxManager.model

                listItemComponents: [
                    ListItemComponent {
                        ProfileBoxItem {
                        }
                    }
                ]

                onTriggered: {
                    clearSelection()
                    select(indexPath)

                    selectedItemId = dataModel.data(indexPath).id
                }
            }
            //! [2]
        }
    }
}
