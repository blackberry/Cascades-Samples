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

Page {
    objectName: "CharacteristicsEditorPage"
    
    titleBar: TitleBar {
        title: qsTr("Characteristics Editor")
    }
    
    ScrollArea {
        Container {
            topPadding: ui.du(2.2)
            leftPadding: ui.du(2.2)
            rightPadding: ui.du(2.2)
            bottomPadding: ui.du(2.2)
            
            //! [0]
            LabelLabel {
                title: _bluetoothGatt.editor.characteristicUUID
                text: _bluetoothGatt.editor.characteristicHandle
            }
            
            LabelLabel {
                title: _bluetoothGatt.editor.characteristicName
                text: _bluetoothGatt.editor.characteristicValueHandle
            }
            //! [0]
            
            LabelLabel {
                title: qsTr("Flags:")
                text: _bluetoothGatt.editor.characteristicFlags
            }
            
            Container {
                topMargin: ui.du(1.1)
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    //! [1]
                    TextField {
                        text: _bluetoothGatt.editor.characteristicValue
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 10
                        }
                        id: characteristicValue
                        onTextChanged: {
                            _bluetoothGatt.editor.setCharacteristicValue( text )
                        }
                    }
                    //! [1]
                    
                    //! [2]
                    ImageView {
                        imageSource: "asset:///images/update.png"
                        contextActions: ActionSet {
                            ActionItem {
                                title: qsTr("Read")
                                imageSource: "asset:///images/ic_refresh.png"
                                enabled: _bluetoothGatt.editor.readCharacteristicValueAllowed
                                onTriggered: {
                                    _bluetoothGatt.editor.readCharacteristicValue()
                                }
                            }
                            ActionItem {
                                title: qsTr("Write")
                                imageSource: "asset:///images/ic_write.png"
                                enabled: _bluetoothGatt.editor.writeCharacteristicValueAllowed
                                onTriggered: {
                                    if (characteristicValueText.focused )
                                        _bluetoothGatt.editor.setCharacteristicValueText( characteristicValueText.text )
                                    else _bluetoothGatt.editor.setCharacteristicValue( characteristicValue.text )
                                    _bluetoothGatt.editor.writeCharacteristicValue(true)
                                }
                            }
                            ActionItem {
                                title: qsTr("Write (without response)")
                                imageSource: "asset:///images/ic_write.png"
                                enabled: _bluetoothGatt.editor.writeCharacteristicValueAllowed
                                onTriggered: {
                                    if (characteristicValueText.focused )
                                        _bluetoothGatt.editor.setCharacteristicValueText( characteristicValueText.text )
                                    else _bluetoothGatt.editor.setCharacteristicValue( characteristicValue.text )
                                    _bluetoothGatt.editor.writeCharacteristicValue(false);
                                }
                            }
                        }
                    }
                    //! [2]
                }
                
                TextArea {
                    text: _bluetoothGatt.editor.characteristicValueText
                    maxHeight: ui.du(27.7)
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 10
                    }
                    id: characteristicValueText
                    onTextChanged: {
                        _bluetoothGatt.editor.setCharacteristicValueText( text )
                    }
                }
            }
            
            Container {
                topMargin: ui.du(1.1)
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: qsTr("Notifications")
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                //! [3]
                ToggleButton {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    checked: _bluetoothGatt.editor.characteristicNotificationsEnabled
                    enabled: _bluetoothGatt.editor.characteristicNotificationsAllowed
                    onCheckedChanged: {
                        _bluetoothGatt.editor.characteristicNotificationsEnabled = checked
                    }
                }
                //! [3]
            }
            
            Container {
                topMargin: 10
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: qsTr("Indicators")
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                ToggleButton {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    checked: _bluetoothGatt.editor.characteristicIndicationsEnabled
                    enabled: _bluetoothGatt.editor.characteristicIndicationsAllowed
                    onCheckedChanged: {
                        _bluetoothGatt.editor.characteristicIndicationsEnabled = checked
                    }
                }
            }
            
            Divider {
            }
            
            Label {
                text: qsTr("Descriptors")
            }
            
            //! [4]
            ListView {
                dataModel: _bluetoothGatt.editor.descriptorsModel
                
                /*
                 * A ListItemComponent has its own context, and therefore its code cannot see globals (like _bluetoothGatt).
                 * 
                 * To get around this, we create wrapper functions in the ListView and call them from within the ListItemComponent,
                 * using the id: of the outermost Container (listItemContainer).
                 */
                function readCharacteristicDescriptor(indexInSection) {
                    _bluetoothGatt.editor.readCharacteristicDescriptor(indexInSection);
                }
                function writeCharacteristicDescriptor(indexInSection,text) {
                    _bluetoothGatt.editor.writeCharacteristicDescriptor(indexInSection,text);
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        
                        Container {
                            id: listItemContainer
                            
                            property int indexInSection: ListItem.indexInSection
                            
                            leftPadding: ui.du(1.1)
                            rightPadding: ui.du(1.1)
                            
                            background: ListItem.indexInSection % 2 == 1 ? Color.White : Color.LightGray
                            
                            Label {
                                text: qsTr("%1 / %2").arg(ListItemData.uuid).arg(ListItemData.name)
                            }
                            
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                TextField {
                                    id: textField
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 10
                                    }
                                    text: ListItemData.value
                                }
                                ImageView {
                                    imageSource: "asset:///images/update.png"
                                    contextActions: ActionSet {
                                        ActionItem {
                                            title: qsTr("Read")
                                            imageSource: "asset:///images/ic_refresh.png"
                                            onTriggered: {
                                                listItemContainer.ListItem.view.readCharacteristicDescriptor(listItemContainer.indexInSection);
                                            }
                                        }
                                        ActionItem {
                                            title: qsTr("Write")
                                            imageSource: "asset:///images/ic_write.png"
                                            onTriggered: {
                                                listItemContainer.ListItem.view.writeCharacteristicDescriptor(listItemContainer.indexInSection,textField.text);
                                            }
                                        }
                                    }
                                }
                            }
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                TextField {
                                    id: textFieldAscii
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 10
                                    }
                                    text: ListItemData.valueAscii
                                    enabled: false
                                }
                            }
                            
                            Divider {
                            }
                        }
                    }
                ]
            }
            //! [4]
        }
    }
}