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

// ImagePaint is typically used for tileable images. A tileable
// image is one that can be repeated as a pattern over a surface.
// In this recipe, ImagePaint use is illustrated by showing how a Container
// background can be filled with tiled image patterns.

RecipePage {
    
    // Javascript function to set random image source for the ImagePaintDefinition.
    function randomImageSource() {
        var num = Math.floor((Math.random() * 7) + 1);
        var file = "";
        var text = "";

        switch (num) {
            case 1:
                file = "asset:///images/imagepaint/gplay_256x256.amd";
                imageSource.text = "Nistri"
                break;
            case 2:
                file = "asset:///images/imagepaint/pyamas_16x16.amd";
                imageSource.text = "Pyamas";
                break;
            case 3:
                file = "asset:///images/imagepaint/tactile_stripes_16x16.amd";
                imageSource.text = "Tactile Stripes";
                break;
            case 4:
                file = "asset:///images/imagepaint/white_stripes_16x16.amd";
                imageSource.text = "White Stripes";
                break;
            case 5:
                file = "asset:///images/imagepaint/scribble_light_256x256.amd";
                imageSource.text = "Scribble Light";
                break;
            case 6:
                file = "asset:///images/imagepaint/light_toast_128x128.amd";
                imageSource.text = "Light Toast";
                break;
            case 7:
                file = "asset:///images/imagepaint/gplay_256x256.amd";
                imageSource.text = "Tile Gplay";
                break;
        }
        return file;
    }

    Container {

        background: recipeBackground.imagePaint
        attachedObjects: [
            // The ImagePaint defines which image to paint with and which
            // repeat pattern that should be used (both X and Y direction in this
            // case). The provided image needs to have a width and height that
            // are each a power of two, for optimization purposes. Furthermore, the image
            // has to be accompanied by a meta file (with file ending .amd)
            // with the "repeatable" property set to true. Please note that the
            // .png ending has been dropped on the image in order for the matching
            // images/imagepaint/Tile_nistri_16x16.amd file to be found.
            ImagePaintDefinition {
                id: recipeBackground
                imageSource: randomImageSource()
                repeatPattern: RepeatPattern.XY
            }
        ]

        Container {
            topPadding: ui.du(5)
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Image Paint Definition Source:"
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                }
            }
            
            Label {
                id: imageSource
                horizontalAlignment: HorizontalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.PrimaryText
                }
            }
        }
    }
}

