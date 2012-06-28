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

#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Button>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TranslateTransition>

using namespace bb::cascades;

#define CONTENT_WIDTH 768.0f

AnimationRecipe::AnimationRecipe() :
        CustomControl()
{
    Container *recipeContainer = new Container();
    DockLayout *recipeLayout = new DockLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredSize(CONTENT_WIDTH, 1280);

    // Create the top Container containing objects that animates.
    Container *animationContainer = setUpAnimationContainer();

    // Create the bottom Container that controls animations and dock it to the
    // bottom of the screen.
    Container *controllerContainer = setUpControllerContainer();
    controllerContainer->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Bottom));

    // Add the two Containers to the main CustomControl Container.     
    recipeContainer->add(animationContainer);
    recipeContainer->add(controllerContainer);

    setRoot(recipeContainer);

    mHideAnimStopped = false;
    mShowAnimStopped = false;
}

void AnimationRecipe::setUpAnimations(Container *animatedEgg)
{
    // The show animation translates the egg into screen.
    mShowMoreEgg = TranslateTransition::create(animatedEgg).toX(430).duration(600).parent(this);

    // The hide animation translates the off the screen.
    mHideMoreEgg = TranslateTransition::create(animatedEgg).toX(CONTENT_WIDTH).duration(600).parent(this);

    // Connect to on end signal of the hide animation, after it is over
    // an implicit animation to scale the super eggs back will be run.
    connect(mHideMoreEgg, SIGNAL(ended()), this, SLOT(onHideAnimEnded()));

    // To avoid conflicting animations we have to stop ongoing animations before
    // starting them again, this can not be done in the same function call.
    // So we attach to the stop signal in order to start animations that was
    // just stopped.
    connect(mHideMoreEgg, SIGNAL(stopped()), this, SLOT(playStoppedAnimations()));
    connect(mShowMoreEgg, SIGNAL(stopped()), this, SLOT(playStoppedAnimations()));

    // For the extra egg we also add a tilt animation which will wiggle the
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
    Container *animationContainer = new Container();
    animationContainer->setLayout(new AbsoluteLayout());

    // The background image.
    ImagePaint paint(QUrl("asset:///images/dark_background.png"));
    animationContainer->setBackground(paint);
    animationContainer->setPreferredSize(768.0f, 674.0f);

    // The two "super" eggs, two big eggs stacked side by side.
    // This entire Container i scaled by an implicit animation when the toggle is switched.
    mSuperEggs = new Container();
    mSuperEggs->setLayout(StackLayout::create().direction(LayoutDirection::LeftToRight));
    mSuperEggs->setLayoutProperties(AbsoluteLayoutProperties::create().y(360.0f));
    mSuperEggs->setPreferredHeight(450);

    // When scaling the entire Container down it should be done on a point corresponding to left edge.
    mSuperEggs->setPivotX(-351);

    // Add the two initial eggs to the super egg Container.
    mSuperEggs->add(setUpAnimationEgg());
    mSuperEggs->add(setUpAnimationEgg());

    // A third egg will be animated in from the side after the super eggs
    // have been scaled down.
    Container *moreEgg = setUpAnimationEgg();
    moreEgg->setLayoutProperties(AbsoluteLayoutProperties::create().y(360.0f));
    moreEgg->setScale(0.7f);
    moreEgg->setTranslationX(CONTENT_WIDTH);

    setUpAnimations(moreEgg);

    // Add controls to the animation Container.
    animationContainer->add(mSuperEggs);
    animationContainer->add(moreEgg);

    return animationContainer;
}

Container *AnimationRecipe::setUpAnimationEgg()
{
    Container *animationEggContainer = new Container();
    animationEggContainer->setLayoutProperties(AbsoluteLayoutProperties::create().y(10));
    animationEggContainer->setLayout(new AbsoluteLayout());

    // The egg image.
    ImageView *eggImage = ImageView::create("asset:///images/animation/egg_isolated.png");
    eggImage->setPreferredSize(259, 203);
    eggImage->setPivotY(eggImage->preferredHeight() / 2);
    eggImage->setLayoutProperties(AbsoluteLayoutProperties::create().x(40));
    eggImage->setObjectName("eggImage");

    // The egg shadow put beneath the egg in Y direction.
    ImageView *shadowImage = ImageView::create("asset:///images/animation/egg_isolated_shadow.png");
    shadowImage->setPreferredSize(351, 297);
    shadowImage->setPivotY(-shadowImage->preferredHeight() / 2);
    shadowImage->setLayoutProperties(AbsoluteLayoutProperties::create().y(150));
    shadowImage->setObjectName("shadowImage");

    // Add the images.
    animationEggContainer->add(shadowImage);
    animationEggContainer->add(eggImage);

    return animationEggContainer;
}

