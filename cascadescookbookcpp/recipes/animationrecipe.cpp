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
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TranslateTransition>

using namespace bb::cascades;

#define CONTENT_WIDTH 768.0f

AnimationRecipe::AnimationRecipe(Container *parent) :
    CustomControl(parent)
{
  Container *recipeContainer = new Container();
  DockLayout *recipeLayout = new DockLayout();
  recipeContainer->setLayout(recipeLayout);
  recipeContainer->setPreferredSize(CONTENT_WIDTH, 1280);

  // Create the top Container containing the animated objects.
  Container *animationContainer = setUpAnimationContainer();

  // Create the bottom Container that controls animations and dock it to the
  // bottom of the screen.
  Container *controllerContainer = setUpControllerContainer();
  controllerContainer->setVerticalAlignment(VerticalAlignment::Bottom);

  // Add the two Containers to the main CustomControl Container.
  recipeContainer->add(animationContainer);
  recipeContainer->add(controllerContainer);

  setRoot(recipeContainer);
}

void AnimationRecipe::setUpAnimations(Container *animatedEgg)
{
  // The show animation translates the egg into screen.
  mShowMoreEgg = TranslateTransition::create(animatedEgg).toX(430).duration(600).parent(this);

  // The hide animation translates the egg off the screen.
  mHideMoreEgg = TranslateTransition::create(animatedEgg).toX(CONTENT_WIDTH).duration(600).parent(this);

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
  Container *animationContainer = new Container();
  animationContainer->setLayout(new AbsoluteLayout());

  // The background image
  ImagePaint paint(QUrl("asset:///images/dark_background.png"));
  animationContainer->setBackground(paint);
  animationContainer->setPreferredSize(CONTENT_WIDTH, 674.0f);

  // Create Container to hold two "super" eggs stacked side by side.
  // This entire Container is scaled by an implicit animation when the toggle is switched.
  mSuperEggs = new Container();
  mSuperEggs->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
  mSuperEggs->setLayoutProperties(AbsoluteLayoutProperties::create().y(360.0f));
  mSuperEggs->setPreferredHeight(450);

  // When scaling the entire Container down, it should be done at a point corresponding to left edge.
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

  // The egg image
  ImageView *eggImage = ImageView::create("asset:///images/animation/egg_isolated.png");
  eggImage->setPreferredSize(259, 203);
  eggImage->setPivotY(eggImage->preferredHeight() / 2);
  eggImage->setLayoutProperties(AbsoluteLayoutProperties::create().x(40));
  eggImage->setObjectName("eggImage");

  // The egg shadow put beneath the egg in Y direction
  ImageView *shadowImage = ImageView::create("asset:///images/animation/egg_isolated_shadow.png");
  shadowImage->setPreferredSize(351, 297);
  shadowImage->setPivotY(-shadowImage->preferredHeight() / 2);
  shadowImage->setLayoutProperties(AbsoluteLayoutProperties::create().y(150));
  shadowImage->setObjectName("shadowImage");

  // Add the images to the Container.
  animationEggContainer->add(shadowImage);
  animationEggContainer->add(eggImage);

  return animationEggContainer;
}

Container *AnimationRecipe::setUpControllerContainer()
{
  // The controllerContainer is the bottom part of the animation recipe.
  // It is where the descriptive text and a toggle button for triggering the
  // animations are kept.
  Container *controllerContainer = new Container();
  DockLayout *controllerLayout = new DockLayout();
  controllerContainer->setLayout(controllerLayout);
  controllerContainer->setLeftPadding(35.0f);
  controllerContainer->setPreferredSize(CONTENT_WIDTH, 360.0f);
  controllerContainer->setVerticalAlignment(VerticalAlignment::Bottom);
  ImagePaint paint(QUrl("asset:///images/background.png"), RepeatPattern::XY);
  controllerContainer->setBackground(paint);

  // A recipe text
  Container *descriptionContainer = new Container();
  descriptionContainer->setTopPadding(25.0f);

  // A Label is used to contain the header and a text area for the descriptive text.
  Label *descriptionHeader = new Label();
  descriptionHeader->setText("Scrambled eggs");
  descriptionHeader->textStyle()->setBase(SystemDefaults::TextStyles::bigText());
  descriptionHeader->textStyle()->setColor(Color::Black);

  // A label for the three steps describing how to scramble eggs
  Label *scrambledText = new Label();
  scrambledText->setText("1. Take two eggs.\n2. Scramble them.\n3. Done.");
  scrambledText->setMultiline(true);
  scrambledText->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
  scrambledText->textStyle()->setColor(Color::Black);
  scrambledText->textStyle()->setLineHeight(1.4);

  // Add the Labels to the descriptionContainer.
  descriptionContainer->add(descriptionHeader);
  descriptionContainer->add(scrambledText);

  // The controller is a ToggleButton with descriptive Label.
  // They are stacked in a Container that is aligned to the bottom-right corner.
  Container *toggleContainer = new Container();
  toggleContainer->setBottomPadding(35.0f);
  toggleContainer->setRightPadding(35.0f);
  toggleContainer->setVerticalAlignment(VerticalAlignment::Bottom);
  toggleContainer->setHorizontalAlignment(HorizontalAlignment::Right);

  // Set up the a Label with a descriptive text.
  Label *actionLabel = new Label();
  actionLabel->setHorizontalAlignment(HorizontalAlignment::Right);
  actionLabel->setText("Super size");
  actionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
  actionLabel->textStyle()->setColor(Color::Black);

  // Set up the ToggleButton and connect to its onChanged signal. In
  // the slot function onToggleChanged, we trigger the animations.
  ToggleButton *toggle = new ToggleButton();
  toggle->setHorizontalAlignment(HorizontalAlignment::Right);
  connect(toggle, SIGNAL(checkedChanged(bool)), this,
      SLOT(onToggleChanged(bool)));

  // Add the Label and ToggleButton to the toggleContainer then add
  // that Container to the main controllerContainer.
  toggleContainer->add(toggle);
  toggleContainer->add(actionLabel);

  // Add the descriptionContainer and toggleContainer to the controllerContainer
  controllerContainer->add(descriptionContainer);
  controllerContainer->add(toggleContainer);

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
  // When the third egg is hidden (its animation has completed),
  // scale the super eggs back to their initial size.
  mSuperEggs->setScale(0.7);
}

void AnimationRecipe::onHideAnimEnded()
{
  // When the third egg is hidden (its animation has completed),
  // scale the super eggs back to their initial size.
  mSuperEggs->setScale(1.0);
}
