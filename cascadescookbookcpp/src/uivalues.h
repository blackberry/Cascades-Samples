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

#ifndef UIVALUES_HPP_
#define UIVALUES_HPP_

#include <QObject>
#include <QSize>
#include <QVariantList>
#include <bb/cascades/Application>
#include <bb/device/HardwareInfo>
#include <bb/device/DisplayInfo>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class TextStyle;
    }
}

/**
 * UiValues Description:
 *
 * A singleton class used to store pixel values. Used to get different values
 * depending on device resolution. Resolution independence of image assets are
 * handled using static asset selectors (see e.g. folders assets/images and
 * assets/720x720/images).
 */
class UiValues: public QObject
{
    Q_OBJECT

public:

    enum DeviceType
    {
        DEVICETYPE_768X1280,
        DEVICETYPE_720X1280,
        DEVICETYPE_720X720
    };

    enum Value
    {
        // The App cover header height
        UI_APPCOVERHEADER_HEIGHT,

        // Specific values for the nine-slice recipe
        UI_NINESLICERECIPE_LASANGATEXTSTYLE,

        NBR_OF_VALUES
    };

    /**
     * Returns the single application UiValues instance.
     *
     * @return An instance of the Global context object
     */
    static UiValues* instance();

    /**
     * Function returns the device size as a QSize value object
     * with width and height in pixel dimensions.
     *
     * @param value The enum for accessing the value
     * @return Device size
     */
    QSize size();

    /**
     * Access a value by calling this function or if the
     * value is know to be of integer type call intValue.
     *
     * @return A QVariant with the value data
     */
    QVariant value(UiValues::Value value);

    /**
     * The value is converted to integer type, if the value stored
     * at  UiValues::Value value is not an integer 0 is returned.
     *
     * @param value The enum for accessing the value
     * @return The integer value or 0 if not an integer
     */
    int intValue(UiValues::Value value);

    /**
     * The value is converted to TextStyle, if the value stored
     * at UiValues::Value value is not a TextStyle 0 is returned.
     *
     * @param value The enum for accessing the value
     * @return The TextStyle value or 0 if not an integer
     */
    TextStyle textStyleValue(UiValues::Value value);

    /**
     * The device type on which the application is currently running.
     *
     * @return An enum reflecting which device the application is running on.
     */
    UiValues::DeviceType device();

    /**
     * Returns whether or not a physical keyboard is present on the device. A wrapper
     * function around the HardwareInfo class;
     *
     * @return  Returns true if the device has a physical keyboard, and false otherwise.
     */
    bool isPhysicalKeyboardDevice();

    /**
     * Provides the display's aspect with respect to the display's natural orientation.
     * see DisplayInfo aspectType() function for details.
     *
     * @return Return: Returns the display's aspect. If this object is invalid, then the return value is undefined.
     */
    bb::device::DisplayAspectType::Type aspectType();

private:

    void initValues();

    QSize mApplicationSize;
    QVariantList mValues;
    UiValues::DeviceType mDevice;
    bb::device::HardwareInfo *mHardwareInfo;
    bb::device::DisplayInfo *mDisplayInfo;
};

#endif /* UIVALUES_HPP_ */
