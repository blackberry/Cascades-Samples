/* Copyright (c) 2013 Research In Motion Limited.
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
#ifndef SCREENSIZE_H_
#define SCREENSIZE_H_

#include <QObject>
#include <bb/device/DisplayInfo>
#include <bb/cascades/DisplayDirection>

/**
 * ScreenSize Description:
 *
 * A ScreenSize object for Cascades.
 *
 * Just include the following in your c++ code when setting up your qml root...
 *
 * #include "ScreenSize.hpp"
 * ...
 *
 * ScreenSize* screenSize = new ScreenSize(this);
 * qml->setContextProperty("ScreenSize", screenSize);
 *
 * You will also want to link against -lbbdevice
 *
 */
class ScreenSize: public QObject
{
    Q_OBJECT

    /**
     * The current width of the application screen, might change if the orientation of the application
     * changes from for example portrait to landscape.
     */
    Q_PROPERTY(int width READ width NOTIFY widthChanged FINAL);

    /**
     * The current height of the application screen, might change if the orientation of the application
     * changes from for example portrait to landscape.
     */
    Q_PROPERTY(int height READ height NOTIFY heightChanged FINAL);

    /**
     * The size of the application screen, might change if the orientation of the application
     * changes from for example portrait to landscape.
     */
    Q_PROPERTY(QSize res READ res NOTIFY resChanged FINAL);

    /**
     *  The display direction of the device.
     */
    Q_PROPERTY(bb::cascades::DisplayDirection::Type displayDirection READ displayDirection NOTIFY displayDirectionChanged FINAL);

public:
    ScreenSize(QObject* parent = NULL);
    ScreenSize(int displayId, QObject* parent = NULL);
    ~ScreenSize();

    /**
     * Returns the width of the Application screen.
     *
     * @return The width
     */
    int width() const
    {
        return mRes.width();
    }

    /**
     * Returns the height of the Application screen.
     *
     * @return The height
     */
    int height() const
    {
        return mRes.height();
    }

    /**
     * Returns the size of the Application screen.
     *
     * @return The size
     */
    QSize res() const
    {
        return mRes;
    }

    /**
     *  Returns the display direction of the application.
     *
     *  @return The display direction.
     */
    bb::cascades::DisplayDirection::Type displayDirection() const
    {
        return mDisplayDirection;
    }

    Q_SIGNALS:

    /**
     * Signal emitted when the width of the application screen changes.
     */
    void widthChanged(int width);

    /**
     * Signal emitted when the height of the application screen changes.
     */
    void heightChanged(int height);

    /**
     * Signal emitted when the size of the application screen changes.
     */
    void resChanged(QSize res);

    /**
     * Signal emitted when the display direction of the device changes.
     */
    void displayDirectionChanged(bb::cascades::DisplayDirection::Type direction);

private Q_SLOTS:

    /**
     * Function used to subscribe to size changes of the application screen.
     *
     * @param pixelSize The application screen dimensions in pixels
     */
    void onSizeChanged(QSize pixelSize);

    /**
     * Function used to subscribe to orientation changes of the device.
     *
     * @param direction The new device orientation.
     */
    void onDisplayDirectionChanged(bb::cascades::DisplayDirection::Type direction);

private:
    // Internal helper functions.
    void init();
    void update(QSize res, bb::cascades::DisplayDirection::Type direction);

    // Variables
    bb::cascades::DisplayDirection::Type mDisplayDirection;
    QSize mRes;
    QSize mDisplayRes;
    bb::device::DisplayInfo mDisplayInfo;
};

#endif /* SCREENSIZE_H_ */
