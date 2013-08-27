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

Page {
    titleBar: TitleBar {
        title: qsTr("Persistent Objects Sample")
    }

    Container {
        layout: DockLayout {
        }
        //! [0]
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            leftPadding: 30
            rightPadding: 30

            TextField {
                id: idField
                horizontalAlignment: HorizontalAlignment.Center

                hintText: qsTr("ID")
            }

            TextField {
                id: firstName
                horizontalAlignment: HorizontalAlignment.Center

                hintText: qsTr("first name")
            }

            TextField {
                id: lastName
                horizontalAlignment: HorizontalAlignment.Center

                hintText: qsTr("last name")
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Update")

                onClicked: {
                    _app.updateObject(idField.text, firstName.text, lastName.text);
                }
            }
        }
        //! [0]
    }
}
