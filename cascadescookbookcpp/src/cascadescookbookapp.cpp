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
#include "uivalues.h"
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
#include "recipes/invocationrecipe.h"
#include "recipes/labelrecipe.h"
#include "recipes/menurecipe.h"
#include "recipes/nineslicerecipe.h"
#include "recipes/orientationrecipe.h"
#include "recipes/pixelbufferrecipe.h"
#include "recipes/progressindicatorrecipe.h"
#include "recipes/richtextrecipe.h"
#include "recipes/sliderrecipe.h"
#include "recipes/stockcurverecipe.h"
#include "recipes/segmentedcontrolrecipe.h"
#include "recipes/shortcutrecipe.h"
#include "recipes/webviewrecipe.h"
#include "recipes/activityindicatorrecipe/activityindicatorrecipe.h"
#include "recipes/customdialogrecipe/customdialogrecipe.h"
#include "recipes/custompickerrecipe/custompickerrecipe.h"
#include "recipes/selectionrecipe/selection.h"
#include "recipes/sheetrecipe/sheetrecipe.h"
#include "recipes/accessabilityrecipe.h"
#include "recipes/textstylerecipe.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/Page>
#include <bb/cascades/SceneCover>
#include <bb/cascades/StackLayout>
#include <bb/cascades/TextStyle>
#include <bb/cascades/OrientationSupport>
#include <bb/system/SystemToast>
#include <bb/cascades/Menu>

#include <bb/cascades/resources/titlebar.h>
#include <bb/cascades/controls/chromevisibility.h>
#include <bb/cascades/HelpActionItem>
#include <bb/cascades/SettingsActionItem>

CascadesCookbookApp::CascadesCookbookApp() :
		mRecipeModel() {
	// The application uses a navigation pane to set up a drill down list UI.
	// We also connect the topChanged and popTransitionEnded signals to the appropriate slot functions.
	mNavPane = new NavigationPane();
	connect(mNavPane, SIGNAL(topChanged(bb::cascades::Page* )), this,
			SLOT(onTopChanged(bb::cascades::Page* )));
	connect(mNavPane, SIGNAL(popTransitionEnded(bb::cascades::Page *)), this,
			SLOT(onPopTransitionEnded(bb::cascades::Page *)));

	// connect(backAction, SIGNAL(triggered()), mNavPane, SLOT(pop()));

	// Create a recipe list Page with ListView as content.
	Page *recipeListPage = createRecipePage();

	// Create the content Page where the recipe CustomControl added in.
	mContentPage = createContentPage();

	// Push the page containing the list of recipes to the NavigationPane.
	mNavPane->push(recipeListPage);

	// Set the application scene.
	Application::instance()->setScene(mNavPane);

	// Add an application cover, used when the app is minimized on the running apps panel on the home screen.
	addApplicationCover();

	//Let's make a top menu that you find by dragging down from the bezel and add a regular action, a help action and a settingsaction
	Menu *menu = Menu::create().addAction(
			ActionItem::create().objectName("Action").image(Image("asset:///images/salt_icon.png")).connect(
					SIGNAL(triggered()), this, SLOT(onActionTriggerd())).title(
					"Add Salt")).settings(
			SettingsActionItem::create().objectName("Setting").connect(
					SIGNAL(triggered()), this, SLOT(onActionTriggerd()))).help(
			HelpActionItem::create().objectName("Help").connect(
					SIGNAL(triggered()), this, SLOT(onActionTriggerd())));
	Application::instance()->setMenu(menu);

	// We only want the app to be in Portrait mode when in the ListView is active.
	OrientationSupport::instance()->setSupportedDisplayOrientation(
			SupportedDisplayOrientation::DisplayPortrait);
}

CascadesCookbookApp::~CascadesCookbookApp() {
	// If the content page does not have a parent, it is not shown in the
	// navigation pane so we have to delete it manually to avoid memory leaks.
	if (!mContentPage->parent()) {
		delete mContentPage;
	}
}

Page *CascadesCookbookApp::createContentPage() {
	// Create a second page where the recipes (CustomControls) will be added in
	Page *page = new Page();

	// We need an action on the ContentPage, so we can navigate back.
	ActionItem* backAction = ActionItem::create();
	connect(backAction, SIGNAL(triggered()), mNavPane, SLOT(pop()));
	page->setPaneProperties(
			NavigationPaneProperties::create().backButton(backAction));

	// Will keep the actionbar visible for all recipes.
	page->setActionBarAutoHideBehavior(ActionBarAutoHideBehavior::Disabled);


	// Set the title bar
	TitleBar* titleBar = TitleBar::create(TitleBarScrollBehavior::Sticky).visibility(
			ChromeVisibility::Visible).title("Recipe");
	page->setTitleBar(titleBar);

	// Create the content Container which is where the CustomControl recipes are added in onSelectionChanged
	Container *content = new Container();

	// Set the background and layout
	ImagePaint paint(QUrl("asset:///images/background.amd"), RepeatPattern::XY);
	content->setBackground(paint);
	content->setLayout(DockLayout::create());

	// Add the ContentContainer to the content Page
	page->setContent(content);

	return page;
}

