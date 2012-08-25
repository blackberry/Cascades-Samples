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

/**
*  This is a layout that displays readings from the devices built in sensors
*
*  You will learn how to:
*  -- Define custom properties and reuse them
*  -- Use Javascript functions to set animation properties for the entire Pane
**/

Container {
    id: bottomPane
    property real margins: 40
    background: Color.create (0.0, 0.0, 0.0)
        
    layout: DockLayout {
        topPadding: bottomPane.margins
        leftPadding: bottomPane.margins
        rightPadding: bottomPane.margins
        bottomPadding: 1000
    }
    
    attachedObjects: [
      TextStyleDefinition
      {
        id: editTextStyle
        base: SystemDefaults.TextStyles.TitleText
        color: Color.White
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
                    text: "Settings"
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
                        hide ();
                    }
                }
            }            
        } // Add POPUP
    
	
	    // Helper function for showing the edit field.
	    function show () {        
	        translationY = 0;
	    }
	
	    // This function hides the edit screen translating it to the full height of the screen.
	    function hide () {        
	        translationY = 1280;
	    }
	    
	    function trigger () {
	        if (translationY == 0)
	            translationY = 1280;
	        else
	            translationY = 0;    
	    }
	    
        onCreationCompleted: {
            // The input Container is outside the screen when the edit screen is not active.
            translationY = 1280
        }

}