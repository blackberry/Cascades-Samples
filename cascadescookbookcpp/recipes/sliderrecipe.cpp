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
#include "sliderrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/Slider>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

SliderRecipe::SliderRecipe(Container *parent) :
        CustomControl(parent)
{
    // The recipe Container
    Container *recipeContainer = Container::create().left(20.0).right(20.0);

    // A Label is used to present the current value (with one digit precision).
    mValueLabel = new Label();
    mValueLabel->setText((const QString) "value: 0");
    mValueLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());

    // Create a Slider and connect a slot function to the signal for Slider's immediate value.
    Slider *slider = new Slider();
    connect(slider, SIGNAL(immediateValueChanged(float)), this,
            SLOT(onSliderValueChanging(float)));

    // Add the controls to the recipe Container and set it as the root component.
    recipeContainer->add(mValueLabel);
    recipeContainer->add(slider);

    setRoot(recipeContainer);
}

void SliderRecipe::onSliderValueChanging(float value)
{
    // Convert the value to only have one decimal number.
    float valueOneDec = (float) (((int) (value * 10)) / 10.0f);

    // Create a QString from the number.
    QString newValue = NULL;
    newValue.setNum(valueOneDec, 'g', 1);

    // Present the new value in the Label.
    mValueLabel->setText((const QString) "value: " + newValue);
}
