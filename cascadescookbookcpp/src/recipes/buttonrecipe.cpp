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

#include "buttonrecipe.h"

#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageButton>
#include <bb/cascades/StackLayout>

using namespace bb::cascades;

ButtonRecipe::ButtonRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();

    // Create a Button that will cycle through all different Control states.
    mFruitButton = new Button();
    mFruitButton->setText((const char*) "Ripen");

    // Set the icon source by creating an image asset from the PNG image (note that the file
    // extension format is not specified as it is handled by the setImageSource function).
    mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_green.png"));
    mFruitButton->setTopMargin(40.0f);

    // Connect the Button clicked signal to the onButtonClicked function.
    connect(mFruitButton, SIGNAL(clicked()), this, SLOT(onRipenButtonClicked()));

    // Create a Button with the text "Eat", it will consume the first button.
    mEatButton = new Button();
    mEatButton->setText((const char*) "Eat");
    mEatButton->setTopMargin(40.0f);

    // Connect the Button clicked signals to the onButtonClicked function.
    connect(mEatButton, SIGNAL(clicked()), this, SLOT(onEatButtonClicked()));

    // Create a disabled ImageButton with the text "New fruit" set.
    mNewButton = new ImageButton();
    mNewButton->setDefaultImageSource(QUrl("asset:///images/button/image_button_enabled.png"));
    mNewButton->setPressedImageSource(QUrl("asset:///images/button/image_button_selected.png"));
    mNewButton->setDisabledImageSource(QUrl("asset:///images/button/image_button_disabled.png"));
    mNewButton->setEnabled(false);
    mNewButton->setTopMargin(40.0f);
    mNewButton->setHorizontalAlignment(HorizontalAlignment::Center);

    // Connect the Button click signals to the onButtonClicked function.
    connect(mNewButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));

    recipeContainer->add(mFruitButton);
    recipeContainer->add(mEatButton);
    recipeContainer->add(mNewButton);

    setRoot(recipeContainer);
    mState = 0;
}

void ButtonRecipe::onRipenButtonClicked()
{
    // Alternate the Button icon when clicked depending on the state of mState.
    switch (mState) {
        case 0: {
            mState = 1;
            mFruitButton->setImageSource(
                    QUrl("asset:///images/button/button_icon_orange_ripe.png"));
            break;
        }
        case 1: {
            mState = 2;
            mFruitButton->setImageSource(
                    QUrl("asset:///images/button/button_icon_orange_mouldy.png"));
            mNewButton->setEnabled(true);
            break;
        }
        case 2: {
            mFruitButton->setImageSource(
                    QUrl("asset:///images/button/button_icon_orange_dust.png"));
            mFruitButton->setEnabled(false);
            break;
        }
    }
}

void ButtonRecipe::onEatButtonClicked()
{
    // Change the button text when clicked and hide the fruit button.
    mEatButton->setText("Burp");
    mEatButton->setEnabled(false);

    mFruitButton->setOpacity(0.0);

    mNewButton->setEnabled(true);
}

void ButtonRecipe::onNewButtonClicked()
{
    // Reset all the buttons to their original state.
    mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_green.png"));
    mFruitButton->setEnabled(true);
    mFruitButton->setOpacity(1.0);
    mState = 0;

    mEatButton->setText("Eat");
    mEatButton->setEnabled(true);

    mNewButton->setEnabled(false);
}

