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

/* AnimationRecipe Description:
 *
 * This recipe shows how to create your own animations and transitions
 * on visual items.
 *
 */
class AnimationRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /*
     * Constructor; sets up the animation recipe.
     * @param parent A parent Container pointer, @c 0 is used.
     */
    AnimationRecipe(Container *parent = 0);

private slots:

    /**
     * We listen for the ToggleButton signal to start animations
     * and when the animation finishes (when out of picture), reset the size.
     *
     * @property on Boolean that will be true if the toggle is on, false otherwise.
     */
    void onToggleChanged(bool on);

    /**
     * Animation signal handler for when the hide animation completes.
     */
    void onHideAnimEnded();

    /**
     * Animation signal handler for when the show animation completes.
     */
    void onShowAnimStarted();

    /**
     * Layout signal handler for the two first eggs used
     * to set up the pivot points for scaling.
     *
     * @param layoutRect The QRectF containing layout information.
     */
    void eggLayoutFrameUpdated(QRectF layoutRect);

private:

    /**
     * Sets up the animations for the third off-screen egg.
     *
     * @param animatedEgg Pointer to the Container for the third off-screen egg.
     */
    void setUpAnimations(Container *animatedEgg);

    /**
     * Sets up the top Container containing the eggs that will be animated.
     *
     * @return A Container setup with the animated eggs.
     */
    Container *setUpAnimationContainer();

    /**
     * Sets up the bottom Container with the controller for triggering animations.
     *
     * @return A Container setup with the controller (ToggleButton with Label).
     */
    Container *setUpControllerContainer();

    /**
     * Sets up a Container with an egg (Egg image and egg shadow with correct pivot point).
     *
     * @return A Container with the egg.
     */
    Container *setUpAnimationEgg();


    // The eggs that will be scaled by implicit animation
    Container *mFirstEgg;
    Container *mSecondEgg;
    Container *mThirdEgg;

    // Animations
    TranslateTransition *mShowMoreEgg;
    TranslateTransition *mHideMoreEgg;
    SequentialAnimation *mTiltEgg;
    SequentialAnimation *mTiltShadow;
};

#endif // ifndef _ANIMATIONRECIPE_H_
