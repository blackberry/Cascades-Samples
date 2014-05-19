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
#include "intro.h"
#include "uivalues.h"

#include <bb/cascades/Button>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/Slider>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

Intro::Intro(Container *parent) :
        CustomControl(parent)
{
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The intro recipe is a greeting screen with a head line, a small example UI
    // and a short intro text. The components are stacked in a Container.
    Container *recipeContainer = Container::create().top(ui->du(2)).left(ui->du(2)).right(ui->du(2));

    // The headline set in the largest available system font
    Label *headArea = new Label();
    headArea->setText((const QString) "Cascades Cookbook");
    headArea->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // The introduction text
    Label *introText = new Label();
    introText->setText(
            (const QString) "In this cookbook, I've put a list of ingredients for making user interfaces. Just like sushi, the end result will depend on your skill of aligning and balancing the components until they look just right.\n\nGood luck!\n/The Chef");
    introText->setMultiline(true);
    introText->textStyle()->setLineHeight(1.1);
    introText->setLayoutProperties(StackLayoutProperties::create());

    // The example UI is set up in a helper function, we add some space at the top and bottom
    // to get some space in the overall recipe UI.
    Container *exampleUI = setUpExampleUI();
    exampleUI->setTopMargin(ui->du(3));
    exampleUI->setBottomMargin(ui->du(3));

    // Add the Controls top the recipe Container.
    recipeContainer->add(headArea);
    recipeContainer->add(exampleUI);
    recipeContainer->add(introText);

    if (UiValues::instance()->device() == UiValues::DEVICETYPE_720X720) {
        // If the device is 720x720 resolution we make it scrollable.
        ScrollView *scrollRecipe = ScrollView::create()
                    .scrollMode(ScrollMode::Vertical).content(recipeContainer);
        setRoot(scrollRecipe);
    } else {
        // The content fits on this screen type so we just set the Container as the root Control.
        setRoot(recipeContainer);
    }
}

Container *Intro::setUpExampleUI()
{
    // A small example UI, illustrating some of the core controls.
    // The UI is arranged using a Container with a stack layout.
    Container *exampleUI = new Container();
    StackLayout *exampleUILayout = new StackLayout();

    exampleUI->setLayout(exampleUILayout);

    // A TextArea with text input functionality
    TextArea *exampleTextArea = new TextArea();
    exampleTextArea->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    exampleTextArea->setHorizontalAlignment(HorizontalAlignment::Fill);

    // An example of a Slider
    Slider *exampleSlider = new Slider();
    exampleSlider->setValue(0.333f);
    exampleSlider->setHorizontalAlignment(HorizontalAlignment::Left);
    exampleSlider->setVerticalAlignment(VerticalAlignment::Bottom);

    // A ToggleButton
    ToggleButton *exampleToggle = new ToggleButton();
    exampleToggle->setHorizontalAlignment(HorizontalAlignment::Right);

    // A regular Button
    Button *exampleButton = new Button();
    exampleButton->setText("Button");

    // Container for the buttons
    Container *buttonContainer = new Container();
    DockLayout *buttonContainerLayout = new DockLayout();

    buttonContainer->setLayout(buttonContainerLayout);
    buttonContainer->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Adding the buttons to the container.
    buttonContainer->add(exampleToggle);
    buttonContainer->add(exampleButton);

    // Add the Controls to the Container, the layouting is done using
    // layout properties and margins of each control (see code above).
    exampleUI->add(exampleTextArea);
    exampleUI->add(exampleSlider);
    exampleUI->add(buttonContainer);

    return exampleUI;
}
