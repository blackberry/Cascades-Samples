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

#ifndef UIVALUES_HPP_
#define UIVALUES_HPP_

#include <QObject>
#include <QSize>
#include <QVariantList>
#include <bb/cascades/Application>

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

    enum DeviceType {
        DEVICETYPE_768X1280,
        DEVICETYPE_720X720
    };

    enum Value {
        SCREEN_WIDTH,
        SCREEN_HEIGHT,

        // Common values
        UI_PADDING_STANDARD,
        UI_PADDING_TINY,
        UI_PADDING_SMALL,
        UI_PADDING_MEDIUM,
        UI_PADDING_LARGE,

        // Specific values for the InlineActivityIndicator CustomControl
        UI_INLINEACTIVITYINDICATOR_HEIGHT,
        UI_INLINEACTIVITYINDICATOR_PADDING,

        // Specific values for the StockCurve recipe
        UI_STOCKCURVERECIPE_EGGDISTANCE,
        UI_STOCKCURVERECIPE_EGGTIME,

        // Specific values for the nine-slice recipe
        UI_NINESLICERECIPE_LASANGAPADTOP,
        UI_NINESLICERECIPE_LASANGAPADBOTTOM,
        UI_NINESLICERECIPE_LASANGAPADSIDE,
        UI_NINESLICERECIPE_LASANGATEXTSTYLE,

        // Specific values for pixel buffer recipe
        UI_PIXELBUFFERRECIPE_PIXELWIDTH,
        UI_PIXELBUFFERRECIPE_PIXELHEIGHT,

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

private:

    void initValues();

    QSize mApplicationSize;
    QVariantList mValues;
    UiValues::DeviceType mDevice;

};

#endif /* UIVALUES_HPP_ */
