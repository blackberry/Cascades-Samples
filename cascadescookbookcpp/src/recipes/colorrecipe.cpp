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
#include "uivalues.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/ImageView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/DockLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>

using namespace bb::cascades;

Container* ColorRecipe::createLabel(bb::cascades::Color myColor, const char* name)
{
    int tinyPadding = UiValues::instance()->intValue(UiValues::UI_PADDING_TINY);

    // Create a group Container that we want our controllers to be laid out left to right in.
    Container *groupContainer = new Container();
    groupContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
    groupContainer->setTopPadding(tinyPadding);

    Container *labelContainer = new Container();
    labelContainer->setVerticalAlignment(VerticalAlignment::Center);
    labelContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(2.0));

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
    colorContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(5.0));
    colorContainer->setLeftMargin(tinyPadding);

    // Last, add the created Containers to the groupContainer.
    labelContainer->add(label);
    groupContainer->add(labelContainer);
    groupContainer->add(colorContainer);

    return groupContainer;
}

Container* ColorRecipe::createIcon(bb::cascades::Color myColor, const char* name, const char* iconSrc, bb::cascades::HorizontalAlignment::Type aligment)
{
    // Create a group Container that we want our controllers to be laid out top to bottom in.
    Container *groupContainer = new Container();
    groupContainer->setHorizontalAlignment(aligment);
    groupContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::TopToBottom));

    // Icon using image path sent when calling the function.
    ImageView *icon = ImageView::create(iconSrc);
    icon->setHorizontalAlignment(HorizontalAlignment::Center);

    // Add a Label with padding and Color set.
    Label *label = new Label();
    label->setVerticalAlignment(VerticalAlignment::Bottom);
    label->setText(name);

    label->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    label->textStyle()->setFontWeight(FontWeight::Bold);
    label->textStyle()->setColor(myColor);
    label->setVerticalAlignment(VerticalAlignment::Bottom);

    // Last, add the created Containers to the groupContainer.
    groupContainer->add(icon);
    groupContainer->add(label);

    return groupContainer;
}

ColorRecipe::ColorRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the application padding values.
    int tinyPadding = UiValues::instance()->intValue(UiValues::UI_PADDING_TINY);
    int standardPadding = UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD);

	ScrollView *scrollView = new ScrollView();
	ScrollViewProperties* scrollViewProp = scrollView->scrollViewProperties();
	scrollViewProp->setScrollMode(ScrollMode::Vertical);

    // Create a Container that stacks our Labels and Containers from top to bottom.
    Container *recipeContainer = Container::create().left(standardPadding).right(standardPadding).top(standardPadding).bottom(standardPadding);

    Container *functionalContainer = Container::create();
    functionalContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    functionalContainer->setLayout(new DockLayout());

    // Header for functional colors
    Label *functionalLabel = new Label();
    functionalLabel->setText("Functional");
    functionalLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // Header for brand colors
	Label *brandLabel = new Label();
	brandLabel->setText("Brand");
	brandLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

	// Header for application colors
	Label *applicationLabel = new Label();
	applicationLabel->setText("Application");
	applicationLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

	// Header for background colors
	Label *backgroundLabel = new Label();
	backgroundLabel->setText("Background");
	backgroundLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    scrollView->setContent(recipeContainer);
    recipeContainer->add(functionalLabel);

    // Call the createLabel function and the createIcon function with all our different colors and add them to the recipe.

    // Functional colors.
    recipeContainer->add(functionalContainer);
    functionalContainer->add(createIcon(Color(Color::fromARGB(0xff00B800)), "#00B800", "asset:///images/color/icon_outgoing.png", HorizontalAlignment::Left));
    functionalContainer->add(createIcon(Color(Color::fromARGB(0xffE6B400)), "#E6B400", "asset:///images/color/icon_waiting.png", HorizontalAlignment::Center));
    functionalContainer->add(createIcon(Color(Color::fromARGB(0xffD60000)), "#D60000", "asset:///images/color/icon_missed.png", HorizontalAlignment::Right));

    // Brand colors.
    recipeContainer->add(brandLabel);
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff0073BC)), "#0073BC"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff00A8DF)), "#00A8DF"));

    // Application colors.
    recipeContainer->add(applicationLabel);
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff96B800)), "#96B800"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xffCC3F10)), "#CC3F10"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff0098F0)), "#0098F0"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xffA30D7E)), "#A30D7E"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff667B94)), "#667B94"));

    // Background colors.
    recipeContainer->add(backgroundLabel);
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff121212)), "#121212"));

    // The white background example is put in a container with a gray background so it will be visible.
    Container *whiteBackgroundContainer = Container::create().topMargin(tinyPadding).left(tinyPadding).right(tinyPadding).bottom(tinyPadding);
    whiteBackgroundContainer->setBackground(Color::fromARGB(0xff667B94));

    recipeContainer->add(whiteBackgroundContainer);
    whiteBackgroundContainer->add(createLabel(Color(Color::fromARGB(0xffF8F8F8)), "#F8F8F8"));
    recipeContainer->add(createLabel(Color(Color::fromARGB(0xff262626)), "#262626"));

    setRoot(scrollView);
}

