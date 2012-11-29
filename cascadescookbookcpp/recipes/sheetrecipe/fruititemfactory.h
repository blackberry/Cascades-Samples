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
#ifndef _FRUITITEMFACTORY_H_
#define _FRUITITEMFACTORY_H_

#include <bb/cascades/ListItemProvider>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ListView;
    }
}

/**
 * FruitItemFactory Description:
 *
 * Item manager for the sheet recipe, provides list items for selection
 * a new fruit in the second level drill down sheet navigation.
 * An item manager supplies item Controls for a list and updates
 * the contents of the items based on the data supplied in the list model.
 */
class FruitItemFactory: public bb::cascades::ListItemProvider
{
public:
    FruitItemFactory();

    /**
     * Creates a VisualNode for list to be used as a list item.
     *
     * @param list ListView for which the item should be created.
     * @param type List item type, the cookbook only have one type of item.
     * @return A VisualNode pointer to the created item.
     */
    VisualNode * createItem(ListView* list, const QString &type);

    /**
     * Updates a listItem based on the provided type, indexPath, and data.
     *
     * This function is called whenever an item is about to be shown, and also
     * when the data representation of the item (in the DataModel) has changed.
     *
     * @param list ListView holding the item to be updated.
     * @param listItem The List item to be updated.
     * @param type List item type, the list only have one type of item
     * @param indexPath Index path to the item that is to be updated.
     * @param data Data from the DataModel that corresponds to listItem.
     */
    void updateItem(ListView* list, VisualNode *listItem, const QString &type,
            const QVariantList &indexPath, const QVariant &data);
};

#endif // ifndef _FRUITITEMFACTORY_H_
