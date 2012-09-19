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

// Import all our cascades functions.
import bb.cascades 1.0

Page {
    // A container is used to gather visual items together.
    Container {
        // A DockLayout is applied to the main container, making it
        // possible to layout controls and view inside the container.
        layout: DockLayout {
        }

        // The Container is painted with an imagePaint. An imagePaint
        // can be tiled, in which the size must be power of two.
        // In this case, it is simply an image of the size of the screen
        // to set our background.
        background: backgroundPaint.imagePaint

        // The container containing the bubble image and text
        Container {
            // This container is also using a dock layout and it is centered on the
            // background image by setting up the layoutProperties for the container.
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: DockLayout {
            }

            // The bubble image
            ImageView {
                imageSource: "asset:///images/bubble.png"
            }

            // A text label with the comforting hello world text
            Label {
                // Set the label text, by using qsTr() the string can be translated.
                text: qsTr("Hello World")
                
                // The Label text style defined in the attachedObjects below
                textStyle.base: helloTextStyle.style
                
                // Center the text in the container.
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }// Label
        }// Container
    }// Container
    
    attachedObjects: [
        // Non UI objects are specified as attached objects
        TextStyleDefinition {
            // A text style definition for the Hello Cascades text, specifying
            // the font (we want it big) and changing the color to a stylish gray
            id: helloTextStyle
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Bold
            color: Color.create("#ff5D5D5D")
        },
        ImagePaintDefinition {
            id: backgroundPaint
            imageSource: "asset:///images/background.png"
        }
    ]
}// Page 
