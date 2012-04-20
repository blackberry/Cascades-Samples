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
import "../EditScreen"

Page {
    content: Container {
        layout: DockLayout {
        }        

        // The Content pane background image.
        ImageView {
            imageSource: "asset:///images/background.png"
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }
 
        QuoteBubble {
            id: quoteBubble
            quote: _contentPane.quote
            name:  _contentPane.firstname + " " + _contentPane.lastname
            
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                
                // The QuoteBubble component is top aligned in edit mode to 
                // make room for the keyboard, otherwise it is center aligned.
                verticalAlignment: {                    
                    if(quoteBubble.editMode) {
                        VerticalAlignment.Top
                    } else {
                        VerticalAlignment.Center
                    }
                }
            }
        }
    }
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Names"
            onTriggered: { 
                // _navPane is set in code.
                _navPane.pop ();
                quoteBubble.editMode = false;
            }
        }
    }
    
    actions: [
        ActionItem {            
            title: "Edit"
            imageSource: "asset:///images/Edit.png"
            onTriggered: {
                quoteBubble.editMode = true
            }
        },
        
        // Forcing the delete action into to the overflow menu.
        ActionItem {},ActionItem {},ActionItem {},ActionItem {},
        
        ActionItem {
            objectName: "DeleteAction"
            title: "Delete"
            onTriggered: {
                 _quoteApp.deleteRecord();
                 quoteBubble.editMode = false
            }
        }
    ]
}
