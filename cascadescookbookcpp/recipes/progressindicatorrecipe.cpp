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

#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/Slider>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

ProgressIndicatorRecipe::ProgressIndicatorRecipe(Container *parent) :
    CustomControl(parent)
{
  // The recipe Container
  Container *recipeContainer = Container::create().left(20.0).right(20.0);

  // The introduction text
  TextArea *introText = new TextArea();
  introText->setText((const QString) "Drag the slider to change the ProgressIndicator");
  introText->setEditable(false);
  introText->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
  introText->setBottomMargin(100);

  mProgressIndicator = new ProgressIndicator();
  mProgressIndicator->setFromValue(0);
  mProgressIndicator->setToValue(100);
  connect(mProgressIndicator, SIGNAL(valueChanged(float)), this, SLOT(onValueChanged(float)));

  // Create a Slider and connect a slot function to the signal for Slider value changing.
  Slider *slider = new Slider();
  slider->setTopMargin(100);
  slider->setFromValue(0);
  slider->setToValue(100);
  // Connect the Slider value directly to the value property of the ProgressIndicator.
  connect(slider, SIGNAL(immediateValueChanged(float)), mProgressIndicator, SLOT(setValue(float)));

  // Create a Slider and connect a slot function to the signal for Slider value changing.
  mButton = new Button();
  mButton->setText((const QString) "Pause");
  connect(mButton, SIGNAL(clicked()), this, SLOT(onClicked()));

  // Add the controls to the recipe Container and set it as root.
  recipeContainer->add(introText);
  recipeContainer->add(mProgressIndicator);
  recipeContainer->add(slider);
  recipeContainer->add(mButton);

  setRoot(recipeContainer);
}

void ProgressIndicatorRecipe::onClicked()
{
  if (mButton->text() == "Pause") {
    mButton->setText((const QString) "unPause");
    mProgressIndicator->setState(bb::cascades::ProgressIndicatorState::Pause);
  } else {
    mButton->setText((const QString) "Pause");
    mProgressIndicator->setState(bb::cascades::ProgressIndicatorState::Progress);
  }
}

void ProgressIndicatorRecipe::onValueChanged(float value)
{
  mButton->setText((const QString) "Pause");
  if (value == 0) {
    mProgressIndicator->setState(bb::cascades::ProgressIndicatorState::Indeterminate);

  } else if (value == 100) {
    mProgressIndicator->setState(bb::cascades::ProgressIndicatorState::Complete);
  } else {
    mProgressIndicator->setState(bb::cascades::ProgressIndicatorState::Progress);
  }

}
