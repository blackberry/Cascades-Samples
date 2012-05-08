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
#include "intro.h"

#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/Slider>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/TextArea>
#include <bb/cascades/ToggleButton>

using namespace bb::cascades;

Intro::Intro(Container *parent) :
        CustomControl(parent)
{
    // The into recipe is a greeting screen with a head line a small example UI 
    // and a short intro text. The components are stacked in a container.
    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setTopPadding(50.0);
    recipeLayout->setLeftPadding(30.0);
    recipeLayout->setRightPadding(30.0);
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredHeight(1280.0f);

    // The headline set in the largest available system font.
    TextArea *headArea = new TextArea();
    headArea->setText((const QString) "Introduction to Cascades Cookbook");
    headArea->textStyle()->setBase(SystemDefaults::TextStyles::bigText()); 
    headArea->textStyle()->setColor(Color::LightGray);
    headArea->textStyle()->setFontWeight(FontWeight::Bold);
    headArea->setEditable(false);

    // The introduction text.
    TextArea *introText = new TextArea();
    introText->setText(
            (const QString) "In this cookbook, I've put a list of ingredients for making user interfaces. Just like sushi, the end result will depend on your skill of aligning and balancing the components until they look just right.\n\nGood luck!\n/The Chef");
    introText->setEditable(false);
    introText->setBackgroundVisible(false);
    introText->textStyle()->setColor(Color::LightGray);
    introText->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    introText->textStyle()->setLineSpacing(1.1);
    

    // The example UI is set up in a helper function, we add some space at the top and bottom
    // to get some space in the overall recipe UI.
    Container *exampleUI = setUpExampleUI();
    exampleUI->setTopMargin(70.0f);
    exampleUI->setBottomMargin(50.0f);

    // Add the Controls top the recipe Container.
    recipeContainer->add(headArea);
    recipeContainer->add(exampleUI);
    recipeContainer->add(introText);

    // Set the root of the CustomControl.
    setRoot(recipeContainer);
}

Container *Intro::setUpExampleUI()
{
    // A small example UI, illustrating some of the core controls.
    // The UI is arranged using a Container with a dock layout.
    Container *exampleUI = new Container();
    StackLayout *exampleUILayout = new StackLayout();

    exampleUI->setLayout(exampleUILayout);

    // A TextArea with text input functionality.
    TextArea *exampleTextArea = new TextArea();
    exampleTextArea->setPreferredHeight(65.0f);
    exampleTextArea->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    exampleTextArea->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    // An example of a Slider.
    Slider *exampleSlider = new Slider();
    exampleSlider->setValue(0.5f);
    exampleSlider->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Left).vertical(
                    VerticalAlignment::Bottom));

    // A ToggleButton.
    ToggleButton *exampleToggle = new ToggleButton();
    exampleToggle->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Right));

    // A regular Button.
    Button *exampleButton = new Button();
    exampleButton->setText("Button");

    // Container for the buttons
    Container *buttonContainer = new Container();
    DockLayout *buttonContainerLayout = new DockLayout();

    buttonContainer->setLayout(buttonContainerLayout);
    buttonContainer->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

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
