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

// This recipe shows how a nine-sliced image works. The nice thing
// with images that can be nine-sliced is that the size can be altered
// without scaling the entire image. A file accompanying the image
// contains margins, which will be unaffected by scaling. If setup
// properly, the image can scale without getting scaling artifacts and
// distortions around the edges.
RecipePage {
    RecipeContainer {

        // Container for the nine-sliced image
        Container {
            topPadding: ui.du(3)
            horizontalAlignment: HorizontalAlignment.Center
            
            layout: DockLayout {
            }

            // This is the image that has nine-slice margins applied to it.
            // So scaling of this image will not cause scaling artifacts around
            // the image, but rather an area in the middle is scaled. The margins
            // that are applied can be found in the file border_image_Pasta.amd.
            ImageView {
                imageSource: "asset:///images/nineslice/border_image_Pasta.amd"
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            // The length of the text in this TextArea will effect its height,
            // which will also alter the height of the surrounding Container.
            // Since the above ImageView is set to fill the entire Container, the
            // scaling of a nine-sliced image is illustrated by switching between
            // short (lazy) and long (diligent) recipe text (see RadioGroup below).
            Container {
                topPadding: ui.px(40)
                bottomPadding: ui.px(80)
                leftPadding: ui.px(40)
                rightPadding: leftPadding
                
                layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                
                Label {
                    id: nineSliceText
                    multiline: true
                    textStyle.base: textStyleLasangaPlate.style
                }
                Label {
                    id: nineSliceText_2
                    multiline: true
                    textStyle.base: textStyleLasangaPlate.style    
                }
                
            }
        }

        // To show how a nine-sliced image is scaled, we provide the possibility to
        // alter the text between two different lengths. This text switch will cause the text
        // area to change size and consequently the image will be changing size.
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            
            Divider {
            }
            
            RadioGroup {
                id: lasagnaOption
                Option {
                    text: "Five minutes"
                }
                Option {
                    text: "Two hours"       
                }

                // Change the text in TextArea depending on which option was selected.
                onSelectedIndexChanged: {
                    if (lasagnaOption.selectedIndex == 0) {
                        nineSliceText.text = "1. Pour Mix.\n2. Add Water.\n3. Stir and heat."
                        nineSliceText_2.text = ""
                    } else if (lasagnaOption.selectedIndex == 1) {
                        nineSliceText.text = "1. Grind tomatoes.\n2. Fry minced meat.\n3. Add lasagna plates."
                        nineSliceText_2.text = "4. Grind Cheese.\n5. Season with salt.\n6. Voilà! Its ready!"
                    }
                }
            }
        }
    }
	
	attachedObjects: [
        TextStyleDefinition {
            id: textStyleLasangaPlate
            color: Color.create ("#88000000")
            lineHeight: 1.5
        }
    ]
    
    onCreationCompleted: {
        lasagnaOption.setSelectedIndex (1);
    }
}
