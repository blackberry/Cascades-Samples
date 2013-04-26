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
#include "custompickerrecipe.h"
#include "customitemprovider.h"
#include "uivalues.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/Picker>
#include <bb/cascades/PickerProvider>
#include <bb/cascades/XmlDataModel>

using namespace bb::cascades;

CustomPickerRecipe::CustomPickerRecipe(Container *parent) :
        CustomControl(parent)
{

    // The recipe Container.
    int standardPadding = UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD);
    Container *recipeContainer = Container::create().top(35.0f).left(standardPadding)
                                            	.right(standardPadding).bottom(standardPadding);
    recipeContainer->setMinHeight(UiValues::instance()->intValue(UiValues::SCREEN_HEIGHT));

    // A title label that will be updated depending on the picker selection.
    mDescription = Label::create("label");

    // A label to show the price
    mBill = Label::create("Bill:");

    // The custom picker its initiated in its expanded state.
    mPicker = Picker::create().title("Custom Pizza Picker").expanded(true);

    // An XMLModel is used for populating the different picker items, this
    // kind of data model is suitable for small lists with static content.
    XmlDataModel *pickerModel = new XmlDataModel(this);
    pickerModel->setSource(QUrl("models/custompickermodel.xml"));
    mPicker->setDataModel(pickerModel);

    // The items in the picker columns are created and handled by an PickerItemProvider,
    // see customprovider.h/cpp for details.
    CustomItemProvider *itemProvider = new CustomItemProvider(this, mPicker->dataModel());
    mPicker->setPickerItemProvider(itemProvider);

    // Finally the picker signal is connected to a slot function, so the title label
    // text can be set according to the selection in the picker.
    connect(mPicker, SIGNAL(selectedValueChanged(const QVariant )),
            SLOT(onValueChanged(const QVariant )));

    // Add the label and the picker to the recipe Container.
    recipeContainer->add(mDescription);
    recipeContainer->add(mBill);
    recipeContainer->add(mPicker);

    setRoot(recipeContainer);
}

void CustomPickerRecipe::onValueChanged(const QVariant &value)
{
    qDebug() << "value " << value;

    if (value.canConvert(QVariant::List)) {
        QVariantList selectionList = value.toList();
        DataModel *pickerModel = mPicker->dataModel();

        // Get the current pizza type data
        QVariantList pizzaIndexPath;
        pizzaIndexPath << 0 << selectionList.at(0).toInt();
        QVariantMap pizza = pickerModel->data(pizzaIndexPath).toMap();

        // Get the current size item data
        QVariantList sizeIndexPath;
        sizeIndexPath << 1 << selectionList.at(1).toInt();
        QVariantMap size = pickerModel->data(sizeIndexPath).toMap();

        // Get the current extra item data
        QVariantList extraIndexPath;
        extraIndexPath << 2 << selectionList.at(2).toInt();
        QVariantMap extra = pickerModel->data(extraIndexPath).toMap();

        // Total amount to pay for the pizza.
        float price = pizza["price"].toFloat() * size["factor"].toFloat() + extra["price"].toFloat();

        // Update the description label with summary of the order.
        mDescription->setText(
        		"Order: " + pizza["text"].toString() + ", " + size["text"].toString() + ", "
                        + extra["text"].toString() +".");

        // Update the bill label with the total amount to pay for the order.
        mBill->setText(
        		"Bill:  " + QString::number(price) + "$");

        // and set the picker description to the total price
        mPicker->setDescription(QString::number(price) + " $");
    }
}
