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
#include "dropdownrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/GridLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/Option>
#include <bb/cascades/ScrollView>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

DropDownRecipe::DropDownRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    bool connectResult;
    Q_UNUSED(connectResult);

    // This Page does not fit on one screen, so a ScrollView is set up so
    // that the user can scroll the Page.
    ScrollView *scrollView = new ScrollView();
    Container *recipeContainer = Container::create()
                                    .top(ui->du(2)).left(ui->du(2))
                                    .right(ui->du(2));
    Label *title = new Label();
    title->setText("Beer recipe");
    title->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());


    // Set up DropDown component and add a number of options for different selections so
    // that the amount of pints produced can be chosen.
    DropDown *dropDown = new DropDown();
    dropDown->setTitle("Number of pints");

    // Either add options directly by using the convenience function...
    // this->instance().add(Option::create().text(optionText).value(value).selected(selected));
    dropDown->add(Option::create().text("1").value(QVariant(1)).selected(false));
    dropDown->add(Option::create().text("2").value(QVariant(2)).selected(true));
    dropDown->add(Option::create().text("3").value(QVariant(3)).selected(false));

    // Or set up an Option and add it to the DropDown manually.
    Option *dropDownOption = new Option();
    dropDownOption->setText("4");
    dropDownOption->setValue(QVariant(4));
    dropDown->add(dropDownOption);

    // Connect to the signal for index changes, so that we can update the recipe when a new selection is made.
    connectResult = connect(dropDown, SIGNAL(selectedIndexChanged(int)), this, SLOT(onSelectedIndexChanged(int)));
    Q_ASSERT(connectResult);


    // Create the Container containing the recipe text which will be modified by selecting an Option in the DropDown.
    Container *beerRecipe = createBeerRecipe();
    updateAmounts(dropDown);

    recipeContainer->add(title);
    recipeContainer->add(dropDown);
    recipeContainer->add(beerRecipe);

    // Add the scrollable content to the ScrollView.
    scrollView->setContent(recipeContainer);

    setRoot(scrollView);
}

void DropDownRecipe::onSelectedIndexChanged(int selectedIndex)
{
    Q_UNUSED(selectedIndex);

    // When the selection in the DropDown changes, we update the "amounts" text to reflect
    // the number of pints selected.
    DropDown* dropDown = dynamic_cast<DropDown*>(sender());
    updateAmounts(dropDown);
}

void DropDownRecipe::updateAmounts(DropDown *dropDown)
{
    Option* selectedOption = dropDown->at(dropDown->selectedIndex());
    float numberOfPints = (selectedOption->value()).toFloat();

    // Set up the new string for the amounts depending on the selected option value.
    QString amounts = QString("%1 kg\n%2 g\n%3 g\n%4 l").arg(numberOfPints * 0.1f).arg(
            numberOfPints * 1.0f).arg(numberOfPints * 0.6f).arg(numberOfPints * 1.5f);
    mAmounts->setText(amounts);

    // Update the image.
    QString imageSource = QString("asset:///images/dropdown/beer%1.png").arg((int) numberOfPints);
    mBeers->setImage(Image(imageSource));
}

Container *DropDownRecipe::createBeerRecipe()
{
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The recipe text is built up from three texts: an "amounts" text that
    // contain the different measures added to create the mix, an "ingredients"
    // text that is aligned to the amounts, and a formula describing the process.
    Container *recipe = new Container();
    recipe->setTopMargin(ui->du(2));

    Container *recipeMeasure = new Container(recipe);
    recipeMeasure->setLayout(GridLayout::create());

    mAmounts = new Label(recipeMeasure);
    mAmounts->setMultiline(true);
    mAmounts->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mAmounts->setPreferredWidth(ui->du(20));

    Label *ingredients = new Label(recipeMeasure);
    ingredients->setMultiline(true);
    ingredients->setText("Pale Ale Malt\nCascade Hops\nYeast\nWater");
    ingredients->textStyle()->setBase(SystemDefaults::TextStyles::titleText());

    // The imageSource will be set as a selection is made in the updateAmounts slot.
    mBeers = ImageView::create();
    mBeers->setScalingMethod(ScalingMethod::AspectFit);
    recipe->add(mBeers);

    Label *formula = new Label(recipe);
    formula->setMultiline(true);

    // We make sure the string is utf8 encoded to get the degree sign to show up correctly.
    QString recipeString = QString::fromUtf8("1. Mash at 67\u00B0C for 60 min\n\
2. Sparge.\n\
3. Boil the wort for 90 min.\n\
4. Add hops after 30 min.\n\
5. Add yeast, ferment 1-2 weeks.\n\
6. Add sugar and ferment in bottles for 1 week.\n\
7. Serve.");
    formula->setText(recipeString);

    return recipe;
}

