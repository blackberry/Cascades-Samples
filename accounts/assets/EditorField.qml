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
    id: root

    property string type
    property string title
    property variant value

    topMargin: ui.du(5.6)

    onTitleChanged: {
        if (type == "string")
            textField.hintText = title
        else if (type == "email")
            emailField.hintText = title
        else if (type == "boolean")
            booleanField.text = title
        else if (type == "number")
            numberField.hintText = title
    }

    onValueChanged: {
        if (type == "string") {
            textField.text = value
        } else if (type == "email")
            emailField.text = value
        else if (type == "boolean")
            booleanField.checked = value
        else if (type == "number")
            numberField.text = value
    }

    TextField {
        id: textField

        visible: (root.type == "string")

        onTextChanging: root.value = text
    }

    TextField {
        id: emailField

        inputMode: TextFieldInputMode.EmailAddress
        visible: (root.type == "email")

        onTextChanging: root.value = text
    }

    CheckBox {
        id: booleanField

        visible: (root.type == "boolean")

        onCheckedChanged: root.value = checked
    }

    TextField {
        id: numberField

        inputMode: TextFieldInputMode.NumbersAndPunctuation
        visible: (root.type == "number")

        onTextChanging: root.value = text
    }
}
//! [0]
