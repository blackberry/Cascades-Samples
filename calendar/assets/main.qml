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

                //! [0]
                // The event list filter input
                SegmentedControl {
                    Option {
                        text: qsTr ("Today")
                        value: "today"
                        selected: true
                    }

                    Option {
                        text: qsTr ("Week")
                        value: "week"
                    }

                    Option {
                        text: qsTr ("Month")
                        value: "month"
                    }

                    onSelectedIndexChanged: {
                        _calendar.filter = selectedValue
                    }
                }
                //! [0]

                //! [1]
                // The list view with all events
                ListView {
                    dataModel: _calendar.model

                    listItemComponents: ListItemComponent {
                        type: "item"

                        StandardListItem {
                            title: ListItemData.subject
                            description: qsTr ("%1 - %2").arg(ListItemData.startTime).arg(ListItemData.endTime)
                        }
                    }

                    onTriggered: {
                        clearSelection()
                        select(indexPath)

                        _calendar.setCurrentEvent(indexPath)

                        _calendar.viewEvent()
                        navigationPane.push(eventViewer.createObject())
                    }
                }
                //! [1]
            }
        }

        //! [2]
        actions: [
            ActionItem {
                title: qsTr ("New")
                imageSource: "asset:///images/action_addevent.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                onTriggered: {
                    _calendar.createEvent()
                    navigationPane.push(eventEditor.createObject())
                }
            }
        ]
        //! [2]
    }

    //! [3]
    attachedObjects: [
        ComponentDefinition {
            id: eventEditor
            source: "EventEditor.qml"
        },
        ComponentDefinition {
            id: eventViewer
            source: "EventViewer.qml"
        }
    ]
    //! [3]
}
