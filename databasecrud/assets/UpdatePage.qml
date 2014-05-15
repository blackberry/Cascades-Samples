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
//! [0]
Container {
    layout: DockLayout {
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        leftPadding: ui.du(3.3)
        rightPadding: ui.du(3.3)

        TextField {
            id: firstNameUpdateTextField
            horizontalAlignment: HorizontalAlignment.Center

            hintText: qsTr("New first name")
        }

        TextField {
            id: lastNameUpdateTextField
            horizontalAlignment: HorizontalAlignment.Center

            hintText: qsTr("New last name")
        }

        TextField {
            id: idUpdateTextField
            horizontalAlignment: HorizontalAlignment.Center

            hintText: qsTr("Key of record to update")
        }

        Button {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("Update")

            onClicked: {
                _app.updateRecord(idUpdateTextField.text, firstNameUpdateTextField.text, lastNameUpdateTextField.text);
            }
        }
    }
}
//! [0]