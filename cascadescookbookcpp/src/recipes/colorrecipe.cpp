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

#include "colorrecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

Container* ColorRecipe::createLabel(bb::cascades::Color myColor, const char* name)
{
    // Create a group Container that we want our controllers to be layed out left to right in.
    Container *groupContainer = new Container();
    groupContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
    groupContainer->setTopPadding(4);

    // Add a Label with padding and Color set.
    Label *label = new Label();
    label->setText(name);

    label->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    label->textStyle()->setFontWeight(FontWeight::Bold);
    label->textStyle()->setColor(myColor);
    label->setVerticalAlignment(VerticalAlignment::Center);

    // Create the rectangle Container and set the Color by setting the background property.
    Container *colorContainer = new Container();
    colorContainer->setBackground(myColor);
    colorContainer->setVerticalAlignment(VerticalAlignment::Fill);
    colorContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1.0));
    colorContainer->setLeftMargin(10);

    // Last, add the created Containers to the groupContainer.
    groupContainer->add(label);
    groupContainer->add(colorContainer);

    return groupContainer;
}

ColorRecipe::ColorRecipe(Container *parent) :
        CustomControl(parent)
{
    // Create a Container that stacks our Labels and Containers from top to bottom.
    Container *recipeContainer = Container::create().left(80);

    // Call the createLabel function with all our different colors and add them to the recipe.
    recipeContainer->add(createLabel(Color(Color::Red), "Red"));
    recipeContainer->add(createLabel(Color(Color::DarkRed), "DarkRed"));
    recipeContainer->add(createLabel(Color(Color::Magenta), "Magenta"));
    recipeContainer->add(createLabel(Color(Color::DarkMagenta), "DarkMagenta"));
    recipeContainer->add(createLabel(Color(Color::Blue), "Blue"));
    recipeContainer->add(createLabel(Color(Color::DarkBlue), "DarkBlue"));
    recipeContainer->add(createLabel(Color(Color::Cyan), "Cyan"));
    recipeContainer->add(createLabel(Color(Color::DarkCyan), "DarkCyan"));
    recipeContainer->add(createLabel(Color(Color::Green), "Green"));
    recipeContainer->add(createLabel(Color(Color::DarkGreen), "DarkGreen"));
    recipeContainer->add(createLabel(Color(Color::Yellow), "Yellow"));
    recipeContainer->add(createLabel(Color(Color::DarkYellow), "DarkYellow"));
    recipeContainer->add(createLabel(Color(Color::White), "White"));
    recipeContainer->add(createLabel(Color(Color::LightGray), "LightGray"));
    recipeContainer->add(createLabel(Color(Color::Gray), "Gray"));
    recipeContainer->add(createLabel(Color(Color::DarkGray), "DarkGray"));
    recipeContainer->add(createLabel(Color(Color::Black), "Black"));
    recipeContainer->add(createLabel(Color::fromRGBA(0.5f, 0.5f, 0.5f), "Custom"));
    recipeContainer->add(createLabel(Color::fromARGB(0xffab2025), "Strawberry"));

    // There is a transparent Color as well, in this case it will of course
    // not be visible so we leave it out of the UI, see below for syntax.
    // recipeContainer->add(createLabel(Color(Color::Transparent), "Transparent"));

    setRoot(recipeContainer);
}

