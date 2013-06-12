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

#include "uivalues.h"
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

// Static global instance, there will be only one for the application.
static UiValues* mInstance = NULL;

UiValues* UiValues::instance()
{
    if (mInstance == NULL) {
        // If an instance of the object does not exist create one.
        mInstance = new UiValues();
        mInstance->initValues();
    }

    // Return the instance.
    return mInstance;
}

QVariant UiValues::value(UiValues::Value value)
{
    if (value < mValues.size()) {
        return mValues[value];
    }
    return 0;
}

UiValues::DeviceType UiValues::device()
{
    return mDevice;
}

TextStyle UiValues::textStyleValue(UiValues::Value value)
{
    if (value < mValues.size()) {
        QVariant variantValue = mValues[value];
        if (variantValue.canConvert<TextStyle>()) {
            return variantValue.value<TextStyle>();
        }
    }
    return SystemDefaults::TextStyles::bodyText();
}

int UiValues::intValue(UiValues::Value value)
{
    if (value < mValues.size()) {
        QVariant variantValue = mValues[value];
        if (variantValue.canConvert(QVariant::Int)) {
            return variantValue.toInt();
        }
    }
    return 0;
}

void UiValues::initValues()
{
    QString envWidth(getenv("WIDTH"));
    QString envHeight(getenv("HEIGHT"));

    mApplicationSize.setWidth(envWidth.toInt());
    mApplicationSize.setHeight(envHeight.toInt());

    mValues.insert(SCREEN_WIDTH, QVariant(mApplicationSize.width()));
    mValues.insert(SCREEN_HEIGHT, QVariant(mApplicationSize.height()));

    if (mApplicationSize.width() == 720 && mApplicationSize.height() == 720) {
        mDevice = UiValues::DEVICETYPE_720X720;
    } else {
        mDevice = UiValues::DEVICETYPE_768X1280;
    }

    // Set up padding.
    if (mDevice == UiValues::DEVICETYPE_768X1280) {
        // Common UI values
        mValues.insert(UI_PADDING_STANDARD, QVariant(20));
        mValues.insert(UI_PADDING_TINY, QVariant(4));
        mValues.insert(UI_PADDING_SMALL, QVariant(35));
        mValues.insert(UI_PADDING_MEDIUM, QVariant(80));
        mValues.insert(UI_PADDING_LARGE, QVariant(100));
    } else {
        mValues.insert(UI_PADDING_STANDARD, QVariant(20));
        mValues.insert(UI_PADDING_TINY, QVariant(4));
        mValues.insert(UI_PADDING_SMALL, QVariant(25));
        mValues.insert(UI_PADDING_MEDIUM, QVariant(30));
        mValues.insert(UI_PADDING_LARGE, QVariant(50));
    }

    // Set up the inline activity indicator values.
    if (mDevice == UiValues::DEVICETYPE_768X1280) {
        mValues.insert(UI_INLINEACTIVITYINDICATOR_HEIGHT, QVariant(140));
        mValues.insert(UI_INLINEACTIVITYINDICATOR_PADDING, QVariant(20));
    } else {
        mValues.insert(UI_INLINEACTIVITYINDICATOR_HEIGHT, QVariant(130));
        mValues.insert(UI_INLINEACTIVITYINDICATOR_PADDING, QVariant(10));
    }

    // Set up the stockcurve recipe values.
    if (mDevice == UiValues::DEVICETYPE_768X1280) {
        // Stock curve recipe values
        mValues.insert(UI_STOCKCURVERECIPE_EGGDISTANCE, QVariant(780));
        mValues.insert(UI_STOCKCURVERECIPE_EGGTIME, QVariant(1500));
    } else {
        // Stock curve recipe values
        mValues.insert(UI_STOCKCURVERECIPE_EGGDISTANCE, QVariant(325));
        mValues.insert(UI_STOCKCURVERECIPE_EGGTIME, QVariant(750));
    }

    // Set up the nine-slice recip values.
    if (mDevice == UiValues::DEVICETYPE_768X1280) {
        // Nine-slice recipe values
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADTOP, QVariant(40));
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADBOTTOM, QVariant(110));
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADSIDE, QVariant(40));
        QVariant lasangaStyle;
        lasangaStyle.setValue(SystemDefaults::TextStyles::titleText());
        mValues.insert(UI_NINESLICERECIPE_LASANGATEXTSTYLE, lasangaStyle);
    } else {
        // Nine-slice recipe values
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADTOP, QVariant(20));
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADBOTTOM, QVariant(60));
        mValues.insert(UI_NINESLICERECIPE_LASANGAPADSIDE, QVariant(40));
        QVariant lasangaStyle;
        lasangaStyle.setValue(SystemDefaults::TextStyles::subtitleText());
        mValues.insert(UI_NINESLICERECIPE_LASANGATEXTSTYLE, lasangaStyle);
    }

    // Set up the pixel buffer recipe values.
    if (mDevice == UiValues::DEVICETYPE_768X1280) {
        // Pixel buffer recipe values
        mValues.insert(UI_PIXELBUFFERRECIPE_PIXELWIDTH, QVariant(640));
        mValues.insert(UI_PIXELBUFFERRECIPE_PIXELHEIGHT, QVariant(880));
    } else {
        // Pixel buffer recipe values
        mValues.insert(UI_PIXELBUFFERRECIPE_PIXELWIDTH, QVariant(600));
        mValues.insert(UI_PIXELBUFFERRECIPE_PIXELHEIGHT, QVariant(450));
    }
}

QSize UiValues::size()
{
    return mApplicationSize;
}

