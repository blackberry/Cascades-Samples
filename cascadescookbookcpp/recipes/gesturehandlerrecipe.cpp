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
#include "gesturehandlerrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DoubleTapHandler>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/LongPressHandler>
#include <bb/cascades/PinchHandler>
#include <bb/cascades/ScaleTransition>
#include <bb/cascades/StockCurve>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TapHandler>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

GestureHandlerRecipe::GestureHandlerRecipe(Container *parent) :
        CustomControl(parent)
{
    // The recipe Container
    Container *recipeContainer = new Container();
    DockLayout *recipeLayout = new DockLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredSize(768, 1280);

    // Title Label used for giving instructions
    mInstructionLabel = new Label();
    mInstructionLabel->setText((const QString) "Tap to start");
    mInstructionLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mInstructionLabel->textStyle()->setFontWeight(FontWeight::W200);
    mInstructionLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mInstructionLabel->setVerticalAlignment(VerticalAlignment::Top);

    // This is the gestureContainer containing an image that will be altered as
    // different gestures are performed.
    mGestureContainer = new Container();
    mGestureContainer->setOpacity(0);
    mGestureContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    mGestureContainer->setVerticalAlignment(VerticalAlignment::Center);

    // A reset animation for restarting the gesture cycle
    mLongPressOut =
            ScaleTransition::create(mGestureContainer).toX(0.0).toY(0.0).duration(400).easingCurve(
                    StockCurve::Linear).parent(this);

    connect(mLongPressOut, SIGNAL(ended()), this, SLOT(onLongPressOutEnded()));

    mGestureImage = ImageView::create("asset:///images/gesturehandler/whole.png");
    mGestureContainer->add(mGestureImage);

    // Add the controls to the recipe Container and set it as root.
    recipeContainer->add(mInstructionLabel);
    recipeContainer->add(mGestureContainer);

    // All gestures in this recipe are handled by these gesture handlers. The gesture
    // handlers belong to the UI element they are in, in this case the RecipeContainer.
    // So events occurring inside that element can be captured by them.
    TapHandler *tapHandler = TapHandler::create().onTapped(this,
            SLOT(onTap(bb::cascades::TapEvent*)));

    DoubleTapHandler *doubleTapHandler = DoubleTapHandler::create().onDoubleTapped(this,
            SLOT(onDoubleTap(bb::cascades::DoubleTapEvent*)));

    LongPressHandler *longPressHandler = LongPressHandler::create().onLongPressed(this,
            SLOT(onLongPress(bb::cascades::LongPressEvent*)));

    PinchHandler *pinchHandler = PinchHandler::create().onPinch(this,
            SLOT(onPinchStart(bb::cascades::PinchEvent*)),
            SLOT(onPinchUpdate(bb::cascades::PinchEvent*)),
            SLOT(onPinchEnd(bb::cascades::PinchEvent*)), SLOT(onPinchCancel()));

    recipeContainer->addGestureHandler(tapHandler);
    recipeContainer->addGestureHandler(doubleTapHandler);
    recipeContainer->addGestureHandler(longPressHandler);
    recipeContainer->addGestureHandler(pinchHandler);

    // Initiate the state variables, in this recipe one gesture at the time is handled
    // the mGestureState variable is used to achieve this.
    mGestureState = "tap";
    mPinchInProgress = false;

    setRoot(recipeContainer);
}

void GestureHandlerRecipe::onTap(bb::cascades::TapEvent* tapEvent)
{
    Q_UNUSED(tapEvent)

    // The gesture state is a local property used to try one gesture at the time, the first one is "tap".
    if (mGestureState.compare("tap") == 0) {

        // Tap was made and double tap is the next gesture to try out.
        mGestureContainer->setOpacity(1);
        mInstructionLabel->setText("Excellent! Try to double-tap");
        mGestureState = "doubletap";
    }
}

void GestureHandlerRecipe::onDoubleTap(bb::cascades::DoubleTapEvent* doubleTapEvent)
{
    Q_UNUSED(doubleTapEvent)

    if (mGestureState.compare("doubletap") == 0) {

        // Double tap was performed, so we move on to pinching.
        mInstructionLabel->setText("Now pinch to enlarge");
        mGestureImage->setImageSource(QUrl("asset:///images/gesturehandler/broken.png"));
        mGestureState = "pinch";
    }
}

void GestureHandlerRecipe::onLongPress(bb::cascades::LongPressEvent* longPressEvent)
{
    Q_UNUSED(longPressEvent)

    if (mGestureState.compare("longpress") == 0) {

        // Run the animation for removing the gesture object.
        mLongPressOut->play();
        mInstructionLabel->setOpacity(0);
    }
}

void GestureHandlerRecipe::onPinchStart(bb::cascades::PinchEvent* pinchEvent)
{
    Q_UNUSED(pinchEvent);

    if (mGestureState.compare("pinch") == 0) {
        mPinchInProgress = true;
    }
}

void GestureHandlerRecipe::onPinchEnd(bb::cascades::PinchEvent* pinchEvent)
{
    Q_UNUSED(pinchEvent);

    if (mPinchInProgress) {
        // Save the current scaling of the image, so that the
        // zoom factor can be adjusted for this during the next pinch session.
        mPreviousPinchRatio = mGestureImage->scaleX() - 1.0;
    } else {
        mPreviousPinchRatio = 0;
        mGestureState = "longpress";
    }
}

void GestureHandlerRecipe::onPinchUpdate(bb::cascades::PinchEvent* pinchEvent)
{
    if (mPinchInProgress) {

        // The total scale factor is the sum of the current pinch ratio
        // and the accumulated pinching performed previously.
        float totalScaleFactor = pinchEvent->pinchRatio() + mPreviousPinchRatio;
        mGestureContainer->setScaleX(totalScaleFactor);
        mGestureContainer->setScaleY(totalScaleFactor);
        mGestureContainer->setRotationZ(pinchEvent->rotation());

        if (totalScaleFactor > 2.0) {

            // When the pinching is larger then 2.0, it's time to try out the
            // long press gesture.
            mPinchInProgress = false;
            mInstructionLabel->setText("Great! Now long-press");
        } else if (totalScaleFactor < 1.0) {
            mInstructionLabel->setText("I said ENLARGE, change direction");
        } else {
            mInstructionLabel->setText("Continue pinching");
        }
    }
}

void GestureHandlerRecipe::onPinchCancel()
{

}

void GestureHandlerRecipe::onLongPressOutEnded()
{
    // Reset gesture object to start over.
    mInstructionLabel->setOpacity(1);
    mInstructionLabel->setText("Well done, tap to start over");
    mGestureContainer->setScaleX(1);
    mGestureContainer->setScaleY(1);
    mGestureContainer->setRotationZ(0);
    mGestureContainer->setOpacity(0);
    mGestureImage->setImageSource(QUrl("asset:///images/gesturehandler/whole.png"));
    mGestureState = "tap";
}
