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
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/TextField>
#include <bb/cascades/TextArea>

using namespace bb::cascades;

InputRecipe::InputRecipe(Container *parent) :
        CustomControl(parent)
{

    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeLayout->setLeftPadding(80);
    recipeLayout->setRightPadding(80);

    // Label used to display the entered text.
    mInputLabel = new Label();
    mInputLabel->setText((const QString) " ");
    mInputLabel->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));
    mInputLabel->setBottomMargin(50.0);
    mInputLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mInputLabel->textStyle()->setColor(Color::Gray) ;
    
    // A multi line text input.
    TextArea *textArea = new TextArea();
    textArea->setHintText("Enter text into multiline TextArea");
    textArea->setMinHeight(120.0f);
    textArea->setMaxHeight(200.0f);
    textArea->setBottomMargin(50.0);
    textArea->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    textArea->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));


    // Connect to the textChanged (to update text).
    QObject::connect(textArea, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));

    // A single line input field with a clear functionality.
    TextField *textField = new TextField();
    textField->setHintText("Enter text into a single line TextField");
    textField->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));
    textField->setBottomMargin(50.0);

    // Connect to the textChanged (to update text).
    QObject::connect(textField, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));

    // A disabled text field.
    TextField *disabledTextField = new TextField();
    disabledTextField->setHintText("This is a disabled text field");
    disabledTextField->setEnabled(false);
    disabledTextField->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    // Add the controls to the recipe Container and set it as the CustomControl root.
    recipeContainer->add(mInputLabel);
    recipeContainer->add(textArea);
    recipeContainer->add(textField);
    recipeContainer->add(disabledTextField);

    //recipeContainer->add(inputContainer);
    setRoot(recipeContainer);
}

void InputRecipe::onTextChanging(const QString &newText)
{
    // Update the label text as text is being entered.
    mInputLabel->setText(newText);
}
