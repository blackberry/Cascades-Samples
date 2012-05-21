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

// This is the speech bubble with the quote and the name beneath.

Container {
    id: quoteBubble
    property alias quote: longText.text
    property alias name: nameLabel.text
    property alias firstName: firstNameField.text
    property alias lastName: lastNameField.text
    property bool editMode: false
    
    layout: StackLayout {
        topPadding: 30
        bottomPadding: topPadding
        rightPadding: topPadding
        leftPadding: topPadding
    }    
    
    attachedObjects: [
      TextStyleDefinition
      {
        id: quoteText
        base: SystemDefaults.TextStyles.TitleText
      }
    ]
    
    // The two buttons for cancel and add/update actions, shown in edit mode.
    Container {
        visible: quoteBubble.editMode
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
            bottomPadding: 40
        }
        
        Button {
            text: "Cancel"
            onClicked: {
                console.debug ("Cancel return to main view.");
                quoteBubble.editMode = false;
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
        
        Label {
            id: editLabel
            text: "Edit"
            
            textStyle {
                base: quoteText.style
                color: Color.create ("#ffffff")
            }
            
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                spaceQuota: 1
            }
        }
        
        Button {
            id: saveButton
            text: "Update"
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            onClicked: {
                console.debug ("Update call code to edit record.");
                _quoteApp.updateSelectedRecord (firstNameField.text, lastNameField.text, longText.text);
                quoteBubble.editMode = false;
            }
        }
    }
    
    // The actual quote.
    Container {
        layout: DockLayout {
        }
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        // The bubble image behind the the quote. Since the image will scale due to
        // different lengths of the quote a nine sliced image is used to ensure  
        // only the middle part (the white area) is scaled, 
        // see images/border_bubble.amd.
        ImageView {
            imageSource: "asset:///images/border_bubble"
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }
        
        // The TextArea is put in a Container in order to put padding around it.
        Container {

            layout: StackLayout {
                topPadding: 54
                bottomPadding: 85
                rightPadding: 30
                leftPadding: rightPadding
            }
            
            TextArea {
                id: longText
                preferredWidth: 520
                maxHeight: {
                    if (quoteBubble.editMode) 400; else 900;
                }
                editable: quoteBubble.editMode
                backgroundVisible: quoteBubble.editMode
                
                // The quote text, data-bound from code.
                text: ""
                
                textStyle {
                    base: quoteText.style
                }
            }
        }
    }
    
    Container {
        layout: DockLayout {
            topPadding: 15            
        }
        
        preferredWidth: {
            if (quoteBubble.editMode) 768; else 520;
        }
        
        Label {
            id: nameLabel
            visible: !quoteBubble.editMode
            
            // The person behind the quote who's first and last name are data bound in C++.
            text: ""
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.create ("#ffffff")
            }
        }
        
        // Edit mode for name.
        Container {
            visible: quoteBubble.editMode
            layout: StackLayout {
                layoutDirection: StackLayout.LeftToRight
            }
                        
            // Text field for first name.
            TextField {
                id: firstNameField
                hintText: "First name"
                text: {
                    if(_contentPane.firstname != undefined)
                        _contentPane.firstname
                    else
                        ""
                }
                textStyle {
                    base: quoteText.style
                }

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
                        
            // Text field for last name.
            TextField {
                id: lastNameField
                hintText: "Last name"
                text: {
                    if(_contentPane.lastname != undefined)
                        _contentPane.lastname
                    else
                        ""
                }
                textStyle {
                    base: quoteText.style
                }

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                onTextChanging: {
                                
                    // A last name has to be entered disable buttons and text areas as long as the length is zero.
                    if (text.length > 0) {
                        saveButton.enabled = true;
                        longText.enabled = true;
                        firstNameField.enabled = true;
                    } else {
                        saveButton.enabled = false;
                        longText.enabled = false;
                        firstNameField.enabled = false;
                    }
                }
            }
        }
    }
}

