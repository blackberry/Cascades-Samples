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
#include "pixelbufferrecipe.h"

#include <bb/cascades/ImageView>
#include <bb/ImageData>
#include <bb/PixelFormat>

using namespace bb;

void PixelBufferRecipe::fillBuffer(uint width, uint height, unsigned char* buf)
{

    // Let's generate pretty checkers

    qsrand((uint) QTime::currentTime().msec());
    uint t = qrand();

    for (uint y = 0; y < height; ++y) {
        for (uint x = 0; x < width; ++x) {
            uint rand = x ^ (y + t);
            buf[0] = rand;
            buf[1] = (rand >> 8);
            buf[2] = (rand >> 16);
            buf += 4;
        }
    }
}

PixelBufferRecipe::PixelBufferRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // Width and height of our image
    static const uint width = ui->du(65);
    static const uint height = ui->du(50);

    // The data we fill our pixel buffer with
    unsigned char *buf = (unsigned char*) malloc(width * height * 4);

    // Call our fillbuffer function.
    fillBuffer(width, height, buf);

    // Create an image with the PixelBufferData.
    Image img(ImageData::fromPixels(buf, PixelFormat::RGBX, width, height, width * 4));

    // Add the image to an ImageView.
    ImageView *imageView = ImageView::create().image(img);

    setRoot(imageView);

    //we don't need the buffer anymore, so we free it from memory
    free(buf);
}

