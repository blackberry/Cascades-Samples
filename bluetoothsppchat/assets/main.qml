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

NavigationPane {
    id: navigationPane

    // auto-delete any page when popped from the NavigationPane
    onPopTransitionEnded: page.destroy()

    Page {
        //! [0]
        actions: [
            ActionItem {
                title: qsTr("Search Devices")
                imageSource: "asset:///images/device_discovery.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    _btController.deviceListing.discover();
                }
            },
            ActionItem {
                title: _btController.bluetoothActive ? qsTr("Bluetooth: ON") : qsTr("Bluetooth: OFF")
                imageSource: _btController.bluetoothActive ? "asset:///images/on.png" : "asset:///images/off.png"

                onTriggered: {
                    _btController.toggleBluetoothActive()
                }
            },
            ActionItem {
                title: _btController.discoverableActive ? qsTr("Discoverable: ON") : qsTr("Discoverable: OFF")
                imageSource: _btController.discoverableActive ? "asset:///images/discoverable_on.png" : "asset:///images/discoverable_off.png"
                onTriggered: {
                    _btController.toggleDiscoverableActive()
                }
            },
            ActionItem {
                title: qsTr("Local Device")
                imageSource: "asset:///images/local_device.png"
                onTriggered: {
                    qsLocalDeviceInfo.open();
                }
            },
            ActionItem {
                title: qsTr("SPP Server")
                imageSource: "asset:///images/send.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    _btController.chatManager.startSPPServer()
                    navigationPane.push(chatPage.createObject())
                }
            }
        ]
        //! [0]

        Container {
            //! [1]
            ListView {
                dataModel: _btController.deviceListing.model

                listItemComponents: [
                    ListItemComponent {
                        type: "listItem"
                        StandardListItem {
                            title: ListItemData.deviceName
                            description: ListItemData.deviceAddress
                            status: ListItemData.deviceClass
                        }
                    }
                ]

                onTriggered: {
                    var selectedItem = dataModel.data(indexPath);
                    _btController.setRemoteDevice(selectedItem.deviceAddress);

                    navigationPane.push(remoteDevicePage.createObject())
                }

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
            LocalDeviceInfoSheet {
                id: qsLocalDeviceInfo
            },
            ComponentDefinition {
                id: remoteDevicePage
                source: "RemoteDevice.qml"
            },
            ComponentDefinition {
                id: chatPage
                source: "SPPChat.qml"
            }
        ]
        //! [2]
    }
}
