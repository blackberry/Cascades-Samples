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
    property bool editMode: false
    layout: StackLayout {
        topPadding: 30
        bottomPadding: topPadding
        rightPadding: topPadding
        leftPadding: topPadding
    }
    attachedObjects: [
        TextStyleDefinition {
            id: quoteText
            base: SystemDefaults.TextStyles.TitleText
        }
    ]
    
    // The two buttons for cancel and add/update actions, shown in edit mode.
    EditControls {
        id: editControls
        visible: quoteBubble.editMode
        
        onCancel: {
            // Revert back to the data that is stored in the _contentView context property.
            _quoteApp.updateSelectedRecord (_contentView.firstname, _contentView.lastname, _contentView.quote);
            quoteBubble.editMode = false;
        }
        
        onUpdate: {
            // Update to the values entered in the fields.
            _quoteApp.updateSelectedRecord (editName.firstName, editName.lastName, longText.text);
            quoteBubble.editMode = false;
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
                preferredHeight: {
                    if (quoteBubble.editMode) {
                         200; 
                     } else {
                         undefined;
                     }
                }

                maxHeight: {
                    if (quoteBubble.editMode) {
                         200; 
                     } else {
                         900;
                     }
                }
                editable: quoteBubble.editMode
                
                // The quote text, data bound from code.
                text: _contentView.quote
                textStyle {
                    base: quoteText.style
                }
            }
        }
    }
    
    // The Name of the person behind the quote.
    Container {
        layout: DockLayout {
            topPadding: 15
        }
        
        preferredWidth: {
            if (quoteBubble.editMode) { 
                undefined; 
            } else {
                520;
            }
        }
        
        Label {
            id: nameLabel
            visible: !quoteBubble.editMode
            
            // The person behind the quote who's first and last name are data bound in C++.
            text: _contentView.firstname + " " + _contentView.lastname;
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.create ("#fafafa")
            }
        }
        
        // Edit mode for name.
        EditName {
            id: editName;
            visible: quoteBubble.editMode
            
            onEnableSave: {
                // A last name has to be entered disable buttons and text areas as long as the length is zero.
                if (enable) {
                    editControls.updateEnabled = true;
                    longText.enabled = true;
                } else {
                    editControls.updateEnabled = false;
                    longText.enabled = false;
                }
            }
        }
    }
}
