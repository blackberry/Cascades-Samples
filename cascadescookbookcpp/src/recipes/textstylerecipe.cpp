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
#include "textstylerecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Header>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

TextStyleRecipe::TextStyleRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    Container *recipeContainer = new Container();

    // The standard text styles that can be accessed via SystemDefaults.
    Header * defaultTextStylesHeader = new Header();
    defaultTextStylesHeader->setTitle("Default TextStyles");

    Container * defaultTextStylesContainer = Container::create().left(ui->du(2)).right(ui->du(2));

    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "BigText", SystemDefaults::TextStyles::bigText()));
    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "TitleText", SystemDefaults::TextStyles::titleText()));
    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "PrimaryText", SystemDefaults::TextStyles::primaryText()));
    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "BodyText", SystemDefaults::TextStyles::bodyText()));
    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "SubtitleText", SystemDefaults::TextStyles::subtitleText()));
    defaultTextStylesContainer->add(
            setUpLabelWithStyle((const QString) "SmallText", SystemDefaults::TextStyles::smallText()));

    // This is how you set the FontSize in a more explicit manner, check the last
    // Label for really explicit using PointValue.
    Header * fontSizeExamplesHeader = new Header();
    fontSizeExamplesHeader->setTitle("FontSize examples");

    Container * fontSizeExamplesContainer = Container::create().left(ui->du(2)).right(ui->du(2));

    fontSizeExamplesContainer->add(
            setUpLabelWithSize((const QString) "Small", FontSize::Small));
    fontSizeExamplesContainer->add(
            setUpLabelWithSize((const QString) "Large", FontSize::Large));
    fontSizeExamplesContainer->add(
            setUpLabelWithSize((const QString) "XLarge", FontSize::XLarge));
    fontSizeExamplesContainer->add(
            setUpLabelWithSize((const QString) "XXLarge", FontSize::XXLarge));
    Label * pointsizeLabel = setUpLabelWithSize((const QString) "PointValue 23",
            FontSize::PointValue);
    pointsizeLabel->textStyle()->setFontSizeValue(23);
    fontSizeExamplesContainer->add(pointsizeLabel);

    Header * fontFamilyGenericHeader = new Header();
    fontFamilyGenericHeader->setTitle("Font Family Generic");

    // There are a couple of generic font families that you can use for
    // formatting text. Use with care ;-)
    Container * fontFamilyGenericContainer = Container::create().left(ui->du(2)).right(ui->du(2));

    fontFamilyGenericContainer->add(
            setUpLabelWithFontFamily((const QString) "Sans-serif", "Sans-serif"));
    fontFamilyGenericContainer->add(
            setUpLabelWithFontFamily((const QString) "Serif", "Serif"));
    fontFamilyGenericContainer->add(
            setUpLabelWithFontFamily((const QString) "Monospace", "Monospace"));
    fontFamilyGenericContainer->add(
            setUpLabelWithFontFamily((const QString) "Fantasy", "Fantasy"));
    fontFamilyGenericContainer->add(
            setUpLabelWithFontFamily((const QString) "Cursive", "Cursive"));

    // Currently, text style rules can only be applied in QML. So, no costume font from c++, see the QML Cookbook sample for that

    recipeContainer->add(defaultTextStylesHeader);
    recipeContainer->add(defaultTextStylesContainer);

    recipeContainer->add(fontSizeExamplesHeader);
    recipeContainer->add(fontSizeExamplesContainer);

    recipeContainer->add(fontFamilyGenericHeader);
    recipeContainer->add(fontFamilyGenericContainer);

    ScrollView *scrollRecipe = ScrollView::create().scrollMode(ScrollMode::Vertical)
                    .content(recipeContainer);

    setRoot(scrollRecipe);
}

Label *TextStyleRecipe::setUpLabelWithStyle(const QString labelText, const TextStyle &textStyle)
{

    Label *label = new Label();
    label->setText(labelText);
    label->textStyle()->setBase(textStyle);

    return label;
}

Label *TextStyleRecipe::setUpLabelWithSize(const QString labelText,
        bb::cascades::FontSize::Type size)
{

    Label *label = new Label();
    label->setText(labelText);
    label->textStyle()->setFontSize(size);

    return label;
}

Label *TextStyleRecipe::setUpLabelWithFontFamily(const QString labelText,
        const QString & fontFamily)
{

    Label *label = new Label();
    label->setText(labelText);
    label->textStyle()->setFontFamily(fontFamily);

    return label;
}
