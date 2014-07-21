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

Page {
    // Main Container
    Container {
        layout: AbsoluteLayout {
        }
        ImageView {
            imageSource: "asset:///images/background.png"
        }

        // An image used to get some extra effect around the foreign window when the TV is on
        ImageView {
            id: on_effects
            imageSource: "asset:///images/on_effects.png"
            opacity: 0.0
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
        }

        // The ForeignWindowControl will punch a hole in the Cascades frame buffer
        // allowing you to show whatever you like beneath it.
        ForeignWindowControl {
            id: myForeignWindow
            objectName: "myForeignWindow"

            // The window is not visible at launch; turning on the TV will make it visible.
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 158
                positionY: 218
            }
            preferredWidth:  ui.px(1126)
            preferredHeight: ui.px(632)
            
            accessibility{
                name:"Foreign window control."
            }
        }
        // Mascot image
        ImageView {
            imageSource: "asset:///images/mascot.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 6.0
                positionY: 0.0
            }
        }
        // TV remote image
        ImageView {
            imageSource: "asset:///images/remote.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
        }
        // Button for remote
        ToggleButton {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 850
                positionY: 1150
            }
            onCheckedChanged: {
                // Call the invokable function in code to start rendering noise to the window.
                foreignWindowApp.tvPower(checked);
                
                if (checked) {
                    // TV is on so show glow effect and make the window visible.
                    on_effects.opacity = 1.0;
                    on_effects.visible = true;
                    myForeignWindow.visible = true;
                } else {
                    // TV is off so turn off glow effect and make the window not visible
                    on_effects.opacity = 0.0;
                    myForeignWindow.visible = false;
                }
            }
            
            accessibility{
                name: "Toggel Button"
                description: (checked)?"The tv is on":"The tv is off" 
            }
            
        }
    } 
}
