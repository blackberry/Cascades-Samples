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
#include "customitemprovider.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DataModel>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/Picker>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/VisualNode>

using namespace bb::cascades;

CustomItemProvider::CustomItemProvider(QObject *parent, DataModel *model) :
        PickerProvider(parent)
{
    // The item provider reports how many columns the Picker has and also what
    // the ranges are of the columns. In this recipe we get this values from a model,
    // but it is possible to set it up in other fashions as well.
    initPickerValues(model);
}

CustomItemProvider::~CustomItemProvider()
{
}

VisualNode * CustomItemProvider::createItem(Picker * pickerList, int columnIndex)
{
    Q_UNUSED(pickerList)
    Container *returnItem;

    switch (columnIndex) {
        case PIZZA_ITEM:
            returnItem = pizzaItem();
            break;
        case SIZE_ITEM:
            returnItem = sizeItem();
            break;
        case STYLE_ITEM:
            returnItem = styleItem();
            break;
        default:
            returnItem = Container::create();
            break;
    }

    return returnItem;
}

void CustomItemProvider::updateItem(Picker * pickerList, int columnIndex, int rowIndex,
        VisualNode * pickerItem)
{
    DataModel *model = pickerList->dataModel();
    QVariantList indexPath;
    indexPath << columnIndex << rowIndex;

    // When
    if (model->data(indexPath).canConvert(QVariant::Map)) {
        QVariantMap dataMap = model->data(indexPath).toMap();

        // Find the item label for which the text will be updated to correspond to the model value.
        Label * itemLabel = pickerItem->findChild<Label *>("itemLabel");
        Label * sliceLabel = pickerItem->findChild<Label *>("sliceLabel");

        switch (columnIndex) {
            case PIZZA_ITEM:
                itemLabel->setText(dataMap["text"].toString());
                break;
            case STYLE_ITEM:
                itemLabel->setText(dataMap["text"].toString());
                break;
            case SIZE_ITEM:
                itemLabel->setText(dataMap["text"].toString());
                sliceLabel->setText("x" + dataMap["slices"].toString());
                break;
        }
    }
}

int CustomItemProvider::columnCount() const
{
    return mColumnCount;
}

void CustomItemProvider::range(int column, int* lowerBoundary, int* upperBoundary)
{
    // We set the lower boundary at 0 for all columns, the upper boundary is read from
    // a model in during the creation of the provider.
    *lowerBoundary = 0;
    *upperBoundary = mUpperRanges.at(column).toInt();
}

void CustomItemProvider::initPickerValues(DataModel* pickerModel)
{
    QVariantList indexPath;
    indexPath << 0;

    // Get the number of columns by checking the child count of the models root element.
    mColumnCount = pickerModel->childCount(QVariantList());

    // The column range correspond to the child count of the models column data elements,
    // this information is reported back in the range function above.
    for (int i = 0; i < mColumnCount; i++) {
        indexPath.replace(0, i);
        mUpperRanges.insert(i, pickerModel->childCount(indexPath) - 1);
    }
}

Container *CustomItemProvider::pizzaItem()
{
    Container *content = Container::create().layout(new DockLayout());

    Label * itemLabel = Label::create().objectName("itemLabel")
            .multiline(true)
            .vertical(VerticalAlignment::Center)
            .horizontal(HorizontalAlignment::Center);
    itemLabel->textStyle()->setBase(SystemDefaults::TextStyles::subtitleText());
    itemLabel->setMaxWidth(250);

    content->add(itemLabel);
    return content;
}

Container *CustomItemProvider::sizeItem()
{
    Container *content =
            Container::create().layout(new DockLayout()).top(5).left(15).right(15).bottom(5);

    Label * itemLabel = Label::create().objectName("itemLabel")
                .vertical(VerticalAlignment::Center)
                .horizontal(HorizontalAlignment::Left);
    itemLabel->textStyle()->setBase(SystemDefaults::TextStyles::subtitleText());

    Label * sliceLabel = Label::create().objectName("sliceLabel")
                .vertical(VerticalAlignment::Bottom)
                .horizontal(HorizontalAlignment::Right);
    sliceLabel->textStyle()->setBase(SystemDefaults::TextStyles::subtitleText());

    ImageView * sliceImage = ImageView::create("asset:///images/custompicker/sliceImage.png")
                .vertical(VerticalAlignment::Bottom)
                .horizontal(HorizontalAlignment::Right);
    sliceImage->setScaleX(0.75f);
    sliceImage->setScaleY(0.75f);
    sliceImage->setRotationZ(15);

    content->add(itemLabel);
    content->add(sliceImage);
    content->add(sliceLabel);

    return content;
}

Container *CustomItemProvider::styleItem()
{
    Container *content = Container::create().layout(new DockLayout());

    Label * itemLabel = Label::create().objectName("itemLabel")
            .vertical(VerticalAlignment::Center)
            .horizontal(HorizontalAlignment::Center);

    itemLabel->textStyle()->setBase(SystemDefaults::TextStyles::subtitleText());

    content->add(itemLabel);
    return content;
}

