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
#include "imagepaintrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Option>
#include <bb/cascades/Paint>

using namespace bb::cascades;

ImagePaintRecipe::ImagePaintRecipe(Container *parent) :
        CustomControl(parent)
{
    mRecipeContainer = Container::create().top(50.0f).left(50.0f).right(50.0f);
    mRecipeContainer->setLayout(DockLayout::create());
    mRecipeContainer->setPreferredSize(768, 1280);

    // ImagePaint defines which image to paint with and which
    // repeat pattern that should be used (both X and Y direction in this
    // case). The provided image needs to have a width and height that
    // is a power of two, for optimization purposes. Furthermore, the image
    // has to be accompanied by a meta file (with file ending .amd)
    // with the "repeatable" property set to true. Please note that the
    // .png format extension has been dropped on the file name in order for the matching
    // images/imagepaint/Tile_nistri_16x16.amd file to be found.
    ImagePaint paint(QUrl("asset:///images/imagepaint/Tile_nistri_16x16.amd"), RepeatPattern::XY);
    mRecipeContainer->setBackground(paint);

    // A drop down control with a couple of different tile images, all of
    // various power of two dimensions.
    DropDown *dropDown = new DropDown();
    dropDown->setTitle("Select tile");

    // Set up Options and add it to the DropDown.
    dropDown->add(
            Option::create().text("Nistri").image(
                    QUrl("asset:///images/imagepaint/Tile_nistri_16x16.amd")));
    dropDown->add(
            Option::create().text("Pyamas").image(
                    QUrl("asset:///images/imagepaint/Tile_pyamas_16x16.amd")));
    dropDown->add(
            Option::create().text("Tactile").image(
                    QUrl("asset:///images/imagepaint/Tile_tactile_stripes_16x16.amd")));
    dropDown->add(
            Option::create().text("White Stripes").image(
                    QUrl("asset:///images/imagepaint/Tile_white_stripes_16x16.amd")));
    dropDown->add(
            Option::create().text("Scribble Light").image(
                    QUrl("asset:///images/imagepaint/Tile_scribble_light_256x256.amd")));
    dropDown->add(
            Option::create().text("Light Toast").image(
                    QUrl("asset:///images/imagepaint/Tile_light_toast_128x128.amd")));
    dropDown->add(
            Option::create().text("Tile Gplay").image(
                    QUrl("asset:///images/imagepaint/Tile_gplay_256x256.amd")));

    // Connect to the signal for index changes, so that we can update the recipe when a new selection is made.
    connect(dropDown, SIGNAL(selectedIndexChanged(int)), this,
            SLOT(onSelectedIndexChanged(int)));

    mRecipeContainer->add(dropDown);
    setRoot(mRecipeContainer);
}

void ImagePaintRecipe::onSelectedIndexChanged(int selectedIndex)
{
    DropDown* dropDown = dynamic_cast<DropDown*>(sender());

    if (dropDown) {
        // Change the tiling image for the paint to the one specified by the imageSource property.
        Option* selectedOption = dropDown->at(selectedIndex);
        ImagePaint paint(QUrl(selectedOption->imageSource()), RepeatPattern::XY);
        mRecipeContainer->setBackground(paint);
    }
}

