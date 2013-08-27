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
//! [0]
Container {
    layout: DockLayout {
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        leftPadding: 30
        rightPadding: 30

        TextField {
            id: firstNameCreateTextField
            horizontalAlignment: HorizontalAlignment.Center

            hintText: qsTr("First Name")
        }

        TextField {
            id: lastNameCreateTextField
            horizontalAlignment: HorizontalAlignment.Center

            hintText: qsTr("Last Name")
        }

        Button {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("Create")

            onClicked: {
                _app.createRecord(firstNameCreateTextField.text, lastNameCreateTextField.text);
            }
        }
    }
}
//! [0]
