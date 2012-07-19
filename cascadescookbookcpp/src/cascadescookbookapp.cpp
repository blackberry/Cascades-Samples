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
#include "recipes/activityindicatorrecipe.h"
#include "recipes/animationrecipe.h"
#include "recipes/buttonrecipe.h"
#include "recipes/colorrecipe.h"
#include "recipes/datetimepickerrecipe.h"
#include "recipes/dropdownrecipe.h"
#include "recipes/docklayoutrecipe.h"
#include "recipes/gesturehandlerrecipe.h"
#include "recipes/imagerecipe.h"
#include "recipes/imagepaintrecipe.h"
#include "recipes/inputrecipe.h"
#include "recipes/intro.h"
#include "recipes/labelrecipe.h"
#include "recipes/nineslicerecipe.h"
#include "recipes/orientationrecipe.h"
#include "recipes/progressindicatorrecipe.h"
#include "recipes/sliderrecipe.h"
#include "recipes/stockcurverecipe.h"
#include "recipes/webviewrecipe.h"
#include "recipes/customdialogrecipe/customdialogrecipe.h"
#include "recipes/selectionrecipe/selection.h"
#include "recipes/sheetrecipe/sheetrecipe.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/Page>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/OrientationSupport>

#include <bb/cascades/resources/titlebar.h>
#include <bb/cascades/controls/chromevisibility.h>

CascadesCookbookApp::CascadesCookbookApp() :
        mRecipeModel()
{
    // The application uses a navigation pane to set up a drill down list UI.
    mNavPane = new NavigationPane();
    connect(mNavPane, SIGNAL(topChanged(bb::cascades::Page* )), this, SLOT(onTopChanged(bb::cascades::Page* )));

    // We create a page with ListView as content.
    Page *recipeListPage = createRecipePage();

    // The recipe Page where the recipe CustomControl added in.
    mContentPage = createContentPage();

    // Finally push the page containing the list of recipes onSelectionChanged.
    mNavPane->push(recipeListPage);

    // Create the application scene and we are done.
    Application::setScene(mNavPane);

    // We only want the app to be in Portrait in the list-view
    OrientationSupport &support = OrientationSupport::instance();
 	support.setSupportedDisplayOrientation(SupportedDisplayOrientation::DisplayPortrait);

}

CascadesCookbookApp::~CascadesCookbookApp()
{
    // If the content page does not have a parent it is not shown in the
    // navigation pane and we have to delete it manually to avoid memory leaks.
    if(!mContentPage->parent()) {
        delete mContentPage;
    }
}

