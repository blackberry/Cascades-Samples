/*!
 * Copyright (c) 2012, 2013  BlackBerry Limited.
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

// A custom text field with a label above it
Container {
    property alias description: label.text
    property alias text: textfield.text
    property alias hintText: textfield.hintText
    property alias inputMode: textfield.inputMode
    property alias textEnabled: textfield.enabled

    topPadding: ui.du(0)
    leftPadding: ui.du(2.2)
    rightPadding: ui.du(2.2)
    bottomPadding: ui.du(1.1)

    Container {
        leftPadding: ui.du(0.6)
        Label {
            id: label

        }
    }

    TextField {
        id: textfield
        input{
            flags: TextInputFlag.SpellCheckOff
        }
    }
}
