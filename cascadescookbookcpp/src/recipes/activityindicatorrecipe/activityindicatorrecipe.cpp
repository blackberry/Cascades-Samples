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
#include "activityindicatorrecipe.h"
#include "inlineactivityindicator.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Image>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;
using namespace customcs;

ActivityIndicatorRecipe::ActivityIndicatorRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The recipe Container
    Container *recipeContainer = Container::create().layout(DockLayout::create());

    // An in-line custom ActivityIndicator with the same functionality as the
    // bare bone ActivityIndicator but adjusted to follow UX recommendations on placement.
    // see inlineactivityindicator.cpp/h for further details on how to use the ActivtyIndicator.
    mActivityIndicator = new InlineActivityIndicator();
    mActivityIndicator->setVerticalAlignment(VerticalAlignment::Bottom);
    mActivityIndicator->setIndicatorText("boiling, boiling, boiling!");




    // Below the rest of the UI is set up containing a text, an egg (two images
    // one whole and one broken egg) and a toggleButton to start cooking the egg
    Container *eggBoilingSimulator = Container::create()
                                        .horizontal(HorizontalAlignment::Center)
                                        .vertical(VerticalAlignment::Center);

    // We alternate between to images as the ActivityIndicator is turned on and off.
    mUnbrokenImage =  Image(QUrl("asset:///images/stockcurve/egg.png"));
    mBrokenImage =  Image(QUrl("asset:///images/stockcurve/broken_egg.png"));
    mEggImageView = ImageView::create().image(mBrokenImage).horizontal(HorizontalAlignment::Center);
    mEggImageView ->setBottomMargin(ui->du(3));

    // The ToggleButton is grouped with a label in a left to right oriented StackLayout.
    Container *toggleContainer = Container::create().layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
    toggleContainer->setHorizontalAlignment(HorizontalAlignment::Center);

    Label *introText = new Label();
    introText->setText((const QString) "Egg boiling simulator");
    introText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());

    // Connect to the clicked signal in order to start and top the ActivityIndicator.
    ToggleButton *mButton = new ToggleButton();
    connectResult = connect(mButton, SIGNAL(checkedChanged(bool)), this, SLOT(onBoilEgg(bool)));
    Q_ASSERT(connectResult);

    // Add the text, the egg and the button.
    toggleContainer->add(introText);
    toggleContainer->add(mButton);

    // Group the ImageView and ToggleButton together in a Container.
    eggBoilingSimulator->add(mEggImageView);
    eggBoilingSimulator->add(toggleContainer);

    // Add the controls to the recipe Container and set it as root.
    recipeContainer->add(eggBoilingSimulator);
    recipeContainer->add(mActivityIndicator);

    setRoot(recipeContainer);
}

void ActivityIndicatorRecipe::onBoilEgg(bool checked)
{
    if (checked) {
        mActivityIndicator->start();
        mEggImageView->setImage(mUnbrokenImage);
    } else {
        mActivityIndicator->stop();
        mEggImageView->setImage(mBrokenImage);
    }
}

