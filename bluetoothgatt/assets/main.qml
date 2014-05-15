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

import bb.cascades 1.3
import bb.system 1.0

NavigationPane {
    id: navigationPane

    //! [0]
    onPopTransitionEnded: {
        if (page.objectName == "ServicePage")
            _bluetoothGatt.disconnectServices()
        else if (page.objectName == "CharacteristicsPage")
            _bluetoothGatt.resetCharacteristicsList()
        else if (page.objectName == "CharacteristicsEditorPage")
            _bluetoothGatt.resetEditor()

        page.destroy()
    }
    //! [0]

    //! [1]
    attachedObjects: [
        SystemToast {
            body: _bluetoothGatt.errorMessage

            onBodyChanged: show()
        },
        SystemToast {
            body: _bluetoothGatt.editor.errorMessage

            onBodyChanged: show()
        }
    ]
    //! [1]

    Page {
        titleBar: TitleBar {
            title: qsTr("Paired GATT Devices")
        }
        //! [2]
        actions: [
            ActionItem {
                title: qsTr("Refresh")
                imageSource: "asset:///images/ic_refresh.png"
                onTriggered: {
                    _bluetoothGatt.refreshDevices();
                }
                ActionBar.placement: ActionBarPlacement.OnBar
            }
        ]
        //! [2]

        //! [3]
        attachedObjects: [
            ComponentDefinition {
                id: servicesPage
                source: "Services.qml"
            }
        ]
        //! [3]

        //! [4]
        ListView {
            onTriggered: {
                if (indexPath.length != 0) {
                    _bluetoothGatt.viewServices(indexPath[0]);
                    navigationPane.push(servicesPage.createObject())
                }
            }

            dataModel: _bluetoothGatt.devicesModel
        //! [4]

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        topPadding: ui.du(1.1)
                        leftPadding: ui.du(1.1)
                        rightPadding: ui.du(1.1)

                        Container {
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }

                            Container {
                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 1
                                }
                                verticalAlignment: VerticalAlignment.Fill
                                horizontalAlignment: HorizontalAlignment.Fill
                                ImageView {
                                    preferredHeight: ui.du(9.4)
                                    preferredWidth: ui.du(9.4)
                                    imageSource: ListItemData.paired ? "asset:///images/device_paired.png" : "asset:///images/device.png"
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Center
                                }
                            }

                            Container {
                                leftMargin: ui.du(1.1)

                                layoutProperties: StackLayoutProperties {
                                    spaceQuota: 10
                                }

                                Label {
                                    text: ListItemData.name
                                    textStyle.fontSize: FontSize.Medium
                                    bottomMargin: ui.du(0.0)
                                }

                                Label {
                                    text: ListItemData.address
                                    textStyle.fontSize: FontSize.Small
                                    topMargin: ui.du(0.0)
                                }
                            }
                        }
                        Divider {
                            topMargin: ui.du(0.1)
                        }
                    }
                }
            ]
        }
    }
}
