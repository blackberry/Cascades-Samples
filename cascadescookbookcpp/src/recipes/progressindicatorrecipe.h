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

#ifndef _PROGRESSINDICATORRECIPE_H_
#define _PROGRESSINDICATORRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ImageView;
        class ProgressIndicator;
        class SequentialAnimation;
        class ToggleButton;
    }
}

/* ProgressIndicatorRecipe Description:
 *
 * Simple ProgressIndicator recipe shows how a ProgressIndicator
 * control can be added and the different states the ProgressIndicator can have.
 *
 */
class ProgressIndicatorRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the recipe
     * @param parent The parent Container
     */
    ProgressIndicatorRecipe(Container *parent = 0);

public slots:
    /**
     * Slot function connected to the ProgressIndicator.
     *
     * @param value The current ProgressIndicator value.
     */
    void onValueChanged(float value);

    /**
     * Slot function for when the ProgressIndicator is clicked to pause/unpause
     * the indicator.
     *
     * @param on true if the toggle button is switched on otherwise false
     */
    void onToggleStove(bool on);

private slots:
    /**
     * Functino called when the cooking simulation reaches the end of a cycle,
     * the animation has run once.
     */
    void onCookingAnimEnded();

private:
    /**
     * Function that sets up the cooking simulator a pot with a tumbling lid.
     *
     * @return the Container with the simulator UI
     */
    Container *setUpCookingSimulator();

    int mCookingProgress;
    static int const mCookingTime;

    ProgressIndicator *mProgressIndicator;
    ToggleButton *mButton;
    SequentialAnimation *mCooking;
    ImageView *mLid;
};

#endif // ifndef _PROGRESSINDICATORRECIPE_H_