Page *CascadesCookbookApp::createRecipePage() {
	Page *recipeListPage = new Page();

	// Create the main app Container and set a DockLayout which is used to align children
	Container *recipeListContainer = new Container();
	DockLayout *recipeListLayout = new DockLayout();
	recipeListContainer->setLayout(recipeListLayout);

	// A nine-sliced book image is used as background of the cookbook.
	ImageView *backgroundImage = ImageView::create(
			"asset:///images/Book_background.amd");
	backgroundImage->setVerticalAlignment(VerticalAlignment::Fill);
	backgroundImage->setHorizontalAlignment(HorizontalAlignment::Fill);

	// Create and set up a Container for the list
	Container *cookbookContainer = new Container();
	cookbookContainer->setTopPadding(15.0f);
	cookbookContainer->setBottomPadding(15.0f);
	cookbookContainer->setLayout(new DockLayout());
	cookbookContainer->setHorizontalAlignment(HorizontalAlignment::Fill);

	// Set up and create the ListView for the recipe list
	mRecipeListView = createRecipeListView();
	mRecipeListView->setHorizontalAlignment(HorizontalAlignment::Fill);

	// Add the controls
	cookbookContainer->add(mRecipeListView);

	recipeListContainer->add(backgroundImage);
	recipeListContainer->add(cookbookContainer);

	recipeListPage->setContent(recipeListContainer);

	return recipeListPage;
}

ListView *CascadesCookbookApp::createRecipeListView() {
	// The list view and its item manager (where item are created and set up)
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

    if (UiValues::instance()->device() != UiValues::DEVICETYPE_720X720) {
        // The orientation recipe is not added for devices that does not support it.
        map["title"] = QString("Orientation");
        map["image"] = QString("asset:///images/items/cucumber.png");
        mRecipeModel << map;
    }

	map["title"] = QString("DockLayout");
	map["image"] = QString("assets/images/items/docklayout.png");
	mRecipeModel << map;
	map["title"] = QString("Animation");
	map["image"] = QString("assets/images/items/egg.png");
	mRecipeModel << map;
	map["title"] = QString("Menu");
	map["image"] = QString("assets/images/items/menu.png");
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
	map["title"] = QString("Dialog");
	map["image"] = QString("assets/images/items/sugarcandy.png");
	mRecipeModel << map;
	map["title"] = QString("GestureHandler");
	map["image"] = QString("assets/images/items/cookie.png");
	mRecipeModel << map;
	map["title"] = QString("ImagePaint");
	map["image"] = QString("assets/images/items/sugar_tile.png");
	mRecipeModel << map;
	map["title"] = QString("PixelBuffer");
	map["image"] = QString("assets/images/items/pixelbuffer.png");
	mRecipeModel << map;
	map["title"] = QString("RichText");
	map["image"] = QString("assets/images/items/alphabet_soup.png");
	mRecipeModel << map;
	map["title"] = QString("SegmentedControl");
	map["image"] = QString("assets/images/items/segmented_soup.png");
	mRecipeModel << map;
    map["title"] = QString("Invocation");
    map["image"] = QString("assets/images/items/chocolate.png");
    mRecipeModel << map;
	map["title"] = QString("Shortcuts");
	map["image"] = QString("assets/images/items/shortcut.png");
	mRecipeModel << map;
	map["title"] = QString("CustomPicker");
	map["image"] = QString("assets/images/items/custompicker.png");
	mRecipeModel << map;
	map["title"] = QString("Accessibility");
	map["image"] = QString("assets/images/items/sesame.png");
	mRecipeModel << map;
    map["title"] = QString("TextStyle");
    map["image"] = QString("assets/images/items/flour.png");
    mRecipeModel << map;
	recipeListView->setDataModel(&mRecipeModel);
	recipeListView->setListItemProvider(recipeItemManager);

	//recipeListView->setVerticalAlignment(VerticalAlignment::Fill);

	// Connect to the selection changed signal, used to update the recipe in the content area.
	connect(recipeListView, SIGNAL(triggered(const QVariantList)), this,
			SLOT(onTriggered(const QVariantList)));

	return recipeListView;
}

