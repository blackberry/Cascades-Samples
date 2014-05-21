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

#ifndef _SHEETRECIPE_H_
#define _SHEETRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ImageView;
        class Label;
        class Sheet;
        class TapEvent;
    }
}

/* SheetRecipe Description:
 *
 * In the Sheet recipe, a sheet is primarily used to present UI concerned with
 * creating or selection of content. Sheets are also typically used
 * when changing context to a temporary sub-flow that contains a more complex entry
 * or an entry that has more than one step (more than one screen).
 * In this recipe, this is illustrated by a bowl of fruit. The content of the bowl
 * can be modified and a drill-down flow consisting of two Sheets is shown.
 */
class SheetRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    SheetRecipe(Container *parent = 0);

public slots:

    /**
     * Slot function for tap events on the recipe page (the main page). When
     * events are received, the UI flow for modifying the content of the
     * recipe page is initiated.
     *
     * @param tapEvent Unused in this case
     */
    void onModifyRecipe(bb::cascades::TapEvent* tapEvent);

    /**
     * Slot function for tap events on modification Sheet. When events are received,
     * a second sheet for selecting a new fruit is displayed on top of the
     * current sheet.
     *
     * @param tapEvent Unused in this case
     */
    void onNewFruitTap(bb::cascades::TapEvent* tapEvent);

    /**
     * Slot function for interaction with the modify-Sheets save Button.
     */
    void onModifySave();

    /**
     * Slot function for interaction with the modify-Sheets cancel Button.
     */
    void onModifyCancel();

    /**
     * Slot function for interaction with the fruit selection-Sheets cancel Button.
     */
    void onNewFruitCancel();

    /**
     * Slot function for selection in the fruit-list second sheet of
     * the Sheet navigation. This is where altering the content of the fruit bowl
     * in the recipe is taken care of.
     *
     * @param indexPath Index path to the item.
     * @param active True if the new state is selected, otherwise false.
     */
    void onNewFruitChanged(const QVariantList indexPath);

    /**
     * Slot function for the modification-sheet text-input field, used
     * for changing the greetings text on the recipe page.
     *
     * @param newText the current text in the text field
     */
    void onTextChanging(const QString &newText);

private:

    /**
     * Set up function for the UI of the first recipe Page, the starting point
     * of the Sheet flow.
     *
     * @return The root Container that contains the UI content of the Page.
     */
    Container *setUpRecipeContent();

    /**
     * Sets up a Sheet used for modifying the content on the recipe Page. This is the
     * first Sheet in the drill-down navigation from the Sheet recipe.
     */
    void setUpModifySheet();

    /**
     * Sets up a sheet containing a list of fruits to select from. This is the
     * second Sheet in the drill-down navigation from the Sheet recipe.
     */
    void setUpFruitSheet();

    // The two sheets
    Sheet *mSheetModify;
    Sheet *mSheetFruit;

    // UI elements used for updating based on the interaction in the Sheets
    ImageView *mFruitImage;
    ImageView *mModifyFruitImage;
    Label *mGreetingsLabel;
    QString mGreetingsText;
};

#endif // ifndef _SHEETRECIPE_H_
