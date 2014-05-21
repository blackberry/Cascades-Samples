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
#include "recipeitem.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/UIConfig>

using namespace bb::cascades;

RecipeItem::RecipeItem(Container *parent) :
        CustomListItem(HighlightAppearance::None, parent)
{
    Container *itemContainer = Container::create().layout(DockLayout::create())
                                        .horizontal(HorizontalAlignment::Fill).vertical(VerticalAlignment::Fill);

    // The UIConfig object that gives access to device dependant conversion routines.
    UIConfig *ui = itemContainer->ui();

    itemContainer->setTopPadding(ui->du(0.2));
    itemContainer->setBottomPadding(ui->du(0.6));

    // The white background image paint for an item
    ImagePaint paint(QUrl("asset:///images/white_photo.amd"));
    itemContainer->setBackground(paint);

    // A Colored Container will be used to show if an item is highlighted
    mHighlighContainer = Container::create().background(Color::fromARGB(0xff75b5d3))
                            .horizontal(HorizontalAlignment::Fill).vertical(VerticalAlignment::Fill)
                            .opacity(0.0);

    // Content Container containing an image and label with padding for the alignment
    // on background image. Note that we disable implicit layout animations to avoid
    // unsynchronized animations on the list items as the item image is asynchronously loaded.
    Container *contentContainer = new Container();
    contentContainer->setLeftPadding(ui->du(0.3));
    contentContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
    contentContainer->setImplicitLayoutAnimationsEnabled(false);

    // The list item image which is docked to the top, the actual image is
    // set in updateItem function.
    mItemImage = ImageView::create("asset:///images/white_photo.png")
                    .implicitLayoutAnimations(false).preferredHeight(ui->du(16.8))
                    .scalingMethod(ScalingMethod::AspectFit);

    // A list item label, docked to the center, the text is set in updateItem.
    mItemLabel = Label::create();
    mItemLabel->setVerticalAlignment(VerticalAlignment::Center);
    mItemLabel->setLeftMargin(ui->du(3.0));
    mItemLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mItemLabel->textStyle()->setColor(Color::Black);
    mItemLabel->setImplicitLayoutAnimationsEnabled(false);

    // Add the Image and Label to the content Container
    contentContainer->add(mItemImage);
    contentContainer->add(mItemLabel);

    // Add the background image and the content to the full item container.
    itemContainer->add(mHighlighContainer);
    itemContainer->add(contentContainer);

    setDividerVisible(false);
    setPreferredHeight(ui->du(17.6));
    setContent(itemContainer);
}

void RecipeItem::updateItem(const QString text, const QString imagePath)
{
    // Update image and text for the current item
    mItemImage->setImageSource(imagePath);
    mItemLabel->setText(text);
}

void RecipeItem::select(bool select)
{
    // When an item is selected, show the colored highlight Container
    if (select) {
        mHighlighContainer->setOpacity(0.9f);
    } else {
        mHighlighContainer->setOpacity(0.0f);
    }
}

void RecipeItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    // Since items are recycled, the reset function is where we have
    // to make sure that item state (defined by the arguments) is correct.
    select(selected);
}

void RecipeItem::activate(bool activate)
{
    // There is no special activate state; selected and activated look the same.
    select(activate);
}
