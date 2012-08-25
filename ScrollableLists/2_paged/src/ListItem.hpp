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

#ifndef LISTITEM_HPP_
#define LISTITEM_HPP_

#include <bb/cascades/CustomControl>
#include <bb/cascades/ListItemListener>

using namespace bb::cascades;

namespace bb
{
namespace cascades
{
class ImageView;
class Label;
class Container;
}
}

/**
 * ListItem
 *
 * For the list a custom item is used, the item has a background image,
 * a specific item image. Data is updated via the updateItem function
 * and the focus handling is achieved by implementing the ListItemListener interface.
 */
class ListItem: public bb::cascades::CustomControl, public ListItemListener
{
    Q_OBJECT

public:
    ListItem(Container* parent = 0);

    /**
     * This function updates the data of the item.
     *
     * @param text the label text.
     * @param imagePath the path to the image asset used for the item.
     */
    void updateItem(const QString text, const QString imagePath);

    /**
     * ListItemListener interface function called when the select state changes.
     *
     * @param select true if the item has been selected, false if unselected
     */
    void select(bool select);

    /**
     * ListItemListener interface function called when an item needs to be reset.
     * Since items are recycled the reset function is where we have
     * to make sure that item state, defined by the arguments, is correct.
     *
     * @param selected true if the item should appear selected, false if unselected
     * @param activated true if the item should appear activated, false if deactivated
     */
    void reset(bool selected, bool activated);

    /**
     * ListItemListener interface called when an item is activated/deactivated.
     *
     * @param activate true if the item has been activated, false if inactive.
     */
    void activate(bool activate);

private:

    // Item Controls.
    ImageView* m_itemImage;
    Label* m_itemLabel;
    Container* m_highlighContainer;
};

#endif /* LISTITEM_HPP_ */
