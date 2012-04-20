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
    content: Container {
        layout: AbsoluteLayout {
        }

        preferredWidth: 1280
        preferredHeight: 768
        
        ImageView {
            imageSource: "asset:///images/background.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredWidth: 1280
            preferredHeight: 768
        }
        
        ImageView {
            imageSource: "asset:///images/tv.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 81
                positionY: 58
            }
            preferredWidth: 1118
            preferredHeight: 651
        }
        
        ImageView {
            id: on_effects
            imageSource: "asset:///images/on_effects.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 47
            }
            preferredWidth: 1280
            preferredHeight: 721
            opacity: 0.0
        }
        
        // The ForeignWindow control will punch a hole in the Cascades frame buffer
        // allowing you to show whatever you like beneath it.
        ForeignWindow {
            id: myForeignWindow
            objectName: "myForeignWindow"
            
            // The window is not visible at launch turning on the tv will make it visible.
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 171
                positionY: 90
            }
            preferredWidth: 939
            preferredHeight: 528

        }
        
        ImageView {
            imageSource: "asset:///images/Remote.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 864
                positionY: 547
            }
            preferredWidth: 388
            preferredHeight: 221
        }
        
        // A Toggle for turning the TV on and off.
        ToggleButton {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 1000
                positionY: 635
            }
            
            onCheckedChanged: {
                // Call the invokable function in code to start rendering noise to the window.
                foreignWindowApp.tvPower (checked);
                
                if (checked) {
                    // Tv on show glow effect and make the window visible.
                    on_effects.opacity = 1.0;
                    myForeignWindow.visible = true;
                } else {
                    // Tv off turn off glow effect and make the window not visible.
                    on_effects.opacity = 0.0;
                    myForeignWindow.visible = false;
                }
            }
        }
    }
}
