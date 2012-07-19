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

Page {
    Container {
        layout: AbsoluteLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/background.png"
        }
        
        // An image used to get some extra effect around the foreign window when the TV is on.
        ImageView {
            id: on_effects
            imageSource: "asset:///images/on_effects.png"
            opacity: 0.0
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 47
            }
        }
            
        // The ForeignWindow control will punch a hole in the Cascades frame buffer
        // allowing you to show whatever you like beneath it.
        ForeignWindow {
            id: myForeignWindow
            objectName: "myForeignWindow"
    
            // The window is not visible at launch turning on the TV will make it visible.
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 171
                positionY: 90
            }
            preferredWidth: 939
            preferredHeight: 528
        }
        
        ImageView {
            imageSource: "asset:///images/Remote_Bright.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 863
                positionY: 548
            }
        }
        
        ToggleButton {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 954
                positionY: 629
            }
            
            onCheckedChanged: {
                // Call the invokable function in code to start rendering noise to the window.
                foreignWindowApp.tvPower (checked);
                
                if (checked) {
                    // TV on show glow effect and make the window visible.
                    on_effects.opacity = 1.0;
                    on_effects.visible = true;
                    myForeignWindow.visible = true;
                } else {
                    // TV off turn off glow effect and make the window not visible.
                    on_effects.opacity = 0.0;
                    myForeignWindow.visible = false;
                }
            }
        }
    }
}
