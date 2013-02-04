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

	ScrollView *scrollView = new ScrollView();
	ScrollViewProperties* scrollViewProp = scrollView->scrollViewProperties();
	scrollViewProp->setScrollMode(ScrollMode::Vertical);

    Container *recipeContainer = Container::create().left(80).right(80);

    // Label used to display the entered text
    mInputLabel = new Label();
    mInputLabel->setMultiline(true);
    mInputLabel->setText((const QString) " ");
    mInputLabel->setHorizontalAlignment(HorizontalAlignment::Fill);
    mInputLabel->setBottomMargin(50.0);
    mInputLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());

    // A multi line text input
    TextArea *textArea = new TextArea();
    textArea->setHintText("Enter text into multi-line TextArea");
    textArea->setPreferredHeight(140);
    textArea->setBottomMargin(50.0);
    textArea->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    textArea->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Connect the TextArea textChanging signal to the onTextChanging function to update the text.
    connect(textArea, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));

    // A single line input field with a clear functionality
    TextField *textField = new TextField();
    textField->setHintText("Enter text into a single line TextField");
    textField->setHorizontalAlignment(HorizontalAlignment::Fill);
    textField->setBottomMargin(50.0);

    // Connect the TextField textChanging signal to the onTextChanging function to update the text.
    connect(textField, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));

    // A disabled text field
    TextField *disabledTextField = new TextField();
    disabledTextField->setHintText("This is a disabled text field");
    disabledTextField->setEnabled(false);
    disabledTextField->setHorizontalAlignment(HorizontalAlignment::Fill);
    disabledTextField->setBottomMargin(50.0);

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
