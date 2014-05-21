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

#ifndef _SPEEDGAUGE_H_
#define _SPEEDGAUGE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ImageView;
    }
}

/**
 * SpeedGauge Description:
 *
 * A custom control that displays the writing speed based on the number of
 * characters entered since the user started typing.
 */
class SpeedGauge: public bb::cascades::CustomControl
{
    Q_OBJECT

    /**
     * A property that reflects the average speed of entering text.
     */
    Q_PROPERTY(float averageSpeed READ averageSpeed NOTIFY averageSpeedChanged FINAL)

public:
    SpeedGauge(Container *parent = 0);

    /**
     * This function calculates and updates the speed based on the number of
     * characters entered since the user started typing.
     *
     * @param nbrOfChars Number of characters entered
     */
    Q_SLOT void calculateSpeed(int nbrOfChars = 0);

    /**
     * Stops measuring altogether timer based updates stops.
     */
    Q_SLOT void stop();

    /**
     * This function returns the current average speed of entering text.
     *
     * @return An integer values of the current speed in words/min (each word being 6 characters long).
     */
    int averageSpeed();

signals:
    void averageSpeedChanged(float averageSpeed);

private:
    /**
     * This function returns current time in milliseconds (Ms).
     *
     * @return current time in Ms.
     */
    long getTimeMs();

    /**
     * This function sets up the needle image of the dial.
     *
     * @param pImageUrl File path of the needle image
     * @return An ImageView of the set up needle
     */
    ImageView* setUpNeedle(const QString pImageUrl);

    /**
     * This function is called by a timer mSpeedUpdateTimer and will cause the
     * typing speed to gradually degrade when the user is either entering the
     * wrong text or not entering any text at all.
     */
    Q_SLOT void calculateSpeedTimeOut();

    /**
     * Layout signal handler for the image, used to find the size of the images,
     * so that the correct pivot points for rotation can be set up.
     *
     * @param layoutRect The QRectF containing layout information.
     */
    Q_SLOT void needleLayoutFrameUpdated(QRectF layoutRect);

    // State variables
    long mStartTime;
    int mNbrOfChars;
    float mMaxSpeedAngle;
    float mAverageSpeed;

    // SpeedGauge controls
    ImageView *mSpeedNeedle;
    ImageView *mMaxNeedle;

    QTimer *mSpeedUpdateTimer;

};

#endif // ifndef _SPEEDGAUGE_H_
