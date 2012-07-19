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
// Importing all our cascades functions.
import bb.cascades 1.0

Page {
    // A container is used to gather visual items together.
    Container {
        // A DockLayout is applied to the main container, this makes it
        // possible to layout controls and view inside the container.
        layout: DockLayout {
        }
        
        // The Container is painted with an image paint, an image paint 
        // can be tiled (in which case it has to be power of two in size)
        // but in this case it is simply an image of the size of the screen. 
        background: backgroundPaint.imagePaint
        
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/background.png"
            }
        ]

        // The container containing the bubble image and text.
        Container {
            // This container is also using a dock layout and it is centered on the
            // background image by setting up the layoutProperties for the container.
            layout: DockLayout {
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            // The bubble image.
            ImageView {
                imageSource: "asset:///images/bubble.png"
            }

            // A text label with the comforting hello world text.
            Label {
                // Setting the label text, by using qsTr() the string can be translated.
                text: qsTr("Hello World")

                // A text style definition for the Hello Cascades text, specifying
                // the font (we want it big) and changing the color to a stylish gray.
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    fontWeight: FontWeight.Bold
                    color: Color.create ("#ff5D5D5D")
                }

                // Center the text in the container.
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }
    }
}