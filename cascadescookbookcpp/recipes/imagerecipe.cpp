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
#include "imagerecipe.h"

#include <bb/cascades/ImageView>

using namespace bb::cascades;

ImageRecipe::ImageRecipe(Container *parent) :
        CustomControl(parent)
{
    // Create the ImageView and set its size, if the preferred Width/Height is not
    // set the image will be layouted later and the size will be animated.
    ImageView *image = ImageView::create("asset:///images/peach_big.png");
    image->setPreferredSize(710, 590);

    setRoot(image);
}

