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
#include "docklayoutrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

DockLayoutRecipe::DockLayoutRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Create the recipe Container and set its layout to be a DockLayout.
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new DockLayout());

    // Listen for touch events on the Container, to toggle the alignment of the UI objects.
    connectResult = connect(recipeContainer, SIGNAL(touch(bb::cascades::TouchEvent *)), this,
            SLOT(onTouch(bb::cascades::TouchEvent *)));
    Q_ASSERT(connectResult);

    // Background ImagePaint set a black image.
    ImagePaint paint(QUrl("asset:///images/docklayout/black_page.png"));
    recipeContainer->setBackground(paint);

    // Bottom bean, center/bottom
    mBottomBean = ImageView::create("asset:///images/docklayout/bean_horizontal.png");
    mBottomBean->setHorizontalAlignment(HorizontalAlignment::Center);
    mBottomBean->setVerticalAlignment(VerticalAlignment::Bottom);

    // Right bean, right/center
    mRightBean = ImageView::create("asset:///images/docklayout/bean_vertical.png");
    mRightBean->setHorizontalAlignment(HorizontalAlignment::Right);
    mRightBean->setVerticalAlignment(VerticalAlignment::Center);

    // Left bean, center/center aligned
    mLeftBean = ImageView::create("asset:///images/docklayout/bean_vertical.png");
    mLeftBean->setHorizontalAlignment(HorizontalAlignment::Left);
    mLeftBean->setVerticalAlignment(VerticalAlignment::Center);

    // Left bean, center/center aligned
    mTopBean = ImageView::create("asset:///images/docklayout/bean_horizontal.png");
    mTopBean->setHorizontalAlignment(HorizontalAlignment::Center);
    mTopBean->setVerticalAlignment(VerticalAlignment::Top);

    // A Label with instructional text that the background should be tapped
    // to set all alignment to center.
    mActionLabel = new Label();
    mActionLabel->setText("Tap to center");
    mActionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mActionLabel->textStyle()->setColor(Color::Gray);
    mActionLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mActionLabel->setVerticalAlignment(VerticalAlignment::Center);

    // Add the controls to the recipe Container and set it as the CustomControl root.
    recipeContainer->add(mActionLabel);
    recipeContainer->add(mLeftBean);
    recipeContainer->add(mBottomBean);
    recipeContainer->add(mRightBean);
    recipeContainer->add(mTopBean);
    setRoot(recipeContainer);
}

void DockLayoutRecipe::onTouch(bb::cascades::TouchEvent *event)
{
    if (event->isUp()) {
        // On a touch up event, all alignments are set to center if the top bean is top aligned.
        // If it is centered all alignments are set to correspond to the original values.
        if (mTopBean->verticalAlignment() == VerticalAlignment::Top) {
            mTopBean->setVerticalAlignment(VerticalAlignment::Center);
            mBottomBean->setVerticalAlignment(VerticalAlignment::Center);
            mRightBean->setHorizontalAlignment(HorizontalAlignment::Center);
            mLeftBean->setHorizontalAlignment(HorizontalAlignment::Center);
            mActionLabel->setOpacity(0.0);
        } else {
            mTopBean->setVerticalAlignment(VerticalAlignment::Top);
            mBottomBean->setVerticalAlignment(VerticalAlignment::Bottom);
            mRightBean->setHorizontalAlignment(HorizontalAlignment::Right);
            mLeftBean->setHorizontalAlignment(HorizontalAlignment::Left);
            mActionLabel->setOpacity(1.0);
        }
    }
}
