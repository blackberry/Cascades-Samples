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
import bb.system 1.0

Page {
    //! [0]
    attachedObjects: [
        SystemToast {
            id: toast
            body: qsTr("Toast body")
            icon: "asset:///images/Battery-low.png"
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    console.log("confirm");
                } else if (result == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                } else if (result == SystemUiResult.TimeOut) {
                    console.log("timeout");
                }
            }
        },
        SystemPrompt {
            id: prompt
            title: qsTr("PROMPT")
            modality: SystemUiModality.Application
            inputField.inputMode: SystemUiInputMode.Password
            confirmButton.label: qsTr("Okay button")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel button")
            cancelButton.enabled: true
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    console.log("confirm");
                } else if (result == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                }
            }
        },
        SystemCredentialsPrompt {
            id: credentialsPrompt
            title: qsTr("CREDENTIALS PROMPT")
            body: qsTr("Credentials dialog body")
            includeShowPassword: true
            includeRememberMe: true
            confirmButton.label: qsTr("Login button")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel button")
            cancelButton.enabled: true
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    console.log("confirm");
                } else if (result == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                }
            }
        },
        SystemDialog {
            id: dialog
            title: qsTr("DIALOG")
            body: qsTr("Dialog body")
            confirmButton.label: qsTr("Okay button")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel button")
            cancelButton.enabled: true
            buttons: [
                SystemUiButton {
                    id: random
                    label: qsTr("RANDOM")
                    enabled: true
                },
                SystemUiButton {
                    id: random2
                    label: qsTr("RANDOM2")
                    enabled: true
                }
            ]
            onFinished: {
                var x = result;
                console.log(dialog.error);
                if (x == SystemUiResult.ConfirmButtonSelection) {
                    console.log("confirm");
                } else if (x == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                } else if (x == SystemUiResult.ButtonSelection) {
                    console.log("button");
                } else if (x == SystemUiResult.None) {
                    console.log("none");
                } else if (x == SystemUiResult.Error) {
                    console.log("error");
                } else if (x == SystemUiResult.TimeOut) {
                    console.log("timeout");
                } else {
                    console.log(x);
                }
            }
        }

    ]
    //! [0]

    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }
        DialogsScrollView {

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center

                leftPadding: 20
                topPadding: 20
                rightPadding: 20
                bottomPadding: 20

                Label {
                    horizontalAlignment: HorizontalAlignment.Center

                    text: qsTr("Dialog Demo")
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    textStyle.color: Color.White
                }

                TextField {
                    topMargin: 100

                    hintText: qsTr("Enter text of dialog")
                }

                RadioGroup {
                    id: group

                    topMargin: 20

                    Option {
                        id: option1
                        text: qsTr("Toast")
                        value: toast
                        selected: true
                    }

                    Option {
                        id: option2
                        text: qsTr("Prompt")
                        value: prompt
                    }

                    Option {
                        id: option3
                        text: qsTr("Credentials Prompt")
                        value: credentialsPrompt
                    }

                    Option {
                        id: option4
                        text: qsTr("Dialog")
                        value: dialog
                    }
                }

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    topMargin: 40

                    //! [1]
                    Button {
                        id: show
                        text: qsTr("Show")
                        onClicked: group.selectedValue.show()
                    }
                    //! [1]

                    //! [2]
                    Button {
                        id: cancel
                        text: qsTr("Cancel")
                        onClicked: group.selectedValue.cancel()
                    }
                    //! [2]
                }
            }
        }
    }
}
