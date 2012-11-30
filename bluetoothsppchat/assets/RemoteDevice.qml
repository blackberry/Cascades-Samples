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

// A page that lists all services of a remote bluetooth device
Page {
    //! [0]
    actions: [
        ActionItem {
            title: qsTr("Device Info")
            imageSource: "asset:///images/local_device.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                qsDeviceInfo.open();
            }
        },
        ActionItem {
            title: qsTr("Connect SPP")
            imageSource: "asset:///images/broadcasts.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _btController.chatManager.connectToSPPService();
                navigationPane.push(chatPage.createObject())
            }
        }
    ]
    //! [0]

    Container {
        Container {
            //! [1]
            ListView {
                dataModel: _btController.remoteDeviceInfo.model

                listItemComponents: [
                    ListItemComponent {
                        type: "listItem"
                        StandardListItem {
                            title: ListItemData.uuid
                            description: ListItemData.address
                        }
                    }
                ]

                function itemType(data, indexPath) {
                    if (indexPath.length == 1) {
                        // If the index path contains a single integer, the item
                        // is a "header" type item
                        return "header";
                    } else {
                        // If the index path contains more than one integer, the
                        // item is a "listItem" type item
                        return "listItem";
                    }
                }
            }
            //! [1]
        }

        //! [2]
        attachedObjects: [
            RemoteDeviceInfoSheet {
                id: qsDeviceInfo
            },
            ComponentDefinition {
                id: chatPage
                source: "SPPChat.qml"
            }
        ]
        //! [2]
    }
}
