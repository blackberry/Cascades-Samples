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
#include "cascadescookbookapp.h"
#include "recipeitemfactory.h"
#include "recipes/animationrecipe.h"
#include "recipes/buttonrecipe.h"
#include "recipes/colorrecipe.h"
#include "recipes/datetimepickerrecipe.h"
#include "recipes/dropdownrecipe.h"
#include "recipes/docklayoutrecipe.h"
#include "recipes/imagerecipe.h"
#include "recipes/inputrecipe.h"
#include "recipes/intro.h"
#include "recipes/labelrecipe.h"
#include "recipes/nineslicerecipe.h"
#include "recipes/selection.h"
#include "recipes/sliderrecipe.h"
#include "recipes/stockcurverecipe.h"
#include "recipes/activityindicatorrecipe.h"
#include "recipes/progressindicatorrecipe.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/Page>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

CascadesCookbookApp::CascadesCookbookApp() :
        mRecipeModel()
{
    // The application uses a navigation pane to set up a drill down list UI.
    mNavPane = new NavigationPane();
    connect(mNavPane, SIGNAL(topChanged(bb::cascades::AbstractPane* )), this, SLOT(onTopChanged(bb::cascades::AbstractPane* )));

    // We create a page with ListView as content.
    Page *recipeListPage = createRecipePage();

    // The recipe Page where the recipe CustomControl added in.
    mContentPage = createContentPage();

    // Finally push the page containing the list of recipes onSelectionChanged.
    mNavPane->push(recipeListPage);

    // Create the application scene and we are done.
    Application::setScene(mNavPane);
}

Page *CascadesCookbookApp::createContentPage()
{
    // We create a second page where the recipes (CustomControls) will be added.
    Page *page = new Page();

    // We need an action on the ContentPage, so we can navigate back.
    ActionItem* backAction = ActionItem::create();
    QObject::connect(backAction, SIGNAL(triggered()), mNavPane, SLOT(pop()));
    page->setPaneProperties(NavigationPaneProperties::create().backButton(backAction));

    // Visual appearance of the contentContainer, the content Container
    // is where the CustomControl recipes are added in onSelectionChanged.
    Container *content = new Container();
    content->setBackground(Color::fromARGB(0xff262626));
    content->setLayout(DockLayout::create());

    // Adding the ContentContainer.
    page->setContent(content);

    return page;
}

Page *CascadesCookbookApp::createRecipePage()
{
    Page *recipeListPage = new Page();

    // The main app Container a DockLayout is used to align children.
    Container *recipeListContainer = new Container();
    DockLayout *recipeListLayout = new DockLayout();
    recipeListContainer->setLayout(recipeListLayout);

    // The background of the UI is set up by coloring the background Container,
    // two decoration images at the top and bottom and a paper crease creating the illusion of a book.
    recipeListContainer->setBackground(Color::fromARGB(0xff262626));

    ImageView *topDecorationImage =
            ImageView::create("asset:///images/red_cloth_edge_top.png").preferredHeight(15.0f).layoutProperties(
                    DockLayoutProperties::create().vertical(VerticalAlignment::Top).horizontal(
                            HorizontalAlignment::Fill));

    ImageView *bottomDecorationImage = ImageView::create(
            "images/red_cloth_edge_bottom.png").preferredHeight(15.0f).layoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Bottom).horizontal(
                    HorizontalAlignment::Fill));

    // Create a Container for the list and the content view which
    Container *cookbookContainer = new Container();
    DockLayout *cookbookLayout = new DockLayout();
    cookbookLayout->setTopPadding(15.0f);
    cookbookLayout->setBottomPadding(15.0f);
    cookbookContainer->setLayout(cookbookLayout);
    cookbookContainer->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    // Setup and create the content view and the recipe list.
    mRecipeListView = createRecipeListView();
    mRecipeListView->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    // Add the controls.
    cookbookContainer->add(mRecipeListView);

    recipeListContainer->add(cookbookContainer);
    recipeListContainer->add(topDecorationImage);
    recipeListContainer->add(bottomDecorationImage);

    recipeListPage->setContent(recipeListContainer);

    return recipeListPage;
}

