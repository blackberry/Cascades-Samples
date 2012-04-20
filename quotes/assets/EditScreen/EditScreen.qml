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

//Page {content: 

// The Edit Screen is where the text areas for editing and filling in information for a new 
// record is added together with two buttons for add/edit and cancel.

Container {
    id: editPane
    property real margins: 40
    background: Color.create (0.9, 0.9, 0.9)
        
    layout: DockLayout {
        topPadding: editPane.margins
        leftPadding: editPane.margins
        rightPadding: editPane.margins
    }
    
    attachedObjects: [
      TextStyleDefinition
      {
        id: editTextStyle
        base: SystemDefaults.TextStyles.TitleText
      }
    ]
    
    Container {
        layout: StackLayout {
        }

        // The two buttons for cancel and add/update actions.
        Container {
                        
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }
            
            Button {
                text: "Cancel"
                onClicked: {
                    console.debug ("Cancel return to main view.");
                    hide ();
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            
            Label {
                id: editLabel
                text: "Add"
                textStyle {
                    base: editTextStyle.style
                    alignment: TextAlignment.Center
                }

                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Center
                    spaceQuota: 1
                }
            }
            
            Button {
                id: saveButton
                text: "Save"
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                onClicked: {
                    if (saveButton.text == "Save") {
                        console.debug ("Save call code to create a new record.");
                        _quoteApp.addNewRecord (firstNameField.text, lastNameField.text, quoteField.text);
                    } else {
                        console.debug ("Update call code to edit record.");
                        _quoteApp.updateSelectedRecord (firstNameField.text, lastNameField.text, quoteField.text);
                    }
                    hide ();
                }
            }
        }            

        // The quote text area.
        TextArea {
            id: quoteField
            hintText: "Quote"
            objectName: "longText"
            topMargin: editPane.margins
            bottomMargin: topMargin
            preferredHeight: 500
            
            textStyle {
                base: editTextStyle.style
            }

            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }
        
        Container {
            layout: StackLayout {
                layoutDirection: StackLayout.LeftToRight
            }
                
            // Text field for the first name.
            TextField {
                id: firstNameField
                rightMargin: editPane.margins
                hintText: "First name"
                textStyle {
                    base: editTextStyle.style
                }
            }
                
            // Text field for the last name.
            TextField {
                id: lastNameField
                hintText: "Last name"
                
                textStyle {
                    base: editTextStyle.style
                }

                onTextChanging: {
                        
                        // A last name has to be entered disable buttons and text areas as long as the lenth is zero.
                    if (text.length > 0) {
                        saveButton.enabled = true;
                        quoteField.enabled = true;
                        firstNameField.enabled = true;
                    } else {
                        saveButton.enabled = false;
                        quoteField.enabled = false;
                        firstNameField.enabled = false;
                    }
                }
            }
        }
    }
    
     
    // Function launches the Edit screen with preopulated text areas, so that a quote can be edited.
    function editQuote (firstname, lastname, quote) {
        show ();
        firstNameField.text = firstname;
        lastNameField.text = lastname;
        quoteField.text = quote;
        editLabel.text = "Edit";
        saveButton.text = "Update";
    }
    
    // Function launches the edit screen with empty text areas.
    function newQuote () {
        show ();
        firstNameField.text = "";
        lastNameField.text = "";
        quoteField.text = "";
        editLabel.text = "Add";
        saveButton.text = "Save";
    }

    // Helper function for showing the edit field.
    function show () {        
        translationY = 0;
    }

    // This function hides the edit screen translating it to the full height of the screen.
    function hide () {        
        translationY = 1280;
    }
    
    onCreationCompleted: {
        // The input Container is outside the screen when the edit screen is not active.
        translationY = 1280
    }
}
//}
