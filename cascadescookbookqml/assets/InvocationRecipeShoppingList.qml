/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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

// This recipe illustrates the usage of Invocation and InvokeActionItem.
Container {
    id: invokeContainer
    property bool hasPendingInvocation: false
    property alias title: titleLabel.text
    property alias message: messageField .text
    property alias adress : adressField.text
    
    signal invoke(string value)
    signal selectedInvocationOptionChanged(string href)

    topPadding: ui.du(2)
    leftPadding: ui.du(2)
    rightPadding: ui.du(2)

    layout: StackLayout {
    }

    DropDown {
        title: "Delivery method"
        id: invokeSelection
        options: [
            Option {
                // Each option has a string to update which web page should be open by the infoItem InvokeActionItem.
                property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/sending_invocation.html"

                // The id of the Invocation set up as an attached object below.
                value: "unbound"

                description: "Don't know"
                selected: true
            },
            Option {
                property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/bbm.html"
                value: "bbm"
                description: "BBM"
            },
            Option {
                property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/email.html"
                value: "email"
                description: "Email"
            }
        ]

        onSelectedOptionChanged: {
            selectedInvocationOptionChanged(selectedOption.href);
        }

        onSelectedValueChanged: {
            // As the selected value changes we show and hide certain text fields .
            adressField.visible = false

            if (selectedValue == "email") {
                adressField.visible = true
            }
        }
    }

    Label {
        id: titleLabel
        text: "Shopping List"
    }

    TextField {
        id: adressField
        text: "someemailaddress@cookbook.xyz"
        hintText: "Email address"
        inputMode: TextFieldInputMode.EmailAddress
        
        // We want the keyboard to be Email input.
        input
        {
            keyLayout: KeyLayout.EmailAddress
        }
        
        // The email address field is initially hidden, only to be shown when the Email option is selected in the DropDown.
        visible: false
    }

    TextArea {
        id: messageField
        hintText: "Enter items to buy"
        text: "Chocolate\n" + "Fudge\n" + "Turkish Delight"
    }

    Button {
        id: invokeButton

        text: "Invoke!"
        enabled: ! hasPendingInvocation

        onClicked: {
            invoke(invokeSelection.selectedOption.value);
        }
    }
}
