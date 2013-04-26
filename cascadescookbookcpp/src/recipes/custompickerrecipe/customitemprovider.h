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

#ifndef _PICKERITEMPROVIDER_H_
#define _PICKERITEMPROVIDER_H_


#include <bb/cascades/PickerProvider>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class DataModel;
        class VisualNode;
    }
}

/**
 * CustomItemProvider Description:
 *
 * An implementation of the PickerProvider interface used by the CustomPickerRecipe
 * to populate a picker with custom items. In this implementation we use a data model
 * to determine the configuration of the Picker (it's also possible to take a more direct
 * approach and configure the picker inline in code).
 */
class CustomItemProvider: public bb::cascades::PickerProvider
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the CustomItemProvider control recipe.
     *
     * @param parent A pointer to the parent Container.
     * @param model A model containing the data for populating the picker
     */
    CustomItemProvider(QObject *parent  = 0, DataModel *model = 0);

    /**
     * Destructor;
     */
    virtual ~CustomItemProvider();

    /**
     * creates an empty VisualNode object for the Picker, to be used as a picker item
     *
     * @param pickerList The picker control sending the request.
     * @param The column which the item will be added to.
     */
    VisualNode * createItem(Picker * pickerList, int columnIndex);

    /**
     * Updates an existing picker item for a particular cell in the picker.
     *
     * @param pickerList The picker control sending the request.
     * @param columnIndex The column which the item will be added to.
     * @param rowIndex The item index this node is mapped to on the server.
     * @param pickerItem Pointer to the picker item to update.
     */
    void updateItem(Picker * pickerList, int columnIndex, int rowIndex, VisualNode * pickerItem);

    /**
     * Gets the number of columns for this provider.
     */
    int columnCount() const;

    /**
     * Gets the lower and upper boundaries for the column.
     *
     * @param[in] column The column index.
     * @param[out] lowerBoundary The lower boundary for the column.
     * @param[out] upperBoundary The upper boundary for the column.
     */
    void range (int column, int *lowerBoundary, int *upperBoundary );

private:

    /**
     * An enum used for the item types of this particular provider.
     */
    enum {
        PIZZA_ITEM,
        SIZE_ITEM,
        STYLE_ITEM
    };

    /**
     * Helper function for setting up boundaries of columns and column count
     * based on picker items in a data model.
     */
    void initPickerValues(DataModel *pickerModel);

    /**
     * Creates a pizza picker item.
     *
     * @return a Container with pizza content
     */
    Container *pizzaItem();

    /**
     * Creates a style picker item.
     *
     * @return a Container with style content
     */
    Container *styleItem();

    /**
     * Creates a style size item.
     *
     * @return a Container with pizza content
     */
    Container *sizeItem();

    // A list of the upper range (max index in this case) for the items in each column.
    QVariantList mUpperRanges;

    // The number of columns.
    int mColumnCount;
};

#endif // ifndef _PICKERITEMPROVIDER_H_
