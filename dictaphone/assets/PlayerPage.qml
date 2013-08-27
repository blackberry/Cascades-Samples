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

import bb.cascades 1.0
import bb.multimedia 1.0

//! [0]
Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/sheet_background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: 30
            topPadding: 30
            rightPadding: 30
            bottomPadding: 30

            // The title label
            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr ("Recorded Tracks")
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    color: Color.White
                }
            }

            // The recorded tracks list view
            ListView {
                id: listView

                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 50

                dataModel: _trackManager.model

                listItemComponents: ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.name
                    }
                }

                onTriggered: {
                    clearSelection()
                    select(indexPath)
                }
            }

            // The 'Play' button
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 50

                text: qsTr ("Play")

                onClicked: {
                    // Reset URL of player
                    player.sourceUrl = ""

                    // Set the currently selected track as player source URL
                    player.sourceUrl = "file://" + listView.dataModel.data(listView.selected()).url

                    // Start playback
                    player.play()
                }
            }
        }
    }

    actions: [
        ActionItem {
            title: qsTr ("Clear All Tracks")
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/delete.png"

            onTriggered: {
                _trackManager.clearAllTracks()
                navigationPane.pop()
            }
        }
    ]

    attachedObjects: [
        MediaPlayer {
            id: player
        }
    ]
}
//! [0]
