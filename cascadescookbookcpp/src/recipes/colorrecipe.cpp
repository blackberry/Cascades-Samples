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

#include "colorrecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/ScrollView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/UIPalette>

using namespace bb::cascades;

ColorRecipe::ColorRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the UIConfig object in order to use resolution independent sizes.
    // and set up the UIpadding values.
    UIConfig *ui = this->ui();
    int tinyPadding = ui->du(0.5);
    int standardPadding = ui->du(2);

    ScrollView *scrollView = new ScrollView();

    // Create a Container that stacks our Labels and Containers from top to bottom.
    Container *recipeContainer = Container::create()
                                    .left(standardPadding).right(standardPadding)
                                    .top(standardPadding).bottom(standardPadding);

    Container *functionalContainer = Container::create()
                                 .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));

    // Header for functional colors
    Label *functionalLabel = new Label();
    functionalLabel->setText("Functional");
    functionalLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // Header for primary colors
    Label *primaryLabel = new Label();
    primaryLabel->setText("Primary");
    primaryLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // Header for application colors
    Label *applicationLabel = new Label();
    applicationLabel->setText("Application");
    applicationLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // Header for text colors
    Label *textLabel = new Label();
    textLabel->setText("Text");
    textLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // Header for background colors
    Label *backgroundLabel = new Label();
    backgroundLabel->setText("Background");
    backgroundLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    scrollView->setContent(recipeContainer);
    recipeContainer->add(functionalLabel);

    // Call the createLabel function and the createIcon function with all our different colors and add them to the recipe.

    // Functional colors are represented by a white icon ImageView that is colored using the filterColor property.
    recipeContainer->add(functionalContainer);
    functionalContainer->add( createIcon(Color(Color::fromARGB(0xff00B800)),
                    "asset:///images/color/icon_outgoing.png"));
    functionalContainer->add(createIcon(Color(Color::fromARGB(0xffE6B400)),
                    "asset:///images/color/icon_waiting.png"));
    functionalContainer->add(createIcon(Color(Color::fromARGB(0xffD60000)),
                    "asset:///images/color/icon_missed.png"));

    // Primary colors.
    recipeContainer->add(primaryLabel);

    // The primary colors of the Application are available from the UIConfig object.
    recipeContainer->add(createLabel(ui->palette()->primary()));
    recipeContainer->add(createLabel(ui->palette()->primaryBase()));
    recipeContainer->add(createLabel(ui->palette()->primarySoft()));

    // Application colors.
    recipeContainer->add(applicationLabel);
    recipeContainer->add(createLabel(Color::fromARGB(0xff96B800)));
    recipeContainer->add(createLabel(Color::fromARGB(0xffCC3F10)));
    recipeContainer->add(createLabel(Color::fromARGB(0xff0098F0)));
    recipeContainer->add(createLabel(Color::fromARGB(0xffA30D7E)));
    recipeContainer->add(createLabel(Color::fromARGB(0xff667B94)));

    // Text colors.
    recipeContainer->add(textLabel);

    recipeContainer->add(createLabel(ui->palette()->text()));
    recipeContainer->add(createLabel(ui->palette()->textPrimary()));
    recipeContainer->add(createLabelWithBackground(ui->palette()->textOnPrimary(), ui->palette()->primary())) ;
    recipeContainer->add(createLabelWithBackground(ui->palette()->textOnPlain(), ui->palette()->plain()));

    // Background colors.
    recipeContainer->add(backgroundLabel);

    // The background colors are contrasted against the primary color to make them visible.
    recipeContainer->add(createLabelWithBackground(ui->palette()->background(), ui->palette()->primary()));
    recipeContainer->add(createLabelWithBackground(ui->palette()->plain(), ui->palette()->primary()));
    recipeContainer->add(createLabelWithBackground(ui->palette()->plainBase(), ui->palette()->primary()));

    setRoot(scrollView);
}

Container* ColorRecipe::createLabelWithBackground(bb::cascades::Color myColor, bb::cascades::Color background)
{
    UIConfig *ui = this->ui();
    int tinyPadding = ui->du(0.5);

    Container *backgroundContainer = createLabel(myColor);

    // If a background paint is used, increase the spacing around the swatch to make the background more visible.
    backgroundContainer->setTopPadding(tinyPadding);
    backgroundContainer->setBottomPadding(tinyPadding);
    backgroundContainer->setBackground(background);

    return backgroundContainer;
}

Container* ColorRecipe::createLabel(bb::cascades::Color myColor)
{
    UIConfig *ui = this->ui();
    int tinyPadding = ui->du(0.5);

    // Create a group Container that we want our controllers to be laid out left to right in.
    Container *groupContainer = new Container();
    groupContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
    groupContainer->setLeftPadding(tinyPadding);
    groupContainer->setRightPadding(tinyPadding);
    groupContainer->setTopPadding(tinyPadding/2.0);
    groupContainer->setBottomPadding(tinyPadding/2.0);

    Container *labelContainer = new Container();
    labelContainer->setVerticalAlignment(VerticalAlignment::Center);
    labelContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(5.0));

    // Add a Label with padding and Color set.
    Label *label = new Label();
    label->setText(colorToHexString(myColor));

    label->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    label->textStyle()->setFontWeight(FontWeight::Bold);
    label->textStyle()->setColor(myColor);
    label->setVerticalAlignment(VerticalAlignment::Center);

    // Create the rectangle Container and set the Color by setting the background property.
    Container *colorContainer = new Container();
    colorContainer->setBackground(myColor);
    colorContainer->setVerticalAlignment(VerticalAlignment::Fill);
    colorContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(11.0));
    colorContainer->setLeftMargin(tinyPadding);

    // Last, add the created Containers to the groupContainer.
    labelContainer->add(label);
    groupContainer->add(labelContainer);
    groupContainer->add(colorContainer);

    return groupContainer;
}

Container* ColorRecipe::createIcon(bb::cascades::Color myColor,
        const QString iconSrc)
{
    // Create a group Container that we want our controllers to be laid out top to bottom in.
    Container *groupContainer = Container::create()
                                    .layoutProperties(StackLayoutProperties::create().spaceQuota(1));

    // Icon using image path sent when calling the function.
    ImageView *icon = ImageView::create(iconSrc);
    icon->setHorizontalAlignment(HorizontalAlignment::Center);
    icon->setFilterColor(myColor);

    // Add a Label with padding and Color set.
    Label *label = new Label();
    label->setText(colorToHexString(myColor));
    label->setHorizontalAlignment(HorizontalAlignment::Center);
    label->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    label->textStyle()->setFontWeight(FontWeight::Bold);
    label->textStyle()->setColor(myColor);

    // Last, add the created Containers to the groupContainer.
    groupContainer->add(icon);
    groupContainer->add(label);

    return groupContainer;
}

QString ColorRecipe::colorToHexString(const Color color)
{
    QString colorString;
    QDebug(&colorString) << color;
    colorString = "#"+ colorString.mid(3, 8);
    return colorString.toUpper();
}
