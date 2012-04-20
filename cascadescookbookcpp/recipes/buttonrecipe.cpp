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
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

ButtonRecipe::ButtonRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeContainer->setLayout(recipeLayout);

    // A button that will cycle through all different Control state is created.
    mFruitButton = new Button();
    mFruitButton->setText((const char*) "Ripen");

    // Set the icon source by creating an image asset from the png image (note the file ending
    // not added to the file).
    mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_green"));
    mFruitButton->setTopMargin(40.0f);

    // Subscribe to Button click signals in the onButtonClicked function.
    connect(mFruitButton, SIGNAL(clicked()), this, SLOT(onRipenButtonClicked()));

    // A Button is created with the text "Eat", it will consume the first button.
    mEatButton = new Button();
    mEatButton->setText((const char*) "Eat");
    mEatButton->setTopMargin(40.0f);

    // Subscribe to Button click signals in the onButtonClicked function.
    connect(mEatButton, SIGNAL(clicked()), this, SLOT(onEatButtonClicked()));

    // A disabled Button with the text "New fruit".
    mNewButton = new Button();
    mNewButton->setText((const char*) "New fruit");
    mNewButton->setEnabled(false);
    mNewButton->setTopMargin(40.0f);

    // Subscribe to Button click signals in the onButtonClicked function.
    connect(mNewButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));

    recipeContainer->add(mFruitButton);
    recipeContainer->add(mEatButton);
    recipeContainer->add(mNewButton);

    setRoot(recipeContainer);
    mState = 0;
}

void ButtonRecipe::onRipenButtonClicked()
{
    // Alternate the Button icon on click.
    switch (mState) {
        case 0: {
            mState = 1;
            mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_ripe"));
            break;
        }
        case 1: {
            mState = 2;
            mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_mouldy"));
            mNewButton->setEnabled(true);
            break;
        }
        case 2: {
            mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_dust"));
            mFruitButton->setEnabled(false);
            break;
        }
    }
}

void ButtonRecipe::onEatButtonClicked()
{
    // Change the button text on click and hide the fruit button.
    mEatButton->setText("Burp");
    mEatButton->setEnabled(false);

    mFruitButton->setOpacity(0.0);

    mNewButton->setEnabled(true);
}

void ButtonRecipe::onNewButtonClicked()
{
    // Reset all the buttons to their original state.
    mFruitButton->setImageSource(QUrl("asset:///images/button/button_icon_orange_green"));
    mFruitButton->setEnabled(true);
    mFruitButton->setOpacity(1.0);
    mState = 0;

    mEatButton->setText("Eat");
    mEatButton->setEnabled(true);

    mNewButton->setEnabled(false);
}

