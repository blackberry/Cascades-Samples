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
#include "progressindicatorrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ToggleButton>

using namespace bb::cascades;

int const ProgressIndicatorRecipe::mCookingTime = 10;

ProgressIndicatorRecipe::ProgressIndicatorRecipe(Container *parent) :
        CustomControl(parent), mCookingProgress(0)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The recipe Container.
    Container *recipeContainer = Container::create();

    Container *progressContainer = Container::create().layout(
            StackLayout::create().orientation(LayoutOrientation::LeftToRight))
            .left(ui->du(2)).right(ui->du(2));

    // Set up to the progress indicator and connect to its value changed signal;
    mProgressIndicator = new ProgressIndicator();
    mProgressIndicator->setVerticalAlignment(VerticalAlignment::Center);
    mProgressIndicator->setFromValue(0);
    mProgressIndicator->setToValue(mCookingTime);
    mProgressIndicator->setState(ProgressIndicatorState::Indeterminate);

    connectResult = connect(mProgressIndicator, SIGNAL(valueChanged(float)), SLOT(onValueChanged(float)));
    Q_ASSERT(connectResult);

    // Create a Slider and connect a slot function to the signal for Slider value changing.
    mButton = new ToggleButton();
    connectResult = connect(mButton, SIGNAL(checkedChanged(bool)), this, SLOT(onToggleStove(bool)));
    Q_ASSERT(connectResult);

    progressContainer->add(mProgressIndicator);
    progressContainer->add(mButton);

    recipeContainer->add(setUpCookingSimulator());
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

Container *ProgressIndicatorRecipe::setUpCookingSimulator()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // A Container that will show a cooking pot.
    Container *potContainer = Container::create().bottom(ui->du(3))
            .horizontal(HorizontalAlignment::Center);

    // The lid of the pot
    mLid = ImageView::create("asset:///images/progressindicator/lid.png")
            .horizontal(HorizontalAlignment::Center).translate(0, ui->px(30));

    // An animation for the lid while the stove is on and something is cooking
    mCooking = SequentialAnimation::create(mLid)
                .add(RotateTransition::create(mLid).toAngleZ(2).duration(100))
                .add(RotateTransition::create(mLid).toAngleZ(-2).duration(100)).parent(this);

    // We connect to the end signal of the animation in order to update the cooking progress
    connectResult = connect(mCooking, SIGNAL(ended()), this, SLOT(onCookingAnimEnded()));
    Q_ASSERT(connectResult);

    ImageView *pot = ImageView::create("asset:///images/progressindicator/pot.png")
                        .horizontal(HorizontalAlignment::Center);

    potContainer->add(mLid);
    potContainer->add(pot);

    return potContainer;
}

