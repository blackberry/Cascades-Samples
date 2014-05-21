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

#ifndef _GESTUREHANDLERRECIPE_H_
#define _GESTUREHANDLERRECIPE_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/PinchEvent>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class DoubleTapEvent;
        class ImageView;
        class Label;
        class LongPressEvent;
        class PinchEvent;
        class ScaleTransition;
        class TapEvent;
    }
}

/* GestureHandlerRecipe Description:
 *
 * Sometimes in a UI special gestures are used to trigger particular actions.
 * In this recipe it is shown how to handle: tap, double-tap, pinch and long-press
 * by the use of gestureHandlers.
 */
class GestureHandlerRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the gesture handler recipe.
     * @param parent A pointer to the parent Container.
     */
    GestureHandlerRecipe(Container *parent = 0);

private slots:
    /**
     * This Slot function is for single-tap events, the gesture image is shown.
     *
     * @param tapEvent The tap event that contains the x and y position of the single-tap
     */
    void onTap(bb::cascades::TapEvent* tapEvent);

    /**
     * This Slot function is for double-tap events, the gesture image is exchanged.
     *
     * @param doubleTapEvent The tap event that contains the x and y position of the double-tap
     */
    void onDoubleTap(bb::cascades::DoubleTapEvent* doubleTapEvent);

    /**
     * This Slot function is for long-press tap events, where the gesture image is reset by running
     * an animation.
     *
     * @param longPressEvent The long-press tap event that contains
     *						 the x and y position of the long-press
     */
    void onLongPress(bb::cascades::LongPressEvent* longPressEvent);

    /**
     * This Slot function is for PinchGesture started events. If in the correct state,
     * this is where we set the mPinchInProgress variable.
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid-point, ratio and rotation
     */
    void onPinchStart(bb::cascades::PinchEvent* pinchEvent);

    /**
     * This Slot function is for PinchGesture ended events. If the pinch is in progress,
     * we set the pinch ratio. If the pinch is complete, we set the GestureState to long-press.
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid-point, ratio and rotation
     */
    void onPinchEnd(bb::cascades::PinchEvent* pinchEvent);

    /**
     * This Slot function is for in-progress pinch gestures, the gesture image is scaled and rotated and
     * the instruction label is updated. If the pinch rotation is above a certain level, we set the next
     * next user move to long-press and show the pinch should no longer be in progress.
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid point, ratio and rotation
     */
    void onPinchUpdate(bb::cascades::PinchEvent* pinchEvent);

    /**
     * Not used but we need this function in order to use build patterns
     * for the pinch handler see construction of the pinch handler.
     */
    void onPinchCancel();

    /**
     * This Slot function is for the reset animation that is run on the final
     * gesture of this example. Then we reset the UI elements to their original state.
     */
    void onLongPressOutEnded();

private:

    // UI elements
    Label *mInstructionLabel;
    Container *mGestureContainer;
    ImageView *mGestureImage;
    ScaleTransition* mLongPressOut;

    // State variables
    QString mGestureState;
    bool mPinchInProgress;
    float mPreviousPinchRatio;
};

#endif // ifndef _GESTUREHANDLERRECIPE_H_
