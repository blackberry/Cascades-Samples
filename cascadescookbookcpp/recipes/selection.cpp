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
#include "selection.h"
#include "selectioncheckbox.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Divider>
#include <bb/cascades/Textstyle>
#include <bb/cascades/Systemdefaults>
#include <bb/cascades/Label>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/Option>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

SelectionRecipe::SelectionRecipe(Container * parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setLeftPadding(80);
    recipeLayout->setRightPadding(80);
    recipeContainer->setLayout(recipeLayout);

    Container *checkBoxContainer = new Container();

    Label *typeLabel = new Label();
    typeLabel->setText("Olive Type");
    typeLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    typeLabel->textStyle()->setFontWeight(FontWeight::Bold);
    typeLabel->textStyle()->setColor(Color::LightGray) ;

    typeLabel->setLeftMargin(12.0f);

    checkBoxContainer->add(typeLabel);

    // Since we do not only want the CheckBox Control but also a Label and a
    // colored box to represent the selection of which olives should be in the
    // mix a CustomControl is used, see SelectionCheckBox.cpp.
    SelectionCheckBox *limonCello = new SelectionCheckBox();
    limonCello->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff808000)));
    limonCello->setTitle("Limoncello");
    limonCello->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));
    limonCello->setTextColor(Color::White) ;

    SelectionCheckBox *greek = new SelectionCheckBox();
    greek->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff698B22)));
    greek->setTitle("Greek");
    greek->setTextColor(Color::White) ;

    SelectionCheckBox *kalamata = new SelectionCheckBox();
    kalamata->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff733D1A)));
    kalamata->setTitle("Kalamata");
    kalamata->setTextColor(Color::White) ;

    checkBoxContainer->add(limonCello);
    checkBoxContainer->add(greek);
    checkBoxContainer->add(kalamata);

    // The two menu selections are separated by a Divider.
    Divider *divider = new Divider();
    divider->setTopMargin(30.0f);
    divider->setBottomMargin(30.0f);

    Label *fillingLabel = new Label();
    fillingLabel->setText("Filling");
    fillingLabel->setLeftMargin(12.0f);

    fillingLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    fillingLabel->textStyle()->setColor(Color::LightGray) ;

    // The RadioGroup is a Control that one use if only one option can
    // be selected at the time. RadioGroupOptions are added to the Control
    // and by listening for the onSelectedOptionChanged signal its possible
    // to keep track of what the current selection is.
    RadioGroup *radioGroup = new RadioGroup();
    radioGroup->setDividersVisible(false);

    Option *stoneOption = new Option();
    stoneOption->setText("Stone");

    Option *pimentoOption = new Option();
    pimentoOption->setText("Pimento");

    Option *garlicOption = new Option();
    garlicOption->setText("Garlic");

    radioGroup->add(stoneOption);
    radioGroup->add(pimentoOption);
    radioGroup->add(garlicOption);

    // We listen for changes in selected options on the RadioGroup.
    connect(radioGroup, SIGNAL(selectedIndexChanged(int)), this, 
            SLOT(fillingSelectedOptionChanged(int)));

    recipeContainer->add(checkBoxContainer);
    recipeContainer->add(divider);
    recipeContainer->add(fillingLabel);
    recipeContainer->add(radioGroup);

    setRoot(recipeContainer);
}

void SelectionRecipe::fillingSelectedOptionChanged(int selected)
{
    // When a new option is selected we print the selection to the console.
    qDebug() << "New filling set: " << selected;
}

