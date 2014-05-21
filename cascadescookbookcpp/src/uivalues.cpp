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

bool UiValues::isPhysicalKeyboardDevice()
{
    return mHardwareInfo->isPhysicalKeyboardDevice();
}

bb::device::DisplayAspectType::Type UiValues::aspectType()
{
    return mDisplayInfo->aspectType();
}

void UiValues::initValues()
{
    QString envWidth(getenv("WIDTH"));
    QString envHeight(getenv("HEIGHT"));

    mApplicationSize.setWidth(envWidth.toInt());
    mApplicationSize.setHeight(envHeight.toInt());

    if (mApplicationSize.width() == 720 && mApplicationSize.height() == 720) {
        mDevice = UiValues::DEVICETYPE_720X720;
    } else if(mApplicationSize.width() == 720 && mApplicationSize.height() == 1280){
        mDevice = UiValues::DEVICETYPE_720X1280;
    } else {
        mDevice = UiValues::DEVICETYPE_768X1280;
    }

    // The header height differs on different devices see
    // https://developer.blackberry.com/devzone/design/bb10/active_frames.html
    if (mDevice == UiValues::DEVICETYPE_768X1280){
    	mValues.insert(UI_APPCOVERHEADER_HEIGHT, QVariant(62));
    } else if(mDevice == UiValues::DEVICETYPE_720X1280) {
    	mValues.insert(UI_APPCOVERHEADER_HEIGHT, QVariant(52));
    } else {
    	mValues.insert(UI_APPCOVERHEADER_HEIGHT, QVariant(56));
    }

    // Set up the nine-slice recipe values.
    if (mDevice == UiValues::DEVICETYPE_720X720) {
        // Nine-slice recipe values
        QVariant lasangaStyle;
        lasangaStyle.setValue(SystemDefaults::TextStyles::subtitleText());
        mValues.insert(UI_NINESLICERECIPE_LASANGATEXTSTYLE, lasangaStyle);
    } else {
        // Nine-slice recipe values
        QVariant lasangaStyle;
        lasangaStyle.setValue(SystemDefaults::TextStyles::titleText());
        mValues.insert(UI_NINESLICERECIPE_LASANGATEXTSTYLE, lasangaStyle);
    }

    // Hardware information object, used to determine if device has physical keyboard.
    mHardwareInfo = new bb::device::HardwareInfo(this);

    // Display info object used to determine the aspect ratio of the screen.
    mDisplayInfo = new bb::device::DisplayInfo(this);
}

QSize UiValues::size()
{
    return mApplicationSize;
}

