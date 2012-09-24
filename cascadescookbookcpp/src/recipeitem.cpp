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
#include "recipeitem.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

RecipeItem::RecipeItem(Container *parent) :
    CustomControl(parent)
{
  // A background Container that will hold a background image and an item content Container
  Container *itemContainer = new Container();
  DockLayout *itemLayout = new DockLayout();
  itemContainer->setLayout(itemLayout);
  itemContainer->setPreferredWidth(768.0f);

  // The white background image for an item
  ImageView *bkgImage = ImageView::create("asset:///images/white_photo.png")
                          .preferredSize(768.0f, 173.0f);

  // A Colored Container will be used to show if an item is highlighted
  mHighlighContainer = new Container();
  mHighlighContainer->setBackground(Color::fromARGB(0xff75b5d3));
  mHighlighContainer->setHorizontalAlignment(HorizontalAlignment::Center);
  mHighlighContainer->setOpacity(0.0);
  mHighlighContainer->setPreferredWidth(760.0f);
  mHighlighContainer->setPreferredHeight(168.0f);

  // Content Container containing an image and label with padding for the alignment on background image
  Container *contentContainer = new Container();
  StackLayout *contentLayout = new StackLayout();
  contentLayout->setOrientation(LayoutOrientation::LeftToRight);
  contentContainer->setLeftPadding(3.0f);
  contentContainer->setLayout(contentLayout);

  // The list item image which is docked to the top, the actual image is set in updateItem
  mItemImage = ImageView::create("asset:///images/white_photo.png");
  mItemImage->setVerticalAlignment(VerticalAlignment::Top);

  // A list item label, docked to the center, the text is set in updateItem.
  mItemLabel = Label::create().text(" ");
  mItemLabel->setVerticalAlignment(VerticalAlignment::Center);
  mItemLabel->setLeftMargin(30.0f);
  mItemLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
  mItemLabel->textStyle()->setColor(Color::Black);

  // Add the Image and Label to the content Container
  contentContainer->add(mItemImage);
  contentContainer->add(mItemLabel);

  // Add the background image and the content to the full item container.
  itemContainer->add(bkgImage);
  itemContainer->add(mHighlighContainer);
  itemContainer->add(contentContainer);

  setRoot(itemContainer);
}

void RecipeItem::updateItem(const QString text, const QString imagePath)
{
  // Update image and text for the current item
  mItemImage->setImage(Image(imagePath));
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
