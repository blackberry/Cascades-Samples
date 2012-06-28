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
#include "docklayoutrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

DockLayoutRecipe::DockLayoutRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new DockLayout());

    // Listen for touch on the Container, touch down will alter the layout properties on the images in the UI.
    connect(recipeContainer, SIGNAL(touch(bb::cascades::TouchEvent *)), this,
            SLOT(onTouch(bb::cascades::TouchEvent *)));

    // Background ImagePaint to paint it black.
    ImagePaint paint(QUrl("asset:///images/docklayout/black_page.png"));
    recipeContainer->setBackground(paint);
    recipeContainer->setPreferredSize(768, 1280);

    // Center bean, center/center aligned.
    ImageView *centerBean =
            ImageView::create("asset:///images/docklayout/bean_centered.png").layoutProperties(
                    DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                            VerticalAlignment::Center));

    // Bottom bean, center/bottom.
    mBottomBean =
            ImageView::create("asset:///images/docklayout/bean_horizontal.png").layoutProperties(
                    DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                            VerticalAlignment::Bottom));

    // Right bean, right/center.
    mRightBean = ImageView::create("asset:///images/docklayout/bean_vertical.png").layoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Right).vertical(
                    VerticalAlignment::Center));

    // Left bean, center/center aligned.
    mLeftBean = ImageView::create("asset:///images/docklayout/bean_vertical.png").layoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Left).vertical(
                    VerticalAlignment::Center));

    // Left bean, center/center aligned.
    mTopBean = ImageView::create("asset:///images/docklayout/bean_horizontal.png").layoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                    VerticalAlignment::Top));

    // A Label with instructional text that the background should be tapped
    // to set all alignment to center. It is put inside a Container in order
    // to offset it from the center so it is not overlapping the center image.
    Container *infoContainer = Container::create().layout(DockLayout::create().bottom(300.0f)).layoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                    VerticalAlignment::Center));

    // Label used to display the current layout.
    mActionLabel = new Label();
    mActionLabel->setText((const QString) "Tap to center");
    mActionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mActionLabel->textStyle()->setColor(Color::Gray) ;

    infoContainer->add(mActionLabel);

    // Add the controls to the recipe Container and set it as the CustomControl root.
    recipeContainer->add(infoContainer);
    recipeContainer->add(centerBean);
    recipeContainer->add(mLeftBean);
    recipeContainer->add(mBottomBean);
    recipeContainer->add(mRightBean);
    recipeContainer->add(mTopBean);
    setRoot(recipeContainer);
}

void DockLayoutRecipe::onTouch(bb::cascades::TouchEvent *event)
{
    if (event->isUp()) {
        // On touch up, all alignments are set to center if the top bean is top aligned.
        // If it is centered all alignment is set to correspond to the original values.
        DockLayoutProperties *properties =
                (DockLayoutProperties *) mTopBean->layoutProperties();

        if (properties->verticalAlignment() == VerticalAlignment::Top) {
            DockLayoutProperties::updateFor(mTopBean).vertical(VerticalAlignment::Center);
            DockLayoutProperties::updateFor(mBottomBean).vertical(VerticalAlignment::Center);
            DockLayoutProperties::updateFor(mRightBean).horizontal(HorizontalAlignment::Center);
            DockLayoutProperties::updateFor(mLeftBean).horizontal(HorizontalAlignment::Center);
            mActionLabel->setOpacity(0.0);
        } else {
            DockLayoutProperties::updateFor(mTopBean).vertical(VerticalAlignment::Top);
            DockLayoutProperties::updateFor(mBottomBean).vertical(VerticalAlignment::Bottom);
            DockLayoutProperties::updateFor(mRightBean).horizontal(HorizontalAlignment::Right);
            DockLayoutProperties::updateFor(mLeftBean).horizontal(HorizontalAlignment::Left);
            mActionLabel->setOpacity(1.0);
        }
    }
}