Page *CascadesCookbookApp::createContentPage()
{
    // We create a second page where the recipes (CustomControls) will be added.
    Page *page = new Page();

    // We need an action on the ContentPage, so we can navigate back.
    ActionItem* backAction = ActionItem::create();
    connect(backAction, SIGNAL(triggered()), mNavPane, SLOT(pop()));
    page->setPaneProperties(NavigationPaneProperties::create().backButton(backAction));

    TitleBar* titleBar = TitleBar::create().visibility(ChromeVisibility::Visible).title("Recipe");
    page->setTitleBar(titleBar);

    // Visual appearance of the contentContainer, the content Container
    // is where the CustomControl recipes are added in onSelectionChanged.
    Container *content = new Container();

    // Background and Layout, the additional 121 pixel padding is temporary
    // to avoid putting components under the title-bar.
    ImagePaint paint(QUrl("asset:///images/background"), RepeatPattern::XY);
    content->setBackground(paint);
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

    // A nine-sliced book image is used as background of the cookbook.
    ImageView *backgroundImage = ImageView::create(
            "asset:///images/Book_background.png").layoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Fill).horizontal(
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

    recipeListContainer->add(backgroundImage);
    recipeListContainer->add(cookbookContainer);

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
    map["image"] = QString("assets/images/items/pear_peach.png");
    mRecipeModel << map;
    map["title"] = QString("Image");
    map["image"] = QString("assets/images/items/peach.png");
    mRecipeModel << map;
    map["title"] = QString("Nine Slice");
    map["image"] = QString("assets/images/items/lasagna.png");
    mRecipeModel << map;
    map["title"] = QString("Button");
    map["image"] = QString("assets/images/items/button.png");
    mRecipeModel << map;
    map["title"] = QString("Slider");
    map["image"] = QString("assets/images/items/bananapeeled.png");
    mRecipeModel << map;
    map["title"] = QString("Selection");
    map["image"] = QString("assets/images/items/olives.png");
    mRecipeModel << map;
    map["title"] = QString("Input");
    map["image"] = QString("assets/images/items/lemon.png");
    mRecipeModel << map;
    map["title"] = QString("Label");
    map["image"] = QString("assets/images/items/pear.png");
    mRecipeModel << map;
    map["title"] = QString("Picker");
    map["image"] = QString("assets/images/items/banana.png");
    mRecipeModel << map;
    map["title"] = QString("DropDown");
    map["image"] = QString("assets/images/items/beer.png");
    mRecipeModel << map;
    map["title"] = QString("ActivityIndicator");
    map["image"] = QString("assets/images/items/orange_sliced.png");
    mRecipeModel << map;
    map["title"] = QString("ProgressIndicator");
    map["image"] = QString("assets/images/items/peas_in_pod.png");
    mRecipeModel << map;
    map["title"] = QString("Color");
    map["image"] = QString("assets/images/items/strawberries.png");
    mRecipeModel << map;
    map["title"] = QString("Orientation");
    map["image"] = QString("asset:///images/items/cucumber.png");
    mRecipeModel << map;
    map["title"] = QString("DockLayout");
    map["image"] = QString("assets/images/items/docklayout.png");
    mRecipeModel << map;
    map["title"] = QString("Animation");
    map["image"] = QString("assets/images/items/egg.png");
    mRecipeModel << map;
    map["title"] = QString("Stock Curve");
    map["image"] = QString("assets/images/items/leek.png");
    mRecipeModel << map;
    map["title"] = QString("WebView");
    map["image"] = QString("assets/images/items/orange.png");
    mRecipeModel << map;
    map["title"] = QString("Sheet");
    map["image"] = QString("assets/images/items/eggplant.png");
    mRecipeModel << map;
    map["title"] = QString("CustomDialog");
    map["image"] = QString("assets/images/items/sugarcandy.png");
    mRecipeModel << map;
    map["title"] = QString("GestureHandler");
    map["image"] = QString("assets/images/items/cookie.png");
    mRecipeModel << map;
    map["title"] = QString("ImagePaint");
    map["image"] = QString("assets/images/items/sugar_tile.png");
    mRecipeModel << map;

    recipeListView->setDataModel(&mRecipeModel);
    recipeListView->setListItemManager(recipeItemManager);

    recipeListView->setLayoutProperties(
            StackLayoutProperties::create().vertical(VerticalAlignment::Fill));

    // Connect to the selection changed signal, used to update the recipe in the content area.
    connect(recipeListView, SIGNAL(triggered(const QVariantList)), this,
            SLOT(onTriggered(const QVariantList)));

    return recipeListView;
}

void CascadesCookbookApp::onTopChanged(bb::cascades::Page* page)
{
    if(page != mContentPage) {
        // Reset list selection when the top Container is not the Content Page.
        mRecipeListView->clearSelection();

        Container *content = dynamic_cast<Container *>(mContentPage->content());

        Control *recipeControl =  content->at(0);

        // Remove the current recipe once we return to the list view.
        if (content->remove(recipeControl)) {
            delete recipeControl;
        }
    }

    // We only want the app to be in Portrait in the list-view so here we reset.
    OrientationSupport &support = OrientationSupport::instance();
    if (support.uiOrientation() == UiOrientation::Landscape) {
    	support.setSupportedDisplayOrientation(SupportedDisplayOrientation::DisplayPortrait);
    }

}

void CascadesCookbookApp::onTriggered(const QVariantList indexPath)
{
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
    } else if (title.compare("Orientation") == 0) {
           recipe = new OrientationRecipe();
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
    } else if (title.compare("WebView") == 0) {
        recipe = new WebViewRecipe();
    } else if (title.compare("Sheet") == 0) {
        recipe = new SheetRecipe();
    } else if (title.compare("CustomDialog") == 0) {
        recipe = new CustomDialogRecipe();
    } else if (title.compare("GestureHandler") == 0) {
        recipe = new GestureHandlerRecipe();
    } else if (title.compare("ImagePaint") == 0) {
        recipe = new ImagePaintRecipe();
    }


    if (recipe) {
        // Get the content Container of the ContentPage, add the new recipe (it will be removed in the onTopChanged SLOT).
        Container *content = dynamic_cast<Container *>(mContentPage->content());
        recipe->setLayoutProperties(DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                        VerticalAlignment::Center));
        content->add(recipe);

        mContentPage->titleBar()->setTitle(title);

        mNavPane->push(mContentPage);
    } else {
        qDebug("No recipe created for this item yet.");
    }

}
