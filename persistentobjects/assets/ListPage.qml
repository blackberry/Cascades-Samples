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
import PersistentObjectsLib 1.0 // needed to access enums of App class

Page {
    titleBar: TitleBar {
        title: qsTr("Persistent Objects Sample")
    }

    Container {
        //! [0]
        DropDown {
            horizontalAlignment: HorizontalAlignment.Fill

            title: qsTr("Save location")

            Option {
                text: qsTr("App Settings")
                description: qsTr("Store in the application's settings")
                value: App.StoreInSettings
                selected: true
            }

            Option {
                text: qsTr("Custom File")
                description: qsTr("Stream to file in data folder.")
                value: App.StoreInFile
            }

            onSelectedValueChanged: {
                _app.setStorageLocation(selectedValue);
            }
        }

        ListView {
            dataModel: _app.dataModel

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        imageSource: "asset:///images/icon1.png"
                        title: qsTr("%1 %2").arg(ListItemData.firstName).arg(ListItemData.lastName)
                        description: qsTr("Unique Key: %1").arg(ListItemData.customerID)
                    }
                }
            ]
        }

        Button {
            horizontalAlignment: HorizontalAlignment.Fill

            text: qsTr("Refresh")
            onClicked: {
                _app.refreshObjects(); // Refresh the list view.
            }
        }
        //! [0]
    }
}
