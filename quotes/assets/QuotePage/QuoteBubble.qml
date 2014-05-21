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

// This is the speech bubble with the quote and the name underneath it.

Container {
    id: quoteBubble
    property bool editMode: false
    property bool enableSave: false
    property string firstName: "John"
    property string lastName: "Doe"
    property alias pendingFirstName: editName.firstName
    property alias pendingLastName: editName.lastName

    property alias quoteText: longText.text
    
    topPadding: ui.du(2)
    bottomPadding: topPadding
    rightPadding: topPadding
    leftPadding: topPadding

    // The actual quote
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        layout: DockLayout {
        }

        // The bubble image behind the quote. Since the image will scale due to
        // different lengths of the quote a nine-sliced image is used to ensure
        // only the middle part (the white area) is scaled (see images/border_bubble.amd).
        ImageView {
            imageSource: "asset:///images/border_bubble.amd"
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }

        // TextArea that is put in a Container to add padding around it
        Container {
            topPadding: ui.du(6.6) 
            bottomPadding: ui.du(10.5) 
            rightPadding: ui.du(3) 
            leftPadding: rightPadding
            
            TextArea {
                id: longText
                preferredWidth: ui.du(52)
                editable: quoteBubble.editMode
                enabled: enableSave
                input.flags: TextInputFlag.SpellCheckOff
                textStyle.color: Color.create("#262626");
            }
        } // Text area Container
    } // Quote Container

    // The name of the person who wrote/said the quote
    Container {
        topPadding: ui.du(2) 
        layout: DockLayout {
        }
        Label {
            id: nameLabel
            visible: ! quoteBubble.editMode

            // The person behind the quote who's first and last name are data bound in C++.
            text: quoteBubble.firstName + " " + quoteBubble.lastName
            textStyle.base: quoteStyleLightBody.style
        }

        // Edit mode for name
        EditName {
            id: editName
            visible: quoteBubble.editMode
            firstName: quoteBubble.firstName
            lastName: quoteBubble.lastName

            onEnableSave: {
                // A last name has to be entered disable buttons and text areas as long as the length is zero.
                quoteBubble.enableSave = enable;
            }
        } // EditName
    } // Name Container
}// Main Container
