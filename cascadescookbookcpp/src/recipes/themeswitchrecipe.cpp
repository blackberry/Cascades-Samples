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
#include "themeswitchrecipe.h"

#include <bb/cascades/Application>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Option>
#include <bb/cascades/Theme>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/VisualStyle>

using namespace bb::cascades;

ThemeSwitchRecipe::ThemeSwitchRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    mRecipeContainer = Container::create().top(ui->du(2)).left(ui->du(2)).right(ui->du(2));
    mRecipeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    mRecipeContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // A drop down control with different visuals styles that an application can have
    DropDown *dropDown = new DropDown();
    dropDown->setTitle("Theme");

    // Check if bright theme is set for the application so that we can set the correct selection in the DropDown.
    bool brightThemeSelected = (Application::instance()->themeSupport()->theme()->colorTheme()->style() == VisualStyle::Bright) ? true : false;

    // Set up Options and add it to the DropDown.
    dropDown->add(Option::create().text("Dark")
            .value(VisualStyle::Dark)
            .selected(!brightThemeSelected));
    dropDown->add(Option::create().text("Bright")
            .value(VisualStyle::Bright)
            .selected(brightThemeSelected));

    // Connect to the signal for index changes, so that we can update the recipe when a new selection is made.
    connectResult = connect(dropDown, SIGNAL(selectedOptionChanged(bb::cascades::Option*)), this,
            SLOT(onSelectedOptionChanged(bb::cascades::Option*)));
    Q_ASSERT(connectResult);

    mRecipeContainer->add(dropDown);
    setRoot(mRecipeContainer);
}

void ThemeSwitchRecipe::onSelectedOptionChanged(bb::cascades::Option* selectedOption)
{
    // Change the application theme using the visual style stored in the options value.
    Application::instance()->themeSupport()->setVisualStyle(selectedOption->value().toUInt());
}

