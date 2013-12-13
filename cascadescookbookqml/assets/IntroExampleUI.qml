/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2

// This QML Component is used by the intro recipe. It sets up a Container
// showcasing some of the Core Controls available in Cascades. A text area,
// Button, Slider, and ToggleButton is added on top of a nice looking Background image.
Container {

    // A text area is set to receive text input, aligned to the top right corner.
    TextArea {
        maxHeight: 400
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        
        layout: DockLayout {
        }
        
        // A regular Button
        Button {
            text: "Button"
        }

        // A ToggleButton
        ToggleButton {
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center            
        }
    }
    
    // An example of a Slider control
    Slider {
        value: 0.333
        horizontalAlignment: HorizontalAlignment.Fill
    }
}
