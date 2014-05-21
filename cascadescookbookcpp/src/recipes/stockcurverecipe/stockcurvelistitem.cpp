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
#include "stockcurvelistitem.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>

using namespace bb::cascades;

StockCurveListItem::StockCurveListItem(Container *parent) :
        CustomListItem(HighlightAppearance::None, parent)
{
    setDividerVisible(false);

    // Dock layout with margins inside
    mItemContainer = Container::create().layout(DockLayout::create())
                                    .horizontal(HorizontalAlignment::Fill)
                                    .vertical(VerticalAlignment::Fill)
                                    .background(Color::Transparent);

    // The list item image, the actual image is set in updateItem
    mItemImage = ImageView::create();
    mItemImage->setHorizontalAlignment(HorizontalAlignment::Center);
    mItemImage->setVerticalAlignment(VerticalAlignment::Center);

    // Add the the content to the full item container.
    mItemContainer->add(mItemImage);
    setContent(mItemContainer);
}

void StockCurveListItem::updateItem(const QString imagePath)
{
    QString imageSource = "asset:///images/stockcurve/icon_" + imagePath + ".png";
    // Update image and text for the current item.
    mItemImage->setImageSource(imageSource);
}

void StockCurveListItem::select(bool select)
{
    // When an item is selected, show the colored highlight Container.
    if (select) {
        mItemContainer->setBackground(Color::fromARGB(0x9975b5d3));
    } else {
        mItemContainer->setBackground(Color::Transparent);
    }
}

void StockCurveListItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    // Since items are recycled the reset function is where we have
    // to make sure that item state, defined by the arguments, is correct.
    select(selected);
}

void StockCurveListItem::activate(bool activate)
{
    // There is no special activate state, select and activate looks the same.
    //select(activate);
}