void CascadesCookbookApp::addApplicationCover() {
	// A small UI consisting of just an ImageView in a Container is set up
	// and used as the cover for the application when running in minimized mode.
	Container *coverContainer = Container::create().background(Color::fromRGBA(0,0,0,1));
	coverContainer->setLayout(new DockLayout());

	// A background image for the app cover.
	ImageView *coverImage = ImageView::create(
			"asset:///images/application-cover.png").scalingMethod(ScalingMethod::AspectFit);

	Container *titleContainer = Container::create().bottom(31);
	titleContainer->setHorizontalAlignment(HorizontalAlignment::Center);
	titleContainer->setVerticalAlignment(VerticalAlignment::Bottom);

	Container *labelContainer =
			Container::create().preferredSize(84, 42).background(
					Color::fromARGB(0xff121212));
	labelContainer->setLayout(new DockLayout());

	// A title for the "book" cover so that one can see that it is the
	// C++ version of the cookbook that is running.
	Label* title = Label::create("C++");
	title->textStyle()->setColor(Color::fromARGB(0xffebebeb));
	title->textStyle()->setFontSize(FontSize::PointValue);
	title->textStyle()->setFontSizeValue(6);
	title->setHorizontalAlignment(HorizontalAlignment::Center);
	title->setVerticalAlignment(VerticalAlignment::Bottom);

	// Setting up the title Container with a title
	labelContainer->add(title);
	titleContainer->add(labelContainer);

	// Adding the background image and the title Container
	coverContainer->add(coverImage);
	coverContainer->add(titleContainer);

	// Create a SceneCover and set the application cover
	SceneCover *sceneCover = SceneCover::create().content(coverContainer);
	Application::instance()->setCover(sceneCover);
}

void CascadesCookbookApp::onActionTriggerd() {

	//We create a SystemToast that shows that show's what you pressed
	bb::system::SystemToast* toast = new bb::system::SystemToast(this);

	if (sender()->objectName() == "Action") {
		toast->setBody("Salt Added");
	} else if (sender()->objectName() == "Setting") {
		toast->setBody("Settings");
	} else if (sender()->objectName() == "Help") {
		toast->setBody("Help");
	}

	toast->show();
}

void CascadesCookbookApp::onTopChanged(bb::cascades::Page* page) {
	if (page != mContentPage) {
		// Reset list selection when the main container Page is not the Content Page
		mRecipeListView->clearSelection();
	}

	// We only want the app to be in Portrait mode when viewing ListView
	OrientationSupport *support = OrientationSupport::instance();
	if (support->orientation() == UIOrientation::Landscape) {
		support->setSupportedDisplayOrientation(
				SupportedDisplayOrientation::DisplayPortrait);
	}
}

void CascadesCookbookApp::onPopTransitionEnded(bb::cascades::Page *page) {
	if (page == mContentPage) {

		// When the transition from the content page is complete, the recipe is removed.
		Container *content = qobject_cast<Container *>(mContentPage->content());

		if (content) {
			Control *recipeControl = content->at(0);

			// Remove the current recipe once we return to the ListView
			if (recipeControl && content->remove(recipeControl)) {
				delete recipeControl;
			}
		}
	}
}

void CascadesCookbookApp::onTriggered(const QVariantList indexPath) {
	CustomControl *recipe = NULL;

	// Get the selected item title
	QVariantMap map = mRecipeModel.data(indexPath).toMap();
	QString title = map.value("title").toString();

	// Create a recipe based on the selected item title
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
	} else if (title.compare("Dialog") == 0) {
		recipe = new CustomDialogRecipe();
	} else if (title.compare("GestureHandler") == 0) {
		recipe = new GestureHandlerRecipe();
	} else if (title.compare("ImagePaint") == 0) {
		recipe = new ImagePaintRecipe();
	} else if (title.compare("PixelBuffer") == 0) {
		recipe = new PixelBufferRecipe();
	} else if (title.compare("Menu") == 0) {
		recipe = new MenuRecipe();
	} else if (title.compare("RichText") == 0) {
		recipe = new RichTextRecipe();
	} else if (title.compare("SegmentedControl") == 0) {
		recipe = new SegmentedControlRecipe();
	} else if (title.compare("Invocation") == 0) {
        recipe = new InvocationRecipe();
    } else if (title.compare("Shortcuts") == 0) {
		recipe = new ShortcutRecipe();
	} else if (title.compare("CustomPicker") == 0) {
		recipe = new CustomPickerRecipe();
	} else if (title.compare("Accessibility") == 0) {
		recipe = new AccessabilityRecipe();
	} else if (title.compare("TextStyle") == 0) {
        recipe = new TextStyleRecipe();
    }

	if (recipe) {
		// Get the content Container of the ContentPage, add the new recipe, it will be removed when
		// the popTransitionEnded signal is received in the onPopTransitionEnded function.
		Container *content = qobject_cast<Container *>(mContentPage->content());

		if (content) {
			recipe->setHorizontalAlignment(HorizontalAlignment::Center);
			recipe->setVerticalAlignment(VerticalAlignment::Center);
			content->add(recipe);
			mContentPage->titleBar()->setTitle(title);
			mNavPane->push(mContentPage);
		} else {
			delete recipe;
		}

	} else {
		qDebug("No recipe created for this item yet.");
	}

}
