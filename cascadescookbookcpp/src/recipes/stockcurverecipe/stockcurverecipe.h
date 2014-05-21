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
        class SequentialAnimation;
        class TranslateTransition;
        class StockCurve;
    }
}

class StockcurveList;

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
     * Animation-started slot function used for resetting the animation image.
     */
    void resetEggImage();

    /**
     * Animation-ended slot function used for showing a different image in
     * the animated ImageView. In the case, an animation curve has been chosen which
     * ends abruptly.
     */
    void breakEgg();

    /**
     * Layout signal handler for finding the size of the screen, so that the
     * proper animation distance can be set.
     *
     * @param layoutRect The QRectF containing layout information.
     */
    void screenLayoutFrameUpdated(QRectF layoutRect);

    /**
     * This function is called when an item in one of the three
     * StockcurveList is tapped, the item will be selected and the
     * other lists deselected.
     *
     * @param indexPath The index path to the triggered item.
     */
    void itemTriggered(QVariantList indexPath);

private:
    /**
     * Creates and returns a QVariantMap representing the data for a
     * stockcurve item in the StockcurveList.
     *
     * @param name A QString representing the name of the curve for presentation.
     * @param curve The actual StockCurve.
     */
    QVariantMap mapForNameAndCurve(const QString name, StockCurve curve);

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
    void playAnim(const QString name, StockCurve curve);

    /**
     * The selection of easing curve is done in a panel to the right. The
     * panel has three lists one for each ease type (out/in/inout).
     * This function creates the three groups.
     *
     * @param parent The parent Container to which the lists should be added.
     */
    void setUpStockCurveSelectionPanel(Container *parent);

    // The description label presenting the name of the current StockCurve being used
    Label *mStockCurveDescription;

    // The animation that contains both the reset and the easing animation
    SequentialAnimation *mAnim;
    TranslateTransition *mEaseAnim;

    // The ImageView that will be animated
    ImageView *mEgg;
};

#endif // ifndef _STOCKCURVERECIPE_H_
