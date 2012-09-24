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
#include "activityindicatorrecipe.h"

#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

ActivityIndicatorRecipe::ActivityIndicatorRecipe(Container *parent) :
    CustomControl(parent)
{
  // The recipe Container
  Container *recipeContainer = new Container();
  recipeContainer->setLeftPadding(20.0);
  recipeContainer->setRightPadding(20.0);

  // The introduction text
  TextArea *introText = new TextArea();
  introText->setText((const QString) "This is a milk boiling simulator recipe");
  introText->setEditable(false);
  introText->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
  introText->setBottomMargin(100);

  Container* smashContainer = new Container();
  smashContainer->setLayout(new DockLayout());

  // Create the unbrown egg ImageView
  mUnbroken = ImageView::create("asset:///images/stockcurve/egg.png");

  // Center the unbrown egg image
  mUnbroken->setHorizontalAlignment(HorizontalAlignment::Center);
  mUnbroken->setVerticalAlignment(VerticalAlignment::Center);

  // Since this broken egg image is on top of the unbroken egg image, we can hide
  // this image by changing the opacity value of this image.
  mBroken = ImageView::create("asset:///images/stockcurve/broken_egg.png").opacity(0.0);

  // Center the brown egg image (same as unbroken one)
  mBroken->setHorizontalAlignment(HorizontalAlignment::Center);
  mBroken->setVerticalAlignment(VerticalAlignment::Center);

  mActivityIndicator = new ActivityIndicator();
  mActivityIndicator->setPreferredSize(130, 130);

  smashContainer->add(mUnbroken);
  smashContainer->add(mActivityIndicator);
  smashContainer->add(mBroken);

  mButton = new Button();
  mButton->setTopMargin(100);
  mButton->setText((const QString) "start cooking");
  connect(mButton, SIGNAL(clicked()), this, SLOT(onClicked()));

  // Add the controls to the recipe Container and set it as root.
  recipeContainer->add(introText);
  recipeContainer->add(smashContainer);

  recipeContainer->add(mButton);

  setRoot(recipeContainer);

}
void ActivityIndicatorRecipe::onClicked()
{

  if (mButton->text() == "start cooking") {
    mActivityIndicator->start();
    mButton->setText((const QString) "look away");
  } else {
    mActivityIndicator->stop();
    mButton->setEnabled(false);
    mButton->setText((const QString) "clean up");
    mUnbroken->setOpacity(0.0);
    mBroken->setOpacity(1.0);
  }

}

