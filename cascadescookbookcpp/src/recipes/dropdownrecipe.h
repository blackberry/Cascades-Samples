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
#ifndef _DROPDOWNRECIPE_H_
#define _DROPDOWNRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class DropDown;
        class ImageView;
        class Label;
    }
}

/* DropDownRecipe Description:
 *
 * A recipe showing how the DropDown Control is used to make a selection.
 * In this case, different selections result in a recipe for delicious
 * beer.
 */
class DropDownRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the DropDown recipe.
     * @param parent A pointer to the parent Container.
     */
    DropDownRecipe(Container *parent = 0);

private slots:
    /**
     * This Slot function is used when connecting to the selectedIndexChanged
     * signal for the DropDown Control.
     *
     * @param selectedIndex The index of newly selected option
     */
    void onSelectedIndexChanged(int selectedIndex);

private:

    /**
     * This Slot function updates the amount of different ingredients based
     * on the selection in the DropDown Control.
     *
     * @param dropDown A pointer the DropDown Control
     */
    void updateAmounts(DropDown *dropDown);

    /**
     * Helper function for setting up the TextAreas for the beer recipe.
     *
     * @return The Container containing the beer recipe TextAreas
     */
    Container *createBeerRecipe();

    // The TextArea that will be updated depending on the selection in DropDown
    Label *mAmounts;

    // The beer image for which the image will change depending on the selection
    ImageView *mBeers;
};

#endif // ifndef _DROPDOWNRECIPE_H_
