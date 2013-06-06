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
#include "inlineactivityindicator.h"
#include <uivalues.h>

#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;
using namespace customcs;

ActivityIndicatorRecipe::ActivityIndicatorRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the UiValues instance for handling different resolutions.
    UiValues *uiValues = UiValues::instance();

    // The recipe Container
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new DockLayout());
    recipeContainer->setPreferredSize(uiValues->intValue(UiValues::SCREEN_WIDTH),
                                      uiValues->intValue(UiValues::SCREEN_HEIGHT));

    // An inline custom ActivityIndicator with the same functionality as the
    // bare bone ActivityIndicator but adjusted to follow UX recommendations on placement.
    // see inlineactivityindicator.cpp/h for further details on how to use the ActivtyIndicator.
    mActivityIndicator = new InlineActivityIndicator();
    mActivityIndicator->setVerticalAlignment(VerticalAlignment::Bottom);
    mActivityIndicator->setIndicatorText("Default text");

    // Below the rest of the UI is set up containing a text, an egg (two images
    // one whole and one broken egg) and a button to start cooking the egg
    Label *introText = new Label();
    introText->setText((const QString) "This is an egg boiling simulator.");
    introText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    introText->setBottomMargin(uiValues->intValue(UiValues::UI_PADDING_LARGE));

    Container* smashContainer = new Container();
    smashContainer->setLayout(new DockLayout());
    smashContainer->setHorizontalAlignment(HorizontalAlignment::Center);

    // A centered unbroken egg ImageView
    mUnbroken = ImageView::create("asset:///images/stockcurve/egg.png");
    mUnbroken->setHorizontalAlignment(HorizontalAlignment::Center);
    mUnbroken->setVerticalAlignment(VerticalAlignment::Center);

    // Since this broken egg image is on top of the unbroken egg image, we can hide
    // this image by changing the opacity value of this image (also centered).
    mBroken = ImageView::create("asset:///images/stockcurve/broken_egg.png").opacity(0.0);
    mBroken->setHorizontalAlignment(HorizontalAlignment::Center);
    mBroken->setVerticalAlignment(VerticalAlignment::Center);

    // Add the egg images to the same Container, the DockLayout will make sure they overlap.
    smashContainer->add(mUnbroken);
    smashContainer->add(mBroken);

    mButton = new Button();
    mButton->setTopMargin(uiValues->intValue(UiValues::UI_PADDING_LARGE));
    mButton->setText((const QString) "Start cooking");
    connect(mButton, SIGNAL(clicked()), this, SLOT(onClicked()));
    mButton->setHorizontalAlignment(HorizontalAlignment::Center);

    Container *uiContainer = new Container();
    uiContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    uiContainer->setTopPadding(uiValues->intValue(UiValues::UI_PADDING_LARGE));

    // Add the text, the egg and the button.
    uiContainer->add(introText);
    uiContainer->add(smashContainer);
    uiContainer->add(mButton);

    // Add the controls to the recipe Container and set it as root.
    recipeContainer->add(uiContainer);
    recipeContainer->add(mActivityIndicator);

    setRoot(recipeContainer);

}

void ActivityIndicatorRecipe::onClicked()
{
    if (mButton->text() == "Start cooking") {
        mActivityIndicator->start();
        mButton->setText((const QString) "Look away");
        mActivityIndicator->setIndicatorText("boiling, boiling, boiling!");
    } else if(mButton->text() == "Look away"){
        mActivityIndicator->stop();
        mButton->setText((const QString) "Clean up");
        mUnbroken->setOpacity(0.0);
        mBroken->setOpacity(1.0);
    } else {
        mButton->setText((const QString) "Start cooking");
        mUnbroken->setOpacity(1.0);
        mBroken->setOpacity(0.0);
    }

}

