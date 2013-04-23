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
#include "customdialogrecipe.h"
#include "customdialogalarm.h"

#include <bb/cascades/Container>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/ScaleTransition>

using namespace bb::cascades;

CustomDialogRecipe::CustomDialogRecipe(Container * parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new AbsoluteLayout());

    mFlame = ImageView::create("asset:///images/customdialog/flame.png");

    // Connect to the layout handler of the flame image, we need the image size
    // in order to set the pivot point for scaling correctly.
    LayoutUpdateHandler::create(mFlame).onLayoutFrameChanged(this,
            SLOT(flameLayoutFrameUpdated(QRectF)));

    // The flame animation gradually scales the flame up in Y direction and
    // finally triggers the CustomDialog.
    mRisingFlame = SequentialAnimation::create(mFlame)
            .add(ScaleTransition::create(mFlame).toY(1.2).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.1).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.4).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.3).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.6).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.5).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.9).duration(400))
            .add(ScaleTransition::create(mFlame).toY(1.7).duration(400))
            .add(ScaleTransition::create(mFlame).toY(2.0).duration(400))
            .parent(this);

    connect(mRisingFlame, SIGNAL(ended()), this, SLOT(onHideAnimEnded()));

    ImageView *candle = ImageView::create("asset:///images/customdialog/background.png");
    candle->setScalingMethod(ScalingMethod::AspectFit);

    // The CustomDialog is added as an attached object since it is visible in the
    // UI from the start. Since a dialog is often used in many different places in an application,
    // it is set up as a separate component. This is to easily add it to other Pages.
    mAlarmDialog = new CustomDialogAlarm(this);
    connect(mAlarmDialog, SIGNAL(visibleChanged(bool)), this, SLOT(onDialogVisible(bool)));

    recipeContainer->add(mFlame);
    recipeContainer->add(candle);

    setRoot(recipeContainer);

    mRisingFlame->play();
}

void CustomDialogRecipe::onHideAnimEnded()
{
    mAlarmDialog->setVisible(true);
}

void CustomDialogRecipe::onDialogVisible(bool visible)
{
    if (!visible) {
        mRisingFlame->play();
    }
}

void CustomDialogRecipe::flameLayoutFrameUpdated(QRectF layoutRect)
{
    if(mFlame) {
        // The pivot points are set to the middle bottom of the image
        // so that it can be scaled upwards in the animation below.
        mFlame->setPivot(layoutRect.width()/2, layoutRect.height()/2);
    }
}

