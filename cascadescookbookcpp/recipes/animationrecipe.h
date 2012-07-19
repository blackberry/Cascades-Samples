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


#ifndef _ANIMATIONRECIPE_H_
#define _ANIMATIONRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class TranslateTransition;
        class SequentialAnimation;
    }
}

/* AnimationRecipe
 *
 * This recipe shows how to create your own animations and transitions
 * on visual items.
 *
 */
class AnimationRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    AnimationRecipe(Container *parent = 0);

private slots:

    /**
     * We listen to the ToggleButton change to start animations
     * and the animation ended to reset the size (when out of picture).
     *
     * @property on Boolean that will be true if the toggle is on.
     */
    void onToggleChanged(bool on);

    /**
     * Animation signal handler for when the hide animation that will be
     * called when the animation has ended.
     */
    void onHideAnimEnded();

    /**
     * Animation signal handler for when the show animation that will be
     * called when the animation has started.
     */
    void onShowAnimStarted();

private:

    /**
     * Sets up the animations for the third off screen egg.
     */
    void setUpAnimations(Container *animatedEgg);

    /**
     * Sets up the top Container containing the eggs that will be animated.
     */
    Container *setUpAnimationContainer();

    /**
     * Sets up the bottom Container with the controller for triggering animations.
     */
    Container *setUpControllerContainer();

    /**
     * Sets up a Container with an egg (Egg image and egg shadow with correct pivot point).
     */
    Container *setUpAnimationEgg();

    // The Container with the two original super sized eggs.
    Container *mSuperEggs;

    // Animations.
    TranslateTransition *mShowMoreEgg;
    TranslateTransition *mHideMoreEgg;
    SequentialAnimation *mTiltEgg;
    SequentialAnimation *mTiltShadow;
};

#endif // ifndef _ANIMATIONRECIPE_H_
