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

//This is a little convinence method that adds text to  your checkbox.

Container {
    property alias objName: checkboxId.objectName
    property alias text: myLabel.text
    property bool pressed: false
    layout: StackLayout {
        layoutDirection: StackLayout.LeftToRight
    }
    Label {
        id: myLabel
        text: "Default Text"
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
            fontWeight: FontWeight.Bold
            color: Color.create ("#ffFDfDfD")
        }
        preferredWidth: 525
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }
    }
    CheckBox {
        id: checkboxId
        // Here we call the getValue for objectname,
        // be careful to set the objectname though.
        checked: _starshipApp.getValueFor (objectName, "yes")
        onCheckedChanged: {
            _starshipApp.saveValueFor (checkboxId.objectName, checked)
        }
    }
    onTouch: {
        if (event.isDown ()) {
            pressed = true;
        } else if (event.isUp ()) {
            if (pressed) {

                // Reverse the state of the check box.
                if (checkboxId.checked) {
                    checkboxId.checked = false;
                } else {
                    checkboxId.checked = true;
                }
            }
            pressed = false;
        }
    }
}