ListView *CascadesCookbookApp::createRecipeListView()
{
    // The list view and its its item manager (where item are created and set up).
    ListView *recipeListView = new ListView();
    RecipeItemFactory *recipeItemManager = new RecipeItemFactory();

    // The list model, item data is added to a variant map.
    QVariantMap map = QVariantMap();
    map["title"] = QString("Introduction");
    map["image"] = QString("asset:///images/items/pear_peach.png");
    mRecipeModel << map;
    map["title"] = QString("Image");
    map["image"] = QString("asset:///images/items/peach.png");
    mRecipeModel << map;
    map["title"] = QString("Nine Slice");
    map["image"] = QString("asset:///images/items/lasagna.png");
    mRecipeModel << map;
    map["title"] = QString("Button");
    map["image"] = QString("asset:///images/items/button.png");
    mRecipeModel << map;
    map["title"] = QString("Slider");
    map["image"] = QString("asset:///images/items/bananapeeled.png");
    mRecipeModel << map;
    map["title"] = QString("Selection");
    map["image"] = QString("asset:///images/items/olives.png");
    mRecipeModel << map;
    map["title"] = QString("Input");
    map["image"] = QString("asset:///images/items/orange.png");
    mRecipeModel << map;
    map["title"] = QString("Label");
    map["image"] = QString("asset:///images/items/pear.png");
    mRecipeModel << map;
    map["title"] = QString("Picker");
    map["image"] = QString("asset:///images/items/banana.png");
    mRecipeModel << map;
    map["title"] = QString("DropDown");
    map["image"] = QString("asset:///images/items/beer.png");
    mRecipeModel << map;
    map["title"] = QString("ActivityIndicator");
    map["image"] = QString("asset:///images/items/peas_in_pod.png");
    mRecipeModel << map;
    map["title"] = QString("ProgressIndicator");
    map["image"] = QString("asset:///images/items/orange_sliced.png");
    mRecipeModel << map;
    map["title"] = QString("Color");
    map["image"] = QString("asset:///images/items/strawberries.png");
    mRecipeModel << map;
    map["title"] = QString("DockLayout");
    map["image"] = QString("asset:///images/items/docklayout.png");
    mRecipeModel << map;
    map["title"] = QString("Animation");
    map["image"] = QString("asset:///images/items/egg.png");
    mRecipeModel << map;
    map["title"] = QString("Stock Curve");
    map["image"] = QString("asset:///images/items/leek.png");
    mRecipeModel << map;

    recipeListView->setDataModel(&mRecipeModel);
    recipeListView->setListItemManager(recipeItemManager);

    recipeListView->setLayoutProperties(
            StackLayoutProperties::create().vertical(VerticalAlignment::Fill));

    // Connect to the selection changed signal, used to update the recipe in the content area.
    connect(recipeListView, SIGNAL(selectionChanged(const QVariantList, bool)), this,
            SLOT(onSelectionChanged(const QVariantList, bool)));

    return recipeListView;
}

void CascadesCookbookApp::onTopChanged(bb::cascades::AbstractPane* pane)
{
    if(pane != mContentPage) {
        // Reset list selection when the top Container is not the Content Page.
        mRecipeListView->clearSelection();
    }
}

void CascadesCookbookApp::onSelectionChanged(const QVariantList indexPath, bool selected)
{
    if (selected) {
        CustomControl *recipe = NULL;

        // Get the selected item title.
        QVariantMap map = mRecipeModel.data(indexPath).toMap();
        QString title = map.value("title").toString();

        // Create a recipe based on the selected item title.
        if (title.compare("Introduction") == 0) {
            recipe = new Intro();
        } else if (title.compare("Image") == 0) {
            recipe = new ImageRecipe();
        } else if (title.compare("Nine Slice") == 0) {
            recipe = new NineSliceRecipe();
        } else if (title.compare("Button") == 0) {
            recipe = new ButtonRecipe();
        } else if (title.compare("Slider") == 0) {
            recipe = new SliderRecipe();
        } else if (title.compare("Selection") == 0) {
            recipe = new SelectionRecipe();
        } else if (title.compare("Input") == 0) {
            recipe = new InputRecipe();
        } else if (title.compare("Label") == 0) {
            recipe = new LabelRecipe();
        } else if (title.compare("DockLayout") == 0) {
            recipe = new DockLayoutRecipe();
        } else if (title.compare("Color") == 0) {
            recipe = new ColorRecipe();
        } else if (title.compare("Animation") == 0) {
            recipe = new AnimationRecipe();
        } else if (title.compare("Stock Curve") == 0) {
            recipe = new StockCurveRecipe();
        } else if (title.compare("Picker") == 0) {
            recipe = new DateTimePickerRecipe();
        } else if (title.compare("DropDown") == 0) {
            recipe = new DropDownRecipe();
        } else if (title.compare("ActivityIndicator") == 0) {
            recipe = new ActivityIndicatorRecipe();
        } else if (title.compare("ProgressIndicator") == 0) {
            recipe = new ProgressIndicatorRecipe();
        }

        if (recipe) {
            // Get the content Container of the ContentPage, remove the old recipe and add the new one.
            Container *content = dynamic_cast<Container *>(mContentPage->content());
            content->removeAll();
            recipe->setLayoutProperties(
                    DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                            VerticalAlignment::Center));
            content->add(recipe);

            mNavPane->push(mContentPage);
        } else {
            qDebug("No recipe created for this item yet.");
        }
    }
}
