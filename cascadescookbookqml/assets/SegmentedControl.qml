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

// A recipe showing how to listen to changes in SegmentedControl
// and use this to change the look of the interface.
RecipePage {
    RecipeScrollView {
        Container {
            topPadding: ui.du(3)
            leftPadding: ui.du(2)
            rightPadding: leftPadding

            // The segmented control is implemented and given three values and text
            // to represent those values.
            SegmentedControl {
                id: segmented
                Option {
                    text: "Leek"
                    value: "1"
                    selected: true
                }
                Option {
                    text: "Beetroot"
                    value: "2"
                }
                Option {
                    text: "Garlic"
                    value: "3"
                }
                
                
                // Here we listen for when the user changes the value.
                onSelectedIndexChanged: {
                    
                    // Depending on what value is chosen the title and and image is
                    // changed to match the ingredient of choice.
                    if (segmented.selectedValue == 1) {
                        titleLabel.text = "Potato Leek Soup";
                        soupImage.imageSource = "asset:///images/segmentedcontrol/soup_green.png";
                    } else if (segmented.selectedValue == 2) {
                        titleLabel.text = "Borstch";
                        soupImage.imageSource = "asset:///images/segmentedcontrol/soup_beet.png";
                    } else if (segmented.selectedValue == 3) {
                        titleLabel.text = "French Farmers Soup";
                        soupImage.imageSource = "asset:///images/segmentedcontrol/soup_white.png";
                    }
                }
            }
            
            // This labels text is altered when the segmentedControl value is changed.
            Label {
                id: titleLabel
                text: "Potato Leek Soup"
                textStyle.base: SystemDefaults.TextStyles.BigText
                horizontalAlignment: HorizontalAlignment.Fill
            }

            // This ImageView's imageSource is altered when the segmentedControl value is changed.
            ImageView {
                id: soupImage
                imageSource: "asset:///images/segmentedcontrol/soup_green.png"
                scalingMethod: ScalingMethod.AspectFit
            }
        }
    }
}
