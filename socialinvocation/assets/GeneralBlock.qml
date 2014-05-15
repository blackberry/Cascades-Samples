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

// The general Status Update text field that is shown on each tab
//! [0]
Container {

    property alias statusLabel: labelStatus.text
    property alias statusText: textStatus.text

    leftPadding: ui.du(2.2)
    rightPadding: ui.du(2.2)
    topPadding: ui.du(2.2)
    bottomPadding: ui.du(2.2)

    Label {
        id: labelStatus
        text: qsTr("What do you want to share?")
        textStyle.fontWeight: FontWeight.Bold
    }

    TextField {
        id: textStatus
    }
}
//! [0]
