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

#include "ListItem.hpp"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

/**
 * This Class implements its counterpart described by "common/ListItem.qml"
 * e.g.: demonstrate how to implement a custom List Item by C++.
 */


/**
 * Constructor
 *
 * Construct the UI of the list item
 */

ListItem::ListItem(Container* parent) :
    CustomControl(parent)
{
    // Dock layout with margins inside.
    Container* itemContainer = new Container();
    DockLayout* itemLayout = new DockLayout();
    itemContainer->setLayout(itemLayout);
    itemContainer->setPreferredWidth(250.0f);

    // A background Container that will hold a background image and an item content Container.
    Container* backgroundContainer = new Container();
    DockLayout* backgroundLayout = new DockLayout();
    backgroundContainer->setLayout(backgroundLayout);
    backgroundContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    backgroundContainer->setVerticalAlignment(VerticalAlignment::Center);

    // A Colored Container will be used to show if an item is highlighted.
    m_highlighContainer = new Container();
    m_highlighContainer->setBackground(Color::fromARGB(0xff75b5d3));
    m_highlighContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    m_highlighContainer->setVerticalAlignment(VerticalAlignment::Center);
    m_highlighContainer->setOpacity(0.0);
    m_highlighContainer->setPreferredWidth(242.0f);
    m_highlighContainer->setPreferredHeight(168.0f);

    // Content Container, Image + text with padding to get alignment on background image.
    Container* contentContainer = new Container();
    DockLayout* contentLayout = new DockLayout();
    contentContainer->setLayout(contentLayout);
    contentContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    contentContainer->setVerticalAlignment(VerticalAlignment::Center);

    // The list item image, docked to the top, the actual image is set in updateItem.
    m_itemImage = ImageView::create("asset:///images/white_photo.png").preferredSize(238.0f, 160.0f);
    m_itemImage->setHorizontalAlignment(HorizontalAlignment::Center);
    m_itemImage->setVerticalAlignment(VerticalAlignment::Center);

    // A list item label, docked to the center, the text is set in updateItem.
    m_itemLabel = Label::create().text(" ");
    m_itemLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    m_itemLabel->setVerticalAlignment(VerticalAlignment::Center);
    m_itemLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    m_itemLabel->textStyle()->setColor(Color::Black);

    // Add the Label to the content.
    contentContainer->add(m_itemLabel);

    // Add the background image and the content to the background item container.
    backgroundContainer->add(m_itemImage);
    backgroundContainer->add(m_highlighContainer);
    backgroundContainer->add(contentContainer);

    // Finally add the background Container to the item Container.
    itemContainer->add(backgroundContainer);

    setRoot(itemContainer);
}

/**
 * void ListItem::updateItem(const QString text, const QString imagePath)
 *
 * Update the background image and the text for this item
 */
void ListItem::updateItem(const QString text, const QString imagePath)
{
    // Update image and text for the current item.
    m_itemImage->setImage(Image(imagePath));
    m_itemLabel->setText(text);
}

/**
 * void ListItem::select(bool select)
 *
 * Change the look and feel of this item depending the selection state
 */
void ListItem::select(bool select)
{
    // When an item is selected show the colored highlight Container-�,
    if (select)
    {
        m_highlighContainer->setOpacity(0.9f);
    }
    else
    {
        m_highlighContainer->setOpacity(0.0f);
    }
}

/**
 * void ListItem::reset(bool selected, bool activated)
 *
 * Reset the item to the given state
 */
void ListItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    // Since items are recycled the reset function is where we have
    // to make sure that item state, defined by the arguments, is correct.
    select(selected);
}

/**
 * void ListItem::activate(bool activate)
 *
 * set the item to the given activated state
 */
void ListItem::activate(bool activate)
{
    // There is no special activate state, select and activated looks the same.
    select(activate);
}
