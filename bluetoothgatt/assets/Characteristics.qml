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
    objectName: "CharacteristicsPage"

    titleBar: TitleBar {
        title: qsTr("Characteristics")
    }

    //! [0]
    attachedObjects: [
        ComponentDefinition {
            id: characteristicsEditorPage
            source: "CharacteristicsEditor.qml"
        }
    ]
    //! [0]

    Container {
        //! [1]
        ListView {
            dataModel: _bluetoothGatt.characteristicsModel

            onTriggered: {
                if (indexPath.length > 0) {
                    _bluetoothGatt.viewCharacteristicsEditor(indexPath[0]);
                    navigationPane.push(characteristicsEditorPage.createObject())
                }
            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        topPadding: ui.du(1.3)
                        leftPadding: ui.du(1.3)
                        rightPadding: ui.du(1.3)

                        Label {
                            text: ListItemData.uuid
                        }

                        Label {
                            text: ListItemData.name
                        }

                        Divider {
                        }
                    }
                }
            ]
        }
        //! [1]
    }
}