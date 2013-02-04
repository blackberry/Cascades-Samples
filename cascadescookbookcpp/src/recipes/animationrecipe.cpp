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
#include "animationrecipe.h"
#include "uivalues.h"

#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TranslateTransition>

using namespace bb::cascades;

AnimationRecipe::AnimationRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new StackLayout());
    recipeContainer->setPreferredSize(UiValues::instance()->intValue(UiValues::SCREEN_WIDTH),
                                     UiValues::instance()->intValue(UiValues::SCREEN_HEIGHT));

    // Create the top Container containing the animated objects.
    Container *animationContainer = setUpAnimationContainer();

    // Create the bottom Container that controls animations
    Container *controllerContainer = setUpControllerContainer();

    // Add the two Containers to the main CustomControl Container.
    recipeContainer->add(animationContainer);
    recipeContainer->add(controllerContainer);

    setRoot(recipeContainer);
}

void AnimationRecipe::setUpAnimations(Container *animatedEgg)
{
    // The show animation translates the egg into screen.
    mShowMoreEgg = TranslateTransition::create(animatedEgg).toX(-230).duration(600).parent(this);

    // The hide animation translates the egg off the screen.
    mHideMoreEgg = TranslateTransition::create(animatedEgg).toX(0).duration(600).parent(
            this);

    // Connect to the ended signal of the hide animation, then we show
    // an implicit animation to scale the egg images back. Also connect the started signal to
    // the show animation.
    connect(mHideMoreEgg, SIGNAL(ended()), this, SLOT(onHideAnimEnded()));
    connect(mShowMoreEgg, SIGNAL(started()), this, SLOT(onShowAnimStarted()));

    // For the extra egg, we also add a tilt animation that will wiggle the
    // egg when it comes to an end. We need two animations to achieve this.
    // One for the eggImage and one for its shadow, they run in opposite directions.
    ImageView *eggImage = animatedEgg->findChild<ImageView*>("eggImage");

    mTiltEgg = SequentialAnimation::create(eggImage).add(
            RotateTransition::create(eggImage).toAngleZ(-15).duration(700 * 0.4f).delay(350)).add(
            RotateTransition::create(eggImage).toAngleZ(5).duration(700 * 0.3f)).add(
            RotateTransition::create(eggImage).toAngleZ(0).duration(700 * 0.3f)).parent(this);

    ImageView *shadowImage = animatedEgg->findChild<ImageView*>("shadowImage");

    mTiltShadow = SequentialAnimation::create(shadowImage).add(
            RotateTransition::create(shadowImage).toAngleZ(15).duration(700 * 0.4f).delay(350)).add(
            RotateTransition::create(shadowImage).toAngleZ(-5).duration(700 * 0.3f)).add(
            RotateTransition::create(shadowImage).toAngleZ(0).duration(700 * 0.3f)).parent(this);

}

Container *AnimationRecipe::setUpAnimationContainer()
{
    ImagePaint paint(QUrl("asset:///images/animation/dark_background.png"));

    // A background Container for the eggs painted with an image and space quota
    // set so that it takes as much of the available space as possible.
    Container *eggContainer = Container::create()
                .layout(DockLayout::create())
                .layoutProperties(StackLayoutProperties::create().spaceQuota(1))
                .background(paint);

    // The Container for the three eggs stacked left to right
    Container *superEggs = Container::create()
                .layout(StackLayout::create()
                .orientation(LayoutOrientation::LeftToRight))
                .top(80)
                .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .vertical(VerticalAlignment::Bottom);

    // Create the eggs
    mFirstEgg = setUpAnimationEgg();
    mSecondEgg = setUpAnimationEgg();
    mThirdEgg = setUpAnimationEgg();

    // Connect to the layout handler of first egg, we need the size
    // in order to set the pivot point for scaling correctly
    // (the second egg will have the same size and is handled at same time).
    LayoutUpdateHandler::create(mFirstEgg).onLayoutFrameChanged(this,
            SLOT(eggLayoutFrameUpdated(QRectF)));

    // A third egg will be animated in from the side after the two other
    // have been scaled down and has its own set of custom animations.
    setUpAnimations(mThirdEgg);

    // Setting space quota to a positive value will force it outside the screen
    mThirdEgg->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));

    // The Third egg is scaled down and pushed a little bit to the left
    // to end up outside the screen
    mThirdEgg->setScale(0.7);
    mThirdEgg->setLeftMargin(20);

    // Add the eggs.
    superEggs->add(mFirstEgg);
    superEggs->add(mSecondEgg);
    superEggs->add(mThirdEgg);

    // Add the superEggs to the top Container.
    eggContainer->add(superEggs);

    return eggContainer;
}

