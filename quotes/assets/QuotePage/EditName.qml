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

// When the quote is in edit mode, it is possible to change the name.
// This control presents the input fields for editng the name of the entry.

Container {
    id: editName
    property alias firstName: firstNameField.text
    property alias lastName: lastNameField.text
    signal enableSave(bool enable)
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    // Text field for first name
    TextField {
        id: firstNameField
        hintText: "First name"

        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        text: _contentView.firstname
    }

    // Text field for last name
    TextField {
        id: lastNameField
        hintText: "Last name"        
        text: _contentView.lastname
                
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        
        onTextChanging: {
                        
            // Since the last name is the primary key for the entry. We will disable buttons and text areas 
			// until the last name length is not zero.
            if (text.length > 0) {
                editName.enableSave(true);
                firstNameField.enabled = true;
            } else {
                editName.enableSave(false);
                firstNameField.enabled = false;
            }
        }
    }// TextField
}