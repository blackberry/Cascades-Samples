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

#include "ListItemFactory.hpp"
#include "ListItem.hpp"

using namespace bb::cascades;

/**
 * This class implements the interface defined by bb::cascades::ListItemManager
 * so that we will be able create list items according to our needs
 */


/**
 * Constructor
 */
ListItemFactory::ListItemFactory()
{
}

/**
 * VisualNode* ListItemFactory::createItem(ListView* list, const QString& type)
 *
 * Interface implementation.
 * create a list item for the given list and item type.
 */
VisualNode* ListItemFactory::createItem(ListView* list, const QString& type)
{
    //We only have one item type so we do not need to check the type variable.
    Q_UNUSED(type);
    Q_UNUSED(list);

    ListItem* listItem = new ListItem();
    return listItem;
}

/**
 * void ListItemFactory::updateItem(ListView* list, bb::cascades::VisualNode* item, const QString& type, const QVariantList& indexPath, const QVariant& data)
 *
 * Interface implementation.
 *
 * update the given item within the given list with the provided information.
 */
void ListItemFactory::updateItem(ListView* list, bb::cascades::VisualNode* item, const QString& type, const QVariantList& indexPath, const QVariant& data)
{
    Q_UNUSED(list);
    Q_UNUSED(indexPath);
    Q_UNUSED(type);

    // Update the control with correct data.
    QVariantMap map = data.value<QVariantMap>();
    ListItem* listItem = static_cast<ListItem*>(item);
    Q_ASSERT(listItem != NULL);
    listItem->updateItem(map["title"].toString(), map["image"].toString());
}
