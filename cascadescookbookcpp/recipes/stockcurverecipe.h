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
 * StockCurveRecipe
 * 
 * This recipe illustrates how the different preset easing curves,
 * "stock curves", effect the timing of the animation (stock in this
 * case being a collection and having nothing to do with the financial market).
 * In the recipe the curves predefined in Cascades 4 is presented in a radio
 * button list to the right. When a new curve is selected an object will animated
 * using the animation curve so that you can see how the different curves behaves.
 */
class StockCurveRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    StockCurveRecipe(Container *parent = 0);

private slots:

    /**
     * Slot function connected to the RadioGroup presenting the different
     * options for ease out animation curves. Will be called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected the newly selected option.
     */
    void outSelectedOptionChanged(int selected);

    /**
     * Slot function connected to the RadioGroup presenting the different
     * options for ease in animation curves. Will be called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected the newly selected option.
     */
    void inSelectedOptionChanged(int selected);

    /**
     * Slot function connected to the RadioGroup presenting the different
     * options for ease inOut animation curves. Will be called when a new
     * selection is made in the group. The function also resets other radio
     * groups in the UI.
     *
     * @param selected the newly selected option.
     */
    void inOutSelectedOptionChanged(int selected);
    
    /**
     * Animation started slot function, used for resetting the animation image.
     */
    void resetEggImage();
    
    /**
     * Animation ended slot function, used for showing an a different image in
     * the animated ImageView in the case an animation curve has been chosen which
     * ends abruptly.
     */
    void breakEgg();

private:
    /**
     * Function that sets up the animation that will illustrate the effect of
     * using different animation curves.
     *
     * @param parent The parent Container to which the animated egg should be
     * @return the ImageView for which the animations have been set up.
     */
    void createAnimatedEgg(Container *parent);

    /**
     * Convenience function for playing the animation with the easing curve
     * stored as a property in the selected RadioGroupOption
     *
     * @param selected the selected option containing information on the
     *        easing curve to be used.
     * @param radioGroup the RadioGroup that the selection was made in.
     */
    void playAnimForOption(int selected, RadioGroup* radioGroup);

    /**
     * The selection of easing curve is done in a panel to the right. The
     * panel has three radio groups one for each ease type (out/in/inout).
     * This function creates the three groups.
     *
     * @return a ScrollView with three RadioGroups with the different easing curves as options.
     */
    ScrollView *setUpStockCurveSelectionPanel();

    /**
     * Set up function for the easing curve RadioGroups. A radio group
     * with radio groups set up from the list in the input parameters will
     * be created. Each RadioGroupOption store the curve and
     * name in properties that are used when triggering the animation
     * on a new selection.
     *
     * @param radioGroup the RadioGroup class member that is going to be created
     * @param title  a title that is put on top of the radioGroup.
     * @param curves the StockCurves.
     * @param names  the names of the StockCurves as QStrings.
     * @param slot   the slot function to which the radioGroup should connect.
     *
     * @return A Container with a RadioGroup and a title on top.
     */
    Container *setUpRadioGroup(RadioGroup **radioGroup, QString const &title, QList<QString> names, QList<StockCurve> curves, const char *slot);


    // The description label presenting the name of the current StockCurve being used.
    Label *mStockCurveDescription;

    // The animation that contain both the reset and the easing animation.
    SequentialAnimation *mAnim;
    TranslateTransition *mEaseAnim;

    // The ImageView that will be animated.
    ImageView *mEgg;

    // The three radio groups.
    RadioGroup *mRadioGroupOut;
    RadioGroup *mRadioGroupIn;
    RadioGroup *mRadioGroupInOut;
};

#endif // ifndef _STOCKCURVERECIPE_H_
