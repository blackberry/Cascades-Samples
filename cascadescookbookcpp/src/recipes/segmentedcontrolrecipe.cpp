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
#include "segmentedcontrolrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/SegmentedControl>
#include <bb/cascades/ScrollView>
#include <bb/cascades/SystemDefaults>

using namespace bb::cascades;

SegmentedControlRecipe::SegmentedControlRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // A recipe showing how to listen to changes in SegmentedControl
    // and use this to change the look of the interface.
    Container *recipeContainer = Container::create().left(ui->du(2)).right(ui->du(2)).top(ui->du(3));

    // The segmented control is implemented and given three values and text
    // to represent those values.
    SegmentedControl *segmented = SegmentedControl::create()
            .add(bb::cascades::Option::create().text("Leek").value("1").selected(true))
            .add(bb::cascades::Option::create().text("Beetroot").value("2"))
            .add(bb::cascades::Option::create().text("Garlic").value("3"));

    // Connect to the selectedIndexChanged in order to update the UI when the selection changes.
    connectResult = connect(segmented, SIGNAL(selectedIndexChanged(int)), this, SLOT(onSegmentedIndexChanged(int)));
    Q_ASSERT(connectResult);



    // This labels text is altered when the segmentedControl value is changed.
    mTitleLabel = new Label();
    mTitleLabel->setText("Potato Leek Soup");
    mTitleLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

    // This ImageView's imageSource is altered when the segmentedControl value is changed.
    mImage = ImageView::create("asset:///images/segmentedcontrol/soup_green.png").scalingMethod(ScalingMethod::AspectFit);

    // Add the controls to the recipe Container.
    recipeContainer->add(segmented);
    recipeContainer->add(mTitleLabel);
    recipeContainer->add(mImage);

    if (UiValues::instance()->device() == UiValues::DEVICETYPE_720X720) {
        // If the device is of 720X720 resolution we make it scrollable.
        ScrollView *scrollRecipe = ScrollView::create()
            .scrollMode(ScrollMode::Vertical)
            .content(recipeContainer);
        setRoot(scrollRecipe);
    } else {
        // The content fits on this screen type so we just set the Container as the root Control.
        setRoot(recipeContainer);
    }
}

// This is the slot that is triggered when the value is changed in segmented control.
// The index of the selected value is taken as a in parameter.
void SegmentedControlRecipe::onSegmentedIndexChanged(int selectedIndex)
{
    // Depending on what index is chosen the title and and image is
    // changed to match the ingredient of choice.
    if (selectedIndex == 0) {
        mTitleLabel->setText(QString("Potato Leek Soup"));
        mImage->setImageSource(QUrl("asset:///images/segmentedcontrol/soup_green.png"));
    } else if (selectedIndex == 1) {
        mTitleLabel->setText(QString("Borscht"));
        mImage->setImageSource(QUrl("asset:///images/segmentedcontrol/soup_beet.png"));
    } else if (selectedIndex == 2) {
        mTitleLabel->setText(QString("French Farmers Soup"));
        mImage->setImageSource(QUrl("asset:///images/segmentedcontrol/soup_white.png"));
    }
}
