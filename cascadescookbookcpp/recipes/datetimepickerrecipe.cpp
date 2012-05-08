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
#include "datetimepickerrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/DateTimePicker>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

DateTimePickerRecipe::DateTimePickerRecipe(Container *parent) :
CustomControl(parent)
{
    // The recipe Container.
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(StackLayout::create().top(50.0f));
    recipeContainer->setMinHeight(1024.0f);

    // The recipe title, we add it to a Container in order to get some padding around it.
    Container *titleContainer = new Container();
    titleContainer->setLayout(StackLayout::create().left(20.0f).bottom(20.0f));
    QDateTime date = QDateTime::currentDateTime();
    Label *title = new Label(titleContainer);
    title->setText("Today is: " + date.toString("M/d/yy"));
    title->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    title->textStyle()->setColor(Color::White) ;


    // This is where we create the DateTimePicker, we set the mode so that we 
    // can change the date and update the state of the fruit depending on that.
    DateTimePicker *datePicker = new DateTimePicker();
    datePicker->setTitle("Banana at date:");
    datePicker->setMode(DateTimePickerMode::Date);
    connect(datePicker, SIGNAL(valueChanged(QDateTime )), this, SLOT(onValueChanged(QDateTime )));

    // An image of a fruit is used to show how one can use the QDateTime value 
    // reported by the picker.
    Container *fruitContainer = new Container();
    fruitContainer->setTopMargin(20.0f);
    fruitContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    mTimeWarpFruit = ImageView::create("asset:///images/picker/banana_new.png");
    mTimeWarpFruit->setPreferredSize(768.0f, 512.0f);
    fruitContainer->add(mTimeWarpFruit);

    recipeContainer->add(titleContainer);
    recipeContainer->add(datePicker);
    recipeContainer->add(fruitContainer);

    setRoot(recipeContainer);
}

void DateTimePickerRecipe::onValueChanged(QDateTime value)
{
    QDateTime today = QDateTime::currentDateTime();

    // We compare the date that is set on the Picker to the current date and set different
    // images depending on how far in the future (or past) that date is.
    if (value < today.addDays(-1)) {
        mTimeWarpFruit->setImageSource(QUrl("asset:///images/picker/banana_past.png"));
    } else if (value >= today.addDays(-1) && value <= today.addDays(3)) {
        mTimeWarpFruit->setImageSource(QUrl("asset:///images/picker/banana_new.png"));
    } else if ( value < today.addMonths(1) ) {
        mTimeWarpFruit->setImageSource(QUrl("asset:///images/picker/banana_old.png"));
    } else {
        mTimeWarpFruit->setImageSource(QUrl("asset:///images/picker/banana_ancient.png"));
    }
}
