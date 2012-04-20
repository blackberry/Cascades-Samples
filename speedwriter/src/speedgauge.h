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


#ifndef _SPEEDGAUGE_H_
#define _SPEEDGAUGE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb {
    namespace cascades {
        class ImageView;
    }
}

/**
 * SpeedGauge
 * 
 * A custom control that display the writing speed based on the number of
 * characters entered since the measuring started.
 */
class SpeedGauge: public bb::cascades::CustomControl
{

public:
    SpeedGauge();

    /**
     * Calculates and updates the speed based on the number of
     * characters entered. 
     *
     * @param nbrOfChars Number of characters entered
     */
    void calculateSpeed(int nbrOfChars);

    /**
     * This function stops the continuous update of the writing speed.
     *
     * @return The average speed at the time stop is called.
     */
    float stopMeasuring();

private:
    /**
     * The current time in Ms can be accessed by calling this function.
     *
     * @return current time in Ms.
     */
    long getTimeMs();

    ImageView* setUpNeedle(float width, float height, const QString pImageUrl);

    // State variables.
    long mStartTime;
    int mNbrOfChars;
    float mMaxSpeedAngle;

    // SpeedGauge controls.
    ImageView *mSpeedNeedle;
    ImageView *mMaxNeedle;

};

#endif // ifndef _SPEEDGAUGE_H_
