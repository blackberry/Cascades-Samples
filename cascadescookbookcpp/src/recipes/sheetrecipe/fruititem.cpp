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
#include "fruititem.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>

using namespace bb::cascades;

FruitItem::FruitItem(Container *parent) :
        CustomControl(parent)
{
    // Dock layout with margins inside
    Container *itemContainer = Container::create().layout(DockLayout::create())
                               .background(Color::fromARGB(0xfff0f0f0));

    // A Colored Container will be used to show if an item is highlighted
    mHighlighContainer = Container::create().background(Color::fromARGB(0xff75b5d3)).opacity(0.0);
    mHighlighContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    mHighlighContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // The list item image, the actual image is set in updateItem
    mItemImage = ImageView::create("asset:///images/white_photo.png");
    mItemImage->setHorizontalAlignment(HorizontalAlignment::Center);
    mItemImage->setVerticalAlignment(VerticalAlignment::Center);

    // Add the the content to the full item container.
    itemContainer->add(mHighlighContainer);
    itemContainer->add(mItemImage);

    setRoot(itemContainer);
}

void FruitItem::updateItem(const QString imagePath)
{
    // Update image and text for the current item.
    mItemImage->setImage(Image(imagePath));
}

void FruitItem::select(bool select)
{
    // When an item is selected, show the colored highlight Container.
    if (select) {
        mHighlighContainer->setOpacity(0.9f);
    } else {
        mHighlighContainer->setOpacity(0.0f);
    }
}

void FruitItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    // Since items are recycled the reset function is where we have
    // to make sure that item state, defined by the arguments, is correct.
    select(selected);
}

void FruitItem::activate(bool activate)
{
    // There is no special activate state, select and activate looks the same.
    select(activate);
}
