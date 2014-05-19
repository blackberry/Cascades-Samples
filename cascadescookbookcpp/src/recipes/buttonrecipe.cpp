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

#include "buttonrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Button>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageButton>
#include <bb/cascades/ScrollView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/resources/controlappearance.h>

using namespace bb::cascades;

ButtonRecipe::ButtonRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    Container *recipeContainer = Container::create().top(ui->du(3));

    // Create a Button that will cycle through all different Control states.
    mDisableButton = new Button();
    mDisableButton->setText("Disable");
    mDisableButton->setHorizontalAlignment(HorizontalAlignment::Center);

    // Connect the Button clicked signal to the onDisableButtonClicked function, so we can respond to button clicks.
    connectResult = connect(mDisableButton, SIGNAL(clicked()), this, SLOT(onDisableButtonClicked()));
    Q_ASSERT(connectResult);

    // Create a Button with an icon image, the image will alter as it is being clicked.
    mFruitButton = new Button();
    mFruitButton->setText("Ripen");
    mFruitButton->setHorizontalAlignment(HorizontalAlignment::Center);

    // Set the icon source by creating an image asset from the PNG image (note that the file
    // extension format is not specified as it is handled by the setImageSource function).
    mFruitButton->setImageSource(QUrl("asset:///images/button/icon_orange_green.png"));

    // Connect the Button clicked signal to the onButtonClicked function, so we can respond to button clicks.
    connectResult = connect(mFruitButton, SIGNAL(clicked()), this, SLOT(onRipenButtonClicked()));
    Q_ASSERT(connectResult);

    // Create a Button with the text "Eat", with a green color.
    mEatButton = new Button();
    mEatButton->setText("Eat");
    mEatButton->setColor(Color::fromARGB(0xff00B800));
    mEatButton->setHorizontalAlignment(HorizontalAlignment::Center);

    // Connect the Button clicked signals to the horizontalAlignment: HorizontalAlignment.Center function.
    connectResult = connect(mEatButton, SIGNAL(clicked()), this, SLOT(onEatButtonClicked()));
    Q_ASSERT(connectResult);

    // Create a Button with the text "New", that has the .
    mNewButton = new Button();
    mNewButton->setText("New");
    mNewButton->setAppearance(ControlAppearance::Primary);
    mNewButton->setHorizontalAlignment(HorizontalAlignment::Center);


    // Connect the Button clicked signals to the onButtonClicked function.
    connectResult = connect(mNewButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));
    Q_ASSERT(connectResult);

    // Create a disabled ImageButton with the text "New fruit" set.
    mImageButton = new ImageButton();
    mImageButton->setDefaultImageSource(QUrl("asset:///images/button/image_button_enabled.png"));
    mImageButton->setPressedImageSource(QUrl("asset:///images/button/image_button_selected.png"));
    mImageButton->setDisabledImageSource(QUrl("asset:///images/button/image_button_disabled.png"));
    mImageButton->setHorizontalAlignment(HorizontalAlignment::Center);

    // Connect the Button click signals to the onButtonClicked function.
    connectResult = connect(mImageButton, SIGNAL(clicked()), this, SLOT(onNewButtonClicked()));
    Q_ASSERT(connectResult);

    // Add the Buttons to the recipe Container.
    recipeContainer->add(mDisableButton);
    recipeContainer->add(mFruitButton);
    recipeContainer->add(mEatButton);
    recipeContainer->add(mNewButton);
    recipeContainer->add(mImageButton);

    if (UiValues::instance()->device() == UiValues::DEVICETYPE_720X720) {
        // If the device is of 720X720 resolution we make it scrollable.
        ScrollView *scrollRecipe = ScrollView::create()
                                    .content(recipeContainer);
        recipeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
        scrollRecipe->setHorizontalAlignment(HorizontalAlignment::Fill);
        setRoot(scrollRecipe);
    } else {
        // The content fits on this screen type so we just set the Container as the root Control.
        setRoot(recipeContainer);
    }

    mState = 0;
}

void ButtonRecipe::onDisableButtonClicked()
{
    // Show the eat button.
    mFruitButton->setEnabled(!mFruitButton->isEnabled());
    mEatButton->setEnabled(!mEatButton->isEnabled());
    mNewButton->setEnabled(!mNewButton->isEnabled());
    mImageButton->setEnabled(!mImageButton->isEnabled());

    // Toggle the button text.
    if(mDisableButton->text() == "Disable") {
        mDisableButton->setText("Enable");
    } else {
        mDisableButton->setText("Disable");
    }
}

void ButtonRecipe::onRipenButtonClicked()
{
    // Alternate the Button icon when clicked depending on the state of mState.
    switch (mState) {
        case 0: {
            mState = 1;
            mFruitButton->setImageSource(QUrl("asset:///images/button/icon_orange_green.png"));
            break;
        }
        case 1: {
            mState = 2;
            mFruitButton->setImageSource(QUrl("asset:///images/button/icon_orange_ripe.png"));
            break;
        }
        case 2: {
            mState = 3;
            mFruitButton->setImageSource(QUrl("asset:///images/button/icon_orange_mouldy.png"));
            break;
        }
        case 3: {
            mState = 0;
            mFruitButton->setImageSource(QUrl("asset:///images/button/icon_orange_dust.png"));
            break;
        }
    }
}

void ButtonRecipe::onEatButtonClicked()
{
    // Hide the eat button, it has been eaten.
    mEatButton->setVisible(false);
}

void ButtonRecipe::onNewButtonClicked()
{
    // Show the eat button.
    mEatButton->setVisible(true);
}


void ButtonRecipe::onImageButtonClicked()
{
    // Show the eat button.
    mEatButton->setVisible(true);
}
