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
#ifndef _STOCKCURVERECIPE_H_
#define _STOCKCURVERECIPE_H_

#include "uivalues.h"
#include <bb/cascades/CustomControl>
#include <bb/cascades/StockCurve>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class ImageView;
        class Label;
        class RadioGroup;
        class Option;
        class SequentialAnimation;
        class TranslateTransition;
        class ScrollView;
    }
}

/** 
 * StockCurveRecipe Description:
 * 
 * This recipe illustrates how the different preset easing curves,
 * "stock curves", effect the timing of the animation (stock, in this
 * case, being a collection and having nothing to do with the financial market).
 * In the recipe, the curves predefined in Cascades are presented in a radio
 * button list to the right. When a new curve is selected, an object will animated
 * using the animation curve so that you can see how the different curves behave.
 */
class StockCurveRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the stock curve recipe
     * @param parent The parent Container, if not specified, 0 is used
     */
    StockCurveRecipe(Container *parent = 0);

private slots:

    /**
     * This Slot function is connected to the RadioGroup presenting the different
     * options for ease "out" animation curves. This function is called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected The newly selected option.
     */
    void outSelectedOptionChanged(int selected);

    /**
     * This Slot function is connected to the RadioGroup presenting the different
     * options for ease "in" animation curves. This function is called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected The newly selected option.
     */
    void inSelectedOptionChanged(int selected);

    /**
     * This Slot function is connected to the RadioGroup presenting the different
     * options for ease "inOut" animation curves. This function is called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected The newly selected option.
     */
    void inOutSelectedOptionChanged(int selected);

    /**
     * Animation-started slot function used for resetting the animation image.
     */
    void resetEggImage();

    /**
     * Animation-ended slot function used for showing a different image in
     * the animated ImageView. In the case, an animation curve has been chosen which
     * ends abruptly.
     */
    void breakEgg();

private:
    /**
     * This function sets up the animation that will illustrate the effect of
     * using different animation curves.
     *
     * @param parent The parent Container to which the animated egg should be
     */
    void createAnimatedEgg(Container *parent);

    /**
     * This convenience function is for playing the animation with the easing curve
     * stored as a property in the selected RadioGroupOption
     *
     * @param selected The selected option containing information on the
     *        easing curve to be used.
     * @param radioGroup The RadioGroup that the selection was made in.
     */
    void playAnimForOption(int selected, RadioGroup* radioGroup);

    /**
     * The selection of easing curve is done in a panel to the right. The
     * panel has three radio groups one for each ease type (out/in/inout).
     * This function creates the three groups.
     *
     * @return A ScrollView with three RadioGroups with the different easing curves as options.
     */
    ScrollView *setUpStockCurveSelectionPanel();

    /**
     * This setup function is for the easing curve RadioGroups. A radio group
     * with other radio groups is set up from the list of input parameters.
     * Each RadioGroupOption stores the curve and name in its properties that
     * is used when triggering the animation on a new selection.
     *
     * @param radioGroup The RadioGroup class member that is going to be created
     * @param title A title that is put on top of the radioGroup.
     * @param curves The StockCurves
     * @param names The names of the StockCurves as QStrings.
     * @param slot The slot function to which the radioGroup should connect.
     *
     * @return A Container with a RadioGroup and a title on top.
     */
    Container *setUpRadioGroup(RadioGroup **radioGroup, QString const &title, QList<QString> names,
            QList<StockCurve> curves, const char *slot);

    // The description label presenting the name of the current StockCurve being used
    Label *mStockCurveDescription;

    // The animation that contains both the reset and the easing animation
    SequentialAnimation *mAnim;
    TranslateTransition *mEaseAnim;

    // The ImageView that will be animated
    ImageView *mEgg;

    // The three radio groups
    RadioGroup *mRadioGroupOut;
    RadioGroup *mRadioGroupIn;
    RadioGroup *mRadioGroupInOut;

    // The UI values object is used to access pixel values for the UI.
    UiValues *mUiValues;
};

#endif // ifndef _STOCKCURVERECIPE_H_
