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

/* GestureHandlerRecipe
 * 
 * Sometimes in a UI special gestures are used to trigger particular actions.
 * In this recipe it is shown how to handle: tap, double-tap, pinch and long-press
 * by the use of gestureHandlers.
 */
class GestureHandlerRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    GestureHandlerRecipe(Container *parent = 0);

private slots:
    /**
     * Slot for tap events, this is where the gesture image is shown.
     *
     * @param tapEvent Contains the x and y position of the tap
     */
    void onTap(bb::cascades::TapEvent* tapEvent);

    /**
     * Slot for tap events, this is where the gesture image is exchanged.
     *
     * @param doubleTapEvent Contains the x and y position of the double-tap
     */
    void onDoubleTap(bb::cascades::DoubleTapEvent* doubleTapEvent);

    /**
     * Slot for tap events, this is where the gesture image is reset by running
     * an animation.
     *
     * @param longPressEvent Contains the x and y position of the long-press
     */
    void onLongPress(bb::cascades::LongPressEvent* longPressEvent);

    /**
     * Slot for pinch gesture started events, if in the correct state
     * this is where we set the mPinchInProgress variable.
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid point, ration and rotation
     */
    void onPinchStart(bb::cascades::PinchEvent* pinchEvent);

    /**
     * Slot for pinch gesture ended events, if the user has pinched
     * above a certain threshold this is where we continue to let him
     * try out the long press
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid point, ration and rotation
     */
    void onPinchEnd(bb::cascades::PinchEvent* pinchEvent);

    /**
     * Slot for pinching, the gesture image is scaled and rotated and
     * the instruction label is updated.
     *
     * @param pinchEvent Event that carries information on the pinch
     *                   like pinch mid point, ration and rotation
     */
    void onPinchUpdate(bb::cascades::PinchEvent* pinchEvent);

    /**
     * Not used but we need this function in order to use build patterns
     * for the pinch handler see construction of the pinch handler.
     */
    void onPinchCancel();

    /**
     * Animation slot for the reset animation that is run on the final
     * gesture of this example, resets the UI elements to their original state.
     */
    void onLongPressOutEnded();

private:

    // UI elements.
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
