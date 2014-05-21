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
#ifndef _GRIDLAYOUTRECIPE_H_
#define _GRIDLAYOUTRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class GridLayout;
        class HorizontalAlignment;
        class Option;
    }
}

/**
 * GridLayoutRecipe Description:
 *
 * A recipe showing the Container GridLayout in action. A GridLayout
 * has a coloumCount and fills row by row the content put into it.
 */
class GridLayoutRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the grid layout recipe.
     *
     * @param parent A pointer to the parent Container, @c if not specified, 0 is used
     */
    GridLayoutRecipe(Container *parent = 0);

private slots:
    /**
     * A segmented control is connected to this function, so that
     * the number of columns in the bookshelf GridLayout can be changed.
     *
     * @param option The selected Option that holds the new columnCount as a value.
     */
    void onColumnCountChanged(bb::cascades::Option *option);

private:
    /**
     * Function that creates the bookshelf Container
     *
     * @return A Container with the bookshelf UI.
     */
    Container *createBookshelf();

    /**
     * Function that creates a bookshelf cell for the GridLayout
     * in the book shelf Container
     *
     * @return A Container representing a cell in the bookshelf UI.
     */
    Container *createBookshelfCell();

    /**
     * Function that adds content to the book shelf
     *
     * @param shelfContainer The shelf Container with a GridLayout
     * @param imageSource The path to the image that should be added
     * @param cellIndex The index of the cell to which to add content
     * @param hoizontalAlignment the alignment of the ImageView that is added
     */
    void addImageToCellInShelf(Container* shelfContainer, const QString imageSource, int cellIndex, HorizontalAlignment::Type horizontalAlignment);

    /**
     * Function that creates the control panel making it possible
     * to modify the number of columns in the GridLayout of the
     * book shelf.
     *
     * @return A Container with controller UI.
     */
    Container *createControlPanel();

    // The GridLayout of the book shelf Container.
    GridLayout *mShelfLayout;
};

#endif // ifndef _GRIDLAYOUTRECIPE_H_
