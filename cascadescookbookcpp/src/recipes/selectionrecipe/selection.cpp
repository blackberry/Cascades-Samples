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
#include "selection.h"
#include "selectioncheckbox.h"
#include "uivalues.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Divider>
#include <bb/cascades/Label>
#include <bb/cascades/Option>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

SelectionRecipe::SelectionRecipe(Container * parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    Container *recipeContainer = Container::create().top(ui->du(3)).left(ui->du(2)).right(ui->du(2));
    Container *checkBoxContainer = new Container();

    Label *typeLabel = new Label();
    typeLabel->setText("Olive Type");
    typeLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    typeLabel->textStyle()->setFontWeight(FontWeight::Bold);
    typeLabel->setBottomMargin(0);

    checkBoxContainer->add(typeLabel);

    // Since we want the CheckBox Control to also have a colored box to represent the
    // selection of which olives should be in the mix, a CustomControl is used (see SelectionCheckBox.cpp).
    SelectionCheckBox *limonCello = new SelectionCheckBox();
    limonCello->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff808000)));
    limonCello->setTitle("Limoncello");
    limonCello->setHorizontalAlignment(HorizontalAlignment::Fill);

    SelectionCheckBox *greek = new SelectionCheckBox();
    greek->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff698B22)));
    greek->setTitle("Greek");

    SelectionCheckBox *kalamata = new SelectionCheckBox();
    kalamata->setOliveColor(QVariant::fromValue<Color>(Color::fromARGB(0xff733D1A)));
    kalamata->setTitle("Kalamata");

    checkBoxContainer->add(limonCello);
    checkBoxContainer->add(greek);
    checkBoxContainer->add(kalamata);

    // The two menu selections are separated by a Divider.
    Divider *divider = new Divider();
    divider->setBottomMargin(ui->du(3));

    Label *fillingLabel = new Label();
    fillingLabel->setText("Filling");
    fillingLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    fillingLabel->textStyle()->setFontWeight(FontWeight::Bold);

    // The RadioGroup is a Control that you can use if only one option can
    // be selected at a time. RadioGroupOptions are added to the Control
    // and by listening for the onSelectedOptionChanged signal, its possible
    // to keep track of what the current selection is.
    RadioGroup *radioGroup = new RadioGroup();
    radioGroup->setDividersVisible(false);

    Option *stoneOption = new Option();
    stoneOption->setText("Stone");
    stoneOption->setSelected(true);

    Option *pimentoOption = new Option();
    pimentoOption->setText("Pimento");

    radioGroup->add(stoneOption);
    radioGroup->add(pimentoOption);

    // We listen for changes in selected options on the RadioGroup.
    connectResult = connect(radioGroup, SIGNAL(selectedIndexChanged(int)), this,
                                        SLOT(fillingSelectedOptionChanged(int)));
    Q_ASSERT(connectResult);

    // Add the controls
    recipeContainer->add(checkBoxContainer);
    recipeContainer->add(divider);
    recipeContainer->add(fillingLabel);
    recipeContainer->add(radioGroup);

    if (UiValues::instance()->device() == UiValues::DEVICETYPE_720X720) {
        // If the device is of 720X720 resolution we make it scrollable.
        ScrollView *scrollRecipe = ScrollView::create()
            .scrollMode(ScrollMode::Vertical)
            .content(recipeContainer);
        setRoot(scrollRecipe);

    } else {
        // The content fits on this screen type so we just set the Container as the root Control.
        setRoot(recipeContainer);
    }
}

void SelectionRecipe::fillingSelectedOptionChanged(int selected)
{
    RadioGroup *radioGroup = dynamic_cast<RadioGroup*>(sender());

    if (radioGroup != 0) {
        // When a new option is selected we print the selection to the console.
        Option *option = radioGroup->at(selected);
        qDebug() << "New filling set: " << option->text();
    }
}