Container *AnimationRecipe::setUpAnimationEgg()
{
    Container *animationEggContainer = new Container();
    animationEggContainer->setLayout(new AbsoluteLayout());

    // The egg image
    ImageView *eggImage = ImageView::create("asset:///images/animation/egg_isolated.png");
    eggImage->setLayoutProperties(AbsoluteLayoutProperties::create().x(40));
    eggImage->setObjectName("eggImage");

    // The egg shadow put beneath the egg in Y direction
    ImageView *shadowImage = ImageView::create("asset:///images/animation/egg_isolated_shadow.png");
    shadowImage->setLayoutProperties(AbsoluteLayoutProperties::create().y(150));
    shadowImage->setObjectName("shadowImage");

    // The egg rotates around half the full height (the bottom of the egg),
    // the pivot point is in the middle of the image to start.
    eggImage->setPivotY(203 / 2);

    // The shadow rotates around half its height,
    // the pivot point is in the middle of the image to start.
    shadowImage->setPivotY(-297 / 2);

    // Add the images to the Container.
    animationEggContainer->add(shadowImage);
    animationEggContainer->add(eggImage);

    animationEggContainer->setClipContentToBounds(false);

    return animationEggContainer;
}

Container *AnimationRecipe::setUpControllerContainer()
{
    // The controllerContainer is the bottom part of the animation recipe.
    // It is where the descriptive text and a toggle button for triggering the
    // animations are kept.
    ImagePaint paint(QUrl("asset:///images/background.png"), RepeatPattern::XY);
    Container *controllerContainer = Container::create()
            .layout(new DockLayout()).horizontal(HorizontalAlignment::Fill).background(paint)
            .top(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
            .bottom(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
            .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
            .right(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));

    // Set up the a Label with a descriptive text.
    Label *actionLabel = new Label();
    actionLabel->setText("More Eggs");
    actionLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());

    // Set up the ToggleButton and connect to its onChanged signal. In
    // the slot function onToggleChanged, we trigger the animations.
    ToggleButton *toggle = new ToggleButton();
    toggle->setHorizontalAlignment(HorizontalAlignment::Right);
    connect(toggle, SIGNAL(checkedChanged(bool)), this, SLOT(onToggleChanged(bool)));

    controllerContainer->add(actionLabel);
    controllerContainer->add(toggle);

    return controllerContainer;
}

void AnimationRecipe::onToggleChanged(bool on)
{
    if (on == true) {
        // Show the extra egg
        mShowMoreEgg->play();
        mTiltShadow->play();
        mTiltEgg->play();
    } else {
        // Hide the extra egg
        mHideMoreEgg->play();
    }
}

void AnimationRecipe::onShowAnimStarted()
{
    // When the third egg is being shown (its animation has started),
    // scale the two other eggs down to make room for it.
    mFirstEgg->setScale(0.7);
    mSecondEgg->setScale(0.7);
}

void AnimationRecipe::onHideAnimEnded()
{
    // When the third egg is hidden (its animation has completed),
    // scale the two other eggs back to their initial size.
    mFirstEgg->setScale(1.0);
    mSecondEgg->setScale(1.0);
}

void AnimationRecipe::eggLayoutFrameUpdated(QRectF layoutRect)
{
    // Pivots points for first and second egg, they should scale
    // around the same point in x-direction.
    mFirstEgg->setPivotX(-layoutRect.width() * 0.5);
    mSecondEgg->setPivotX(-layoutRect.width() * 1.5);
}
