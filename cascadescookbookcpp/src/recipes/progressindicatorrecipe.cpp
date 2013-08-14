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
#include "progressindicatorrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/ScaleTransition>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ToggleButton>

using namespace bb::cascades;

int const ProgressIndicatorRecipe::mCookingTime = 10;

ProgressIndicatorRecipe::ProgressIndicatorRecipe(Container *parent) :
        CustomControl(parent), mCookingProgress(0)
{
    // The recipe Container
    Container *recipeContainer = Container::create();

    // A Container that will show a cooking pot.
    Container *potContainer = Container::create()
                .bottom(50).horizontal(HorizontalAlignment::Center);

    // The lid of the pot
    mLid = ImageView::create("asset:///images/progressindicator/lid.png")
        .horizontal(HorizontalAlignment::Center)
        .translate(0, 30);

    // An animation for the lid while the stove is on and something is cooking
    mCooking = SequentialAnimation::create(mLid)
            .add(RotateTransition::create(mLid).toAngleZ(2).duration(100))
            .add(RotateTransition::create(mLid).toAngleZ(-2).duration(100))
            .parent(this);

    // We connect to the end signal of the animation in order to update the cooking progress
    connect(mCooking, SIGNAL(ended()), this, SLOT(onCookingAnimEnded()));

    ImageView *pot = ImageView::create("asset:///images/progressindicator/pot.png")
        .horizontal(HorizontalAlignment::Center);

    potContainer->add(mLid);
    potContainer->add(pot);

    Container *progressContainer = Container::create()
            .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight))
            .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
            .right(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
            .bottom(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));


    // Set up to the progress indicator and connect to its value changed signal;
    mProgressIndicator = new ProgressIndicator();
    mProgressIndicator->setVerticalAlignment(VerticalAlignment::Center);
    mProgressIndicator->setFromValue(0);
    mProgressIndicator->setToValue(10);
    connect(mProgressIndicator, SIGNAL(valueChanged(float)), this, SLOT(onValueChanged(float)));


    // Create a Slider and connect a slot function to the signal for Slider value changing.
    mButton = new ToggleButton();
    connect(mButton, SIGNAL(checkedChanged(bool)), this, SLOT(onToggleStove(bool)));

    progressContainer->add(mProgressIndicator);
    progressContainer->add(mButton);

    recipeContainer->add(potContainer);
    recipeContainer->add(progressContainer);

    // Add the controls to the recipe Container and set it as root.
    setRoot(recipeContainer);
}
void ProgressIndicatorRecipe::onToggleStove(bool on)
{
    if (on) {
        if (mCookingProgress == mCookingTime) {
            mCookingProgress = 0;
            mProgressIndicator->setValue(mCookingProgress);
        }
        mCooking->play();

        // As soon as the cooking animation is turned on turn on progress
        mProgressIndicator->setState(ProgressIndicatorState::Progress);
    } else {
        if (mCookingProgress == mCookingTime) {
            // If the cooking time has reached its limit set state to complete
            mProgressIndicator->setState(ProgressIndicatorState::Complete);
        } else {
            // Otherwise pause the progress
            mProgressIndicator->setState(ProgressIndicatorState::Pause);
            mCooking->stop();
        }
    }
}

void ProgressIndicatorRecipe::onValueChanged(float value)
{
    if (value == 0) {
        // Value 0 means that the progress is undecided
        mProgressIndicator->setState(ProgressIndicatorState::Indeterminate);
    }
}

void ProgressIndicatorRecipe::onCookingAnimEnded()
{
    // Update cooking progress
    mCookingProgress = mCookingProgress + 1;

    // Update the progress indicators value
    mProgressIndicator->setValue(mCookingProgress);

    if (mCookingProgress == mCookingTime) {
        // Cooking is done turn off the stove and put the lid to rest
        mButton->setChecked(false);
        mLid->setRotationZ(0);
    } else {
        // Continue cooking
        mCooking->play();
    }
}

