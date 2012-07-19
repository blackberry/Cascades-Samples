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

// This recipe shows how a nine sliced image works. The nice thing
// with images that can be nine sliced is that the size can be altered
// without scaling the entire image. A file accompanying the image
// contains margins, which will be unaffected by scaling. If set up
// properly the image can scale without getting scaling artifacts and
// distortions around the edges.

RecipePage {
    RecipeContainer {

        // This is the where the nine sliced image is put.
        Container {
            layout: DockLayout {
                topPadding: 35
                leftPadding: 100
                rightPadding: leftPadding
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }

            // This is the image that has nine slice margins applied to it,
            // thus scaling of this image will not cause scaling artifacts around
            // the image, but rather an area in the middle is scaled. The margins
            // that are applied can be found in the file border_image_Pasta.amd.
            ImageView {
                imageSource: "asset:///images/nineslice/border_image_Pasta"
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                }
            }

            // The length of the text in this TextArea will effect its height,
            // which will in turn alter the height of the surrounding Container.
            // Since the ImageView above is set to fill the entire Container the
            // scaling of a nine sliced image is illustrated by switching between
            // on short (lazy) and long (diligent) recipe text (see RadioGroup below).
            Container {
                layout: StackLayout {
                    topPadding: 40
                    bottomPadding: 110
                    leftPadding: 30
                    rightPadding: leftPadding
                }

                TextArea {
                    id: nineSliceText
                    editable: false

                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.create("#88000000")
                        lineSpacing: 1.4
                    }
                }
            }
        }

        // To show how a nine sliced image is scaled we provide the possibility to
        // alter the text between two different lengths. This will cause the text
        // area to change size and consequently the image will be changing size.
        Container {
            layout: StackLayout {
                rightPadding: 20
                leftPadding: rightPadding
            }

            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Bottom
            }

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
                    } else if (lasagnaOption.selectedIndex == 1) {
                        nineSliceText.text = "1. Grind tomatoes.\n2. Fry minced meat.\n3. Add lasagna plates.\n4. Grind Cheese.\n5. Season with salt."
                    }
                }
            }
        }
    }

    onCreationCompleted: {
        // When the creation of the recipe is complete the second option is selected.
        lasagnaOption.setSelectedIndex(1);
    }
}