Container *AnimationRecipe::setUpControllerContainer()
{
    // The Controller Container is the bottom part of the animation recipe.
    // It is where the descriptive text and a toggle button for triggering the
    // animations is kept.
    Container *controllerContainer = new Container();
    DockLayout *controllerLayout = new DockLayout();
    controllerLayout->setLeftPadding(30.0f);
    controllerContainer->setLayout(controllerLayout);
    controllerContainer->setPreferredSize(768.0f, 360.0f);
    controllerContainer->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Bottom));
    ImagePaint paint(QUrl("asset:///images/background.png"));
    controllerContainer->setBackground(paint);

    // A recipe text.
    Container *descriptionContainer = new Container();
    StackLayout *descriptionLayout = new StackLayout();
    descriptionLayout->setTopPadding(42.0f);
    descriptionContainer->setLayout(descriptionLayout);
    descriptionContainer->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Top).horizontal(
                    HorizontalAlignment::Left));

    // A Label is used for the header and a text area for the descriptive text.
    Label *descriptionHeader = new Label();
    descriptionHeader->textStyle()->setBase(SystemDefaults::TextStyles::bigText());
    descriptionHeader->textStyle()->setColor(Color::Black);
    descriptionHeader->setText("Scrambled eggs");
    descriptionHeader->setBottomMargin(32.0f);

    // Three labels for describing how to scramble the eggs.
    Label *line1 = new Label();
    line1->textStyle()->setBase(SystemDefaults::TextStyles::bodyText()); 
    line1->setText("1. Take two eggs.");
    line1->textStyle()->setColor(Color::Black);

    Label *line2 = new Label();
    line2->textStyle()->setBase(SystemDefaults::TextStyles::bodyText()); 
    line2->setText("2. Scramble them.");
    line2->textStyle()->setColor(Color::Black);

    Label *line3 = new Label();
    line3->textStyle()->setBase(SystemDefaults::TextStyles::bodyText()); 
    line3->setText("3. Done.");
    line3->textStyle()->setColor(Color::Black);

    // Add the texts to the description Container.
    descriptionContainer->add(descriptionHeader);
    descriptionContainer->add(line1);
    descriptionContainer->add(line2);
    descriptionContainer->add(line3);

    // The Controller is a toggle Button and it has a descriptive Label.
    // They are stacked in a Container that is aligned to the bottom right corner.
    Container *toggleContainer = new Container();
    StackLayout *toggleLayout = new StackLayout();
    toggleLayout->setBottomPadding(45.0f);
    toggleLayout->setRightPadding(30.0f);
    toggleContainer->setLayout(toggleLayout);
    toggleContainer->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Bottom).horizontal(
                    HorizontalAlignment::Right));

    // Set up of a Label with a descriptive text.
    Label *actionLabel = new Label();
    actionLabel->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Right));
    actionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText()); 
    actionLabel->setText("Super size");
    actionLabel->textStyle()->setColor(Color::Black);

    // Set up of a toggle Button and connect to its onChanged signal, its in
    // the slot function onToggleChanged were animations are triggered.
    ToggleButton *toggle = new ToggleButton();
    toggle->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Right));
    connect(toggle, SIGNAL(checkedChanged(bool)), this, SLOT(onToggleChanged(bool)));

    // Add the Label and the toggle Button to the toggle Container then add
    // that Container to the main controller Container.
    toggleContainer->add(toggle);
    toggleContainer->add(actionLabel);

    // Add the description and the toggle button Container.
    controllerContainer->add(descriptionContainer);
    controllerContainer->add(toggleContainer);

    return controllerContainer;
}

void AnimationRecipe::onToggleChanged(bool on)
{
    if (on == true) {
        // Scale down the super eggs to make room for a third egg.
        mSuperEggs->setScale(0.7f);

        // Stop ongoing animations, to avoid conflicts.
        mShowAnimStopped = checkPlayingAnimations();

        if (mShowAnimStopped == false) {
            // Show the extra egg.
            mShowMoreEgg->play();
            mTiltShadow->play();
            mTiltEgg->play();
        }
    } else {
        // Stop ongoing animations, to avoid conflicts.
        mHideAnimStopped = checkPlayingAnimations();

        if (mHideAnimStopped == false) {
            // Hide the extra egg.
            mHideMoreEgg->play();
        }
    }
}

void AnimationRecipe::onHideAnimEnded()
{
    // When the third egg is hidden (its animation has come to and end).
    // Scale the super eggs back to their initial size.
    mSuperEggs->setScale(1.0);
}

bool AnimationRecipe::checkPlayingAnimations()
{
    bool animationWasStopped = false;

    // This function checks if any of the animations are running. If that is the
    // case the animation is stopped and true is returned to notify the caller.
    if (mShowMoreEgg->isPlaying()) {
        mShowMoreEgg->stop();
        animationWasStopped = true;
    }
    if (mHideMoreEgg->isPlaying()) {
        mHideMoreEgg->stop();
        animationWasStopped = true;
    }

    return animationWasStopped;
}

void AnimationRecipe::playStoppedAnimations()
{
    // This is the slot function for the hide and show animations onStopped signal.
    // Since it is not possible to stop and start the same animation in the same
    // function call we have to set a variable and wait for onStopped to be called
    // before we can trigger the animation.
    if (mShowAnimStopped == true) {

        // Show the extra egg.
        mShowMoreEgg->play();
        mTiltShadow->play();
        mTiltEgg->play();
    } else if (mHideAnimStopped == true) {

        // Hide the extra egg.
        mHideMoreEgg->play();
    }

    mShowAnimStopped = false;
    mHideAnimStopped = false;
}
