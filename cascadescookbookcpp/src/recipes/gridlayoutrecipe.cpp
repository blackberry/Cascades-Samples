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
#include "gridlayoutrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/GridLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Label>
#include <bb/cascades/SegmentedControl>

using namespace bb::cascades;

GridLayoutRecipe::GridLayoutRecipe(Container *parent) :
        CustomControl(parent)
{
    // Make sure the recipe fills the entire Page
    Container *recipeContainer = Container::create()
                                    .vertical(VerticalAlignment::Fill)
                                    .horizontal(HorizontalAlignment::Fill)
                                    .layout(DockLayout::create());

    // The column controller and the shelf is positioned at the bottom.
    Container *contentContainer = Container::create().vertical(VerticalAlignment::Bottom);

    // Both these Containers contain Containers with GridLayouts.
    Container *shelfContainer = createBookshelf();
    Container *controlPanelContainer = createControlPanel();

    // Assemble the UI, add the containers and set up the recipe Container.
    contentContainer->add(shelfContainer);
    contentContainer->add(controlPanelContainer);
    recipeContainer->add(contentContainer);

    setRoot(recipeContainer);
}

Container* GridLayoutRecipe::createBookshelf()
{
    UIConfig *ui = this->ui();
    Container *content = Container::create()
                            .bottom(ui->du(3))
                            .horizontal(HorizontalAlignment::Center);

    Container *shelfContainer = Container::create();
    mShelfLayout = GridLayout::create();
    shelfContainer->setLayout(mShelfLayout);

    // Fill the shelf with cells
    for(int i = 0; i < 12; i++) {
        shelfContainer->add(createBookshelfCell());
    }

    // Add content to some of the cells
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/pepper.png", 0, HorizontalAlignment::Right);
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/egg.png", 3, HorizontalAlignment::Right);
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/pot.png", 4, HorizontalAlignment::Center);
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/pepper.png", 6, HorizontalAlignment::Left);
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/egg.png", 9, HorizontalAlignment::Left);
    addImageToCellInShelf(shelfContainer, "asset:///images/gridlayout/egg.png", 10, HorizontalAlignment::Right);

    // The legs of the bookcase.
    ImageView *shelfLegs = ImageView::create("asset:///images/gridlayout/legs4.amd")
                                .horizontal(HorizontalAlignment::Fill)
                                .topMargin(0);

    content->add(shelfContainer);
    content->add(shelfLegs);

    return content;
}

Container* GridLayoutRecipe::createBookshelfCell()
{
    UIConfig *ui = this->ui();
    Container *content = Container::create()
                .top(ui->du(4)).left(ui->du(2)).right(ui->du(2))
                .horizontal(HorizontalAlignment::Fill)
                .vertical(VerticalAlignment::Fill)
                .layout(DockLayout::create());
    content->setBackground(ImagePaint("asset:///images/gridlayout/frame6.amd"));

    return content;
}

void GridLayoutRecipe::addImageToCellInShelf(Container* shelfContainer, const QString imageSource,
        int cellIndex, HorizontalAlignment::Type horizontalAlignment)
{
    Container *cellContainer = qobject_cast<Container*>(shelfContainer->at(cellIndex));
    ImageView *cellImage = ImageView::create(imageSource)
                                .horizontal(horizontalAlignment)
                                .vertical(VerticalAlignment::Bottom);
    cellContainer->add(cellImage);
}

Container* GridLayoutRecipe::createControlPanel()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    UIConfig *ui = this->ui();
    Container *content = Container::create()
                            .top(ui->du(2)).bottom(ui->du(2)).left(ui->du(2)).right(ui->du(2))
                            .background(Color::fromARGB(0x33ffffff));

    // The Container use a GridLayout with default coloumn count (2).
    GridLayout *layout = new GridLayout();
    content->setLayout(layout);

    Label *controlLabel = Label::create().text("Columns").vertical(VerticalAlignment::Center);

    // The width of the coloumn has to be set since the SegmentedControl will try and span the entire screen width.
    controlLabel->setMinWidth(ui->du(15));

    SegmentedControl *columnSegmented = SegmentedControl::create();

    // The 720x720 resolution is to small for 2 columns to fit on screen.
    if(UiValues::instance()->device() != UiValues::DEVICETYPE_720X720) {
        columnSegmented->add(Option::create().text("2").value(2));
    }
    columnSegmented->add(Option::create().text("4").value(4));
    columnSegmented->add(Option::create().text("6").value(6));

    connectResult = connect(columnSegmented, SIGNAL(selectedOptionChanged(bb::cascades::Option *)), SLOT(onColumnCountChanged(bb::cascades::Option *)));
    Q_ASSERT(connectResult);

    content->add(controlLabel);
    content->add(columnSegmented);

    // Call the column count function to set up the correct number of columns in the shelf GridLayout.
    onColumnCountChanged(columnSegmented->selectedOption());

    return content;
}

void GridLayoutRecipe::onColumnCountChanged(bb::cascades::Option* option)
{
    mShelfLayout->setColumnCount(option->value().toUInt());
}
