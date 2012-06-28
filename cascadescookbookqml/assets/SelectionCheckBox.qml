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

// Since we do not only want to present the CheckBox but also
// information about what the name and color the olive has we set
// up a QML component which can be reused for the selection list
// in Selection.qml
Container {
    id: checkContainer
    property alias oliveColor: oliveColorContainer.background
    property alias oliveName: oliveCheckBox.text
    property real oliveColorSize: 42
    property real checkBoxPadding: 41

    // A colored rectangle representing the olive's color and the
    // CheckBox is aligned to the Right side of the component.
    layout: StackLayout {
        layoutDirection: LayoutDirection.LeftToRight
        topPadding: checkContainer.checkBoxPadding
        bottomPadding: checkContainer.checkBoxPadding
    }

    // The CheckBox which will tell if the olive should be part of the
    // olive mix or not.
    CheckBox {
        id: oliveCheckBox
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
            spaceQuota: 1
        }

        // When the checked state changes for the CheckBox a signal is
        // caught in this slot function and we print a status to the the console.
        onCheckedChanged: {
            if (checked) {
                console.debug (oliveName + " added to the mix")
            } else {
                console.debug (oliveName + " removed from the mix")
            }
        }
    }

    Container {
        id: oliveColorContainer
        rightMargin: checkContainer.checkBoxPadding
        background: Color.create ("#808000")
        preferredWidth: checkContainer.oliveColorSize
        preferredHeight: checkContainer.oliveColorSize
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }
    }
}
