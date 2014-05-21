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
import "Common"

// A simple recipe showing how the ImageView can be added to a Container.
RecipePage {
    RecipeContainer {

        leftPadding: ui.du(2)
        rightPadding: leftPadding
        topPadding: leftPadding
        bottomPadding: leftPadding

        // An ImageView with a big juicy peach
        ImageView {
            id: pearImage
            imageSource: "asset:///images/peach_big.png"
            scalingMethod: ScalingMethod.AspectFit
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }

        // Let's use the filterColor feature to tint the image.
        CheckBox {
            verticalAlignment: VerticalAlignment.Bottom
            text: "filterColor"
            onCheckedChanged: {
                if (checked) {
                    pearImage.filterColor = Color.create("#ff9b617d")
                } else {
                    //restore the image
                    pearImage.filterColor = null
                }
            }
        }
    }
}
