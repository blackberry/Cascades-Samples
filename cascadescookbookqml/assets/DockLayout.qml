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
import "Common"

// Recipe showing different ways one can align components in a dock layout.
// All layouts are set to center/center if the top bean is pressed.
RecipePage {
    RecipeContainer {
        background: backgroundPaint.imagePaint
	    
	    attachedObjects: [
	        ImagePaintDefinition {
	            id: backgroundPaint
	            imageSource: "asset:///images/docklayout/black_page.png"
	        }
	    ]
		    
        // Center bean, center/center aligned
        ImageView {
            id: centerBean
            imageSource: "asset:///images/docklayout/bean_centered.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }

        // Left bean, left/center aligned
        ImageView {
            id: leftBean
            imageSource: "asset:///images/docklayout/bean_vertical.png"
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
        }

        // Bottom bean, center/bottom aligned
        ImageView {
            id: bottomBean
            imageSource: "asset:///images/docklayout/bean_horizontal.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
        }

        // Right bean, right/center aligned
        ImageView {
            id: rightBean
            imageSource: "asset:///images/docklayout/bean_vertical.png"
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
        }
        ImageView {
            id: topBean
            imageSource: "asset:///images/docklayout/bean_horizontal.png"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
        }

        // A Label with instructional text that the background should be tapped
        // to set all alignment to center. It is put inside a Container in order
        // to offset it from the center so it is not overlapping the center image.
        Container {
            bottomPadding: 300
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Label {
                id: tapLabel
                text: "Tap to center"
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.LightGray
                }
            }
        }

        onTouch: {
            // On a touch up event, all alignments are set to center if the top bean is top-aligned.
            // If the top bean isn't top-aligned, all alignments are set to correspond to the original values.
            if (event.isUp ()) {
                if (topBean.verticalAlignment == VerticalAlignment.Top) {
                    topBean.verticalAlignment = VerticalAlignment.Center;
                    bottomBean.verticalAlignment = VerticalAlignment.Center;
                    rightBean.horizontalAlignment = HorizontalAlignment.Center;
                    leftBean.horizontalAlignment = HorizontalAlignment.Center;
                    tapLabel.opacity = 0;
                } else {
                    topBean.verticalAlignment = VerticalAlignment.Top;
                    bottomBean.verticalAlignment = VerticalAlignment.Bottom;
                    rightBean.horizontalAlignment = HorizontalAlignment.Right;
                    leftBean.horizontalAlignment = HorizontalAlignment.Left;
                    tapLabel.opacity = 1;
                }
            }// if statement
        }// onTouch
    }// RecipeContainer
}// RecipePage
