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
import Dialog.Alert 1.0
import Dialog.ContextMenu 1.0
import Dialog.Prompt 1.0
import Dialog.Login 1.0

Page {
    content: Container {
        background : Color.Black
        layout : StackLayout {
            orientation : LayoutOrientation.TopToBottom
        }
        Label {
            id: alertDialogLabel
        }
        Button {
            layoutProperties : StackLayoutProperties {         
            }
            horizontalAlignment : HorizontalAlignment.Center
            text : "Simple Alert"
            onClicked: {
                alertDialog.addCustomButton("Cheese","AlertDialog_Cheese");
                alertDialog.addCustomButton("Cake","AlertDialog_Cake");
                alertDialog.show();
            }
        }
        Divider {}
        Label {
            id: contextMenuLabel
        }
        Button {
            layoutProperties : StackLayoutProperties {
            }
            horizontalAlignment : HorizontalAlignment.Center
            text : "Context Menu"
            onClicked: {
                contextMenu.addCustomButton("Rotate","Menu_Rotate");
                contextMenu.addCustomButton("Translate","Menu_Translate");
                contextMenu.show();
            }
        }
        Divider {}
        Label { id: promptDialogLabel }
        Button {
            layoutProperties : StackLayoutProperties {
            }
            horizontalAlignment : HorizontalAlignment.Center
            text : "Prompt Dialog"
            onClicked: {
                promptDialog.show();
            }
        }
        Divider {}
        Label { id: loginDialogLabel }
        Button {
            layoutProperties : StackLayoutProperties {
            }
            horizontalAlignment : HorizontalAlignment.Center
            text : "Login Dialog"
            onClicked: {
                loginDialog.addCustomButton("Something","Login_Something");
                loginDialog.show();
            }
        }
        attachedObjects: [
            AlertDialog {
                id: alertDialog
                cancelButton : true
                okButton : true
                alertMessage : "i am heavy metal!"
                titleText : "not a heavy metal fan :("
                onOk: { 
                    alertDialogLabel.text = "You clicked on OK button"; 
                }
                onCancel: {
                    alertDialogLabel.text = "You clicked on CANCEL button"; 
                }
                onCustomButton: {
                    alertDialogLabel.text = "index: " + index + " label: " + label + " context: " + context; 
                }
            },
            ContextMenu {
                id: contextMenu
                copyButton : true
                pasteButton : true
                cutButton : true
                onCopy: {
                    contextMenuLabel.text = "You clicked on copy";
                }
                onCut: {
                    contextMenuLabel.text = "You clicked on cut";
                }
                onPaste: {
                    contextMenuLabel.text = "You clicked on paste";
                }
                onCustomButton: {
                    contextMenuLabel.text = "index: " + index + " label: " + label + " context: " + context; 
                }
            },
            PromptDialog {
                id: promptDialog
                cancelButton : true
                okButton : true
                promptMessage : "Would you have some Grey Poupon?"
                placeholder : "but of course"
                titleText : "Pardon Me"
                onOk: { 
                    promptDialogLabel.text = inputField; 
                }
                onCancel: {
                    promptDialogLabel.text = "You clicked on CANCEL button"; 
                }
                onCustomButton: {
                    promptDialogLabel.text = "index: " + index + " label: " + label + " context: " + context; 
                }
            },
            LoginDialog {
                id: loginDialog
                cancelButton : true
                okButton : true
                loginMessage : "Enter your twitter login info"
                usernamePlaceholder : "enter username here"
                usernameLabel : "Twitter Email"
                passwordPlaceholder : "enter password here"
                passwordLabel : "Twitter Password"
                remembermeLabel : "Save login info"
                rememberme : true
                titleText : "Login to twitter"
                onOk: { 
                    loginDialogLabel.text = loginDialog.username + "/" + loginDialog.password; 
                }
                onCancel: {
                    loginDialogLabel.text = "You clicked on CANCEL button"; 
                }
                onCustomButton: {
                    loginDialogLabel.text = "index: " + index + " label: " + label + " context: " + context; 
                }
            }
        ]
    }
}
