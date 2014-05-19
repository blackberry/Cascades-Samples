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
#include "inputrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextField>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ScrollView>

using namespace bb::cascades;

InputRecipe::InputRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    ScrollView *scrollView = new ScrollView();
    ScrollViewProperties* scrollViewProp = scrollView->scrollViewProperties();
    scrollViewProp->setScrollMode(ScrollMode::Vertical);

    Container *recipeContainer = Container::create().top(ui->du(3)).left(ui->du(2)).right(ui->du(2));

    // Label used to display the entered text
    mInputLabel = new Label();
    mInputLabel->setMultiline(true);
    mInputLabel->setHorizontalAlignment(HorizontalAlignment::Fill);

    // A multi line text input
    TextArea *textArea = new TextArea();
    textArea->setHintText("Enter text into multi-line TextArea");
    textArea->setPreferredHeight(ui->du(14));

    // Connect the TextArea textChanging signal to the onTextChanging function to update the text.
    connectResult = connect(textArea, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));
    Q_ASSERT(connectResult);

    // A single line input field with a clear functionality
    TextField *textField = new TextField();
    textField->setHintText("Enter text into a single line TextField");

    // Setting the keyboards input layout to text but there is a variety to choose from
    textField->input()->setKeyLayout(KeyLayout::Text);


    // Connect the TextField textChanging signal to the onTextChanging function to update the text.
    connectResult = connect(textField, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));
    Q_ASSERT(connectResult);

    // A disabled text field
    TextField *disabledTextField = new TextField();
    disabledTextField->setHintText("This is a disabled text field");
    disabledTextField->setEnabled(false);

    // Add the controls to the recipe Container and ScrollView and set it as the CustomControl root.
    scrollView->setContent(recipeContainer);
    recipeContainer->add(mInputLabel);
    recipeContainer->add(textField);
    recipeContainer->add(disabledTextField);
    recipeContainer->add(textArea);

    //recipeContainer->add(inputContainer);
    setRoot(scrollView);
}

void InputRecipe::onTextChanging(const QString &newText)
{
    // Update the label text as text is being entered.
    mInputLabel->setText(newText);
}
