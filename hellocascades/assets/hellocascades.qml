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
    content: Container {
        // A DockLayout is applied to the main container, this makes it 
        // possible to layout controls and view inside the container. 
        layout: DockLayout {
        }
    
        // Here we create a background ImageView and set its source and size.
        // As you see the images width and height is set to the largest area it could possibly occupy. This 
        // will make the image stretch since it it's not that size in actual pixels. The reason for setting 
        // the size to this is so that the image will take the whole screen both in portraint and landscape.
        // For this image it will not look strange, but for other type of pixel content it could very 
        // well look mighty strange.
        ImageView {
            imageSource: "asset:///images/background.png"
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
        }

        // The container containing the bubble image and text.
        Container {
            // This container is also using a dock layout and it is centetered on the
            // background image by setting up the layoutProperties for the container.  
            layout: DockLayout {
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
 
            // The bubble image, if no size is set the image will animate to its correct size.
            ImageView {
                imageSource: "asset:///images/bubble.png"
                preferredWidth: 490
                preferredHeight: 487
            }
        
            // A text label with the comforting hello world text.
            Label {
                // Setting the label text and text style.
                text: "Hello World"
                
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
