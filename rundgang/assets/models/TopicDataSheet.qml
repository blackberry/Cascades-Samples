/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
import com.rundgang 1.0
import bb.cascades.pickers 1.0

// A sheet with forms for adding a new Topic.
Sheet {
    id: newTopicSheet
    
    // Emitted when the data is supposed to be stored.
    signal saveTopic(string topic, string adressList);
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Topic") + Retranslate.onLocaleOrLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLocaleOrLanguageChanged
                
                onTriggered: {
                    // Just close if the user cancels the operation.
                    newTopicSheet.close()
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save") + Retranslate.onLocaleOrLanguageChanged
                
                onTriggered: {
                    // Save and close if the user hits save.
                    newTopicSheet.saveTopic("#" + topicField.text, adressField.text)
                    newTopicSheet.close();
                }
            }
        }
        
        Container {
            topPadding: ui.du(2)
            rightPadding: topPadding
            leftPadding: topPadding
            
            TextField {
                id: topicField
                hintText: qsTr("Enter a new Topic") + Retranslate.onLocaleOrLanguageChanged
            }
            
            TextArea {
                id: adressField
                maxHeight: ui.du(20)
                minHeight: ui.du(20)
                hintText: qsTr("Enter a comma separated email list") + Retranslate.onLocaleOrLanguageChanged
                
                // Lets specify a keyboard for email input.
                input
                {
                    keyLayout: KeyLayout.EmailAddress
                }
                content {
                    flags: TextContentFlag.ActiveTextOff
                }
            }
            
            Button {
                text: qsTr("Add from contacts") + Retranslate.onLocaleOrLanguageChanged
                onClicked: {
                    contactPicker.open()
                }
                
                onCreationCompleted: {
                    // Set up filters so that only contacts with emails are shown.
                    emailController.setupEmailFilter(contactPicker)
                }
                               
                attachedObjects: [
                    // The ContactPicker and the ContactSelectionMode types are registered in rungangapp.cpp
                    ContactPicker {
                        id: contactPicker
                        mode: ContactSelectionMode.Attribute
                        
                        onContactAttributeSelected: {
                            // Get the email address from the returned attribute and add it to the adressField.                            
                        	var emailAdress = emailController.getEmailForAttribute(attribute);
                            if(adressField.text == ""){
                                adressField.text = emailAdress
                            } else {
                            	adressField.text = adressField.text + ", " + emailAdress    
                            }
                        }

                        onError: {
                            // An error can happen for more reasons then the permission being set, 
                            // but that's one of the reasons. Preferably one would like a more solid way
                            // of checking that the user denying access to contacts disable the picker entirely. 
                            displayPermissionError(qsTr("Its not possible to select contacts, maybe you have to give us permission to do that?"))
                        }
                    },
                    // This is a Custom QObject (common/emailcontroller.cpp/.h) that assist
                    // the picker with some C++ operations that are needed to handle the email addresses.
                    EmailController {
                        id: emailController
                    }
                ]
            }
        }
    }
    
    onOpened: {
        // Set the topic field to be focused as the sheet opens so that the user
        // can start typing immediately.
        topicField.requestFocus();
    }
    
    onClosed: {
    	adressField.text = "";
    	topicField.text = "";
    }
}