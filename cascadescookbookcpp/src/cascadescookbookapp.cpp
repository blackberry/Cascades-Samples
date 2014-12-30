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
#include "recipes/gridlayoutrecipe.h"
#include "recipes/imagerecipe.h"
#include "recipes/imagepaintrecipe.h"
#include "recipes/inputrecipe.h"
#include "recipes/intro.h"
#include "recipes/invocationrecipe.h"
#include "recipes/menurecipe.h"
#include "recipes/nineslicerecipe.h"
#include "recipes/orientationrecipe.h"
#include "recipes/pixelbufferrecipe.h"
#include "recipes/progressindicatorrecipe.h"
#include "recipes/richtextrecipe.h"
#include "recipes/sliderrecipe.h"
#include "recipes/segmentedcontrolrecipe.h"
#include "recipes/shortcutrecipe.h"
#include "recipes/themeswitchrecipe.h"
#include "recipes/webviewrecipe.h"
#include "recipes/activityindicatorrecipe/activityindicatorrecipe.h"
#include "recipes/customdialogrecipe/customdialogrecipe.h"
#include "recipes/custompickerrecipe/custompickerrecipe.h"
#include "recipes/selectionrecipe/selection.h"
#include "recipes/sheetrecipe/sheetrecipe.h"
#include "recipes/stockcurverecipe/stockcurverecipe.h"
#include "recipes/accessabilityrecipe.h"
#include "recipes/textstylerecipe.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/ChromeVisibility>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/HelpActionItem>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Menu>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/OrientationSupport>
#include <bb/cascades/Page>
#include <bb/cascades/SceneCover>
#include <bb/cascades/SettingsActionItem>
#include <bb/cascades/StackLayout>
#include <bb/cascades/TextStyle>
#include <bb/cascades/Theme>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/TitleBar>
#include <bb/cascades/VisualStyle>
#include <bb/cascades/MultiCover>
#include <bb/cascades/ApplicationViewCover>

#include <bb/system/SystemToast>

CascadesCookbookApp::CascadesCookbookApp(QObject *parent) :
    QObject(parent), mRecipeModel()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // The application uses a navigation pane to set up a drill down list UI.
    // We also connect the topChanged and popTransitionEnded signals to the appropriate slot functions.
    mNavPane = new NavigationPane();
    connectResult = connect(mNavPane, SIGNAL(topChanged(bb::cascades::Page* )), this,
            SLOT(onTopChanged(bb::cascades::Page* )));
    Q_ASSERT(connectResult);

    connectResult = connect(mNavPane, SIGNAL(popTransitionEnded(bb::cascades::Page *)), this,
            SLOT(onPopTransitionEnded(bb::cascades::Page *)));
    Q_ASSERT(connectResult);

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
    Menu *menu =
            Menu::create().addAction(
                    ActionItem::create().objectName("Action").image(
                            Image("asset:///images/salt_icon.png")).connect(SIGNAL(triggered()),
                            this, SLOT(onActionTriggerd())).title("Add Salt")).settings(
                    SettingsActionItem::create().objectName("Setting").connect(SIGNAL(triggered()),
                            this, SLOT(onActionTriggerd()))).help(
                    HelpActionItem::create().objectName("Help").connect(SIGNAL(triggered()), this,
                            SLOT(onActionTriggerd())));
    Application::instance()->setMenu(menu);

    // We only want the app to be in Portrait mode when in the ListView is active.
    OrientationSupport::instance()->setSupportedDisplayOrientation(
            SupportedDisplayOrientation::DisplayPortrait);
}

CascadesCookbookApp::~CascadesCookbookApp()
{
    // If the content page does not have a parent, it is not shown in the
    // navigation pane so we have to delete it manually to avoid memory leaks.
    if (!mContentPage->parent()) {
        delete mContentPage;
    }
}

Page *CascadesCookbookApp::createContentPage()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Create a second page where the recipes (CustomControls) will be added.
    Page *page = new Page();

    // We need an action on the ContentPage, so we can navigate back.
    ActionItem* backAction = ActionItem::create();

    connectResult = connect(backAction, SIGNAL(triggered()), mNavPane, SLOT(pop()));
    Q_ASSERT(connectResult);

    // Set the back button action.
    page->setPaneProperties(NavigationPaneProperties::create().backButton(backAction));

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
    ThemeSupport *appThemeSupport = Application::instance()->themeSupport();
    connectResult = connect(appThemeSupport, SIGNAL(themeChanged(const bb::cascades::Theme *)),
            SLOT(onThemeChanged(const bb::cascades::Theme *)));
    Q_ASSERT(connectResult);

    return page;
}

void CascadesCookbookApp::onThemeChanged(const bb::cascades::Theme* theme)
{
    qDebug() << "Theme changed to " << theme->colorTheme()->style();
    Container *content = qobject_cast<Container *>(mContentPage->content());

    if (theme->colorTheme()->style() == VisualStyle::Dark) {
        content->setBackground(Color::Black);
    } else {
        ImagePaint paint(QUrl("asset:///images/background.amd"), RepeatPattern::XY);
        content->setBackground(paint);
    }
}

Page *CascadesCookbookApp::createRecipePage()
{
    Page *recipeListPage = new Page();

    // Create the main app Container and set a DockLayout which is used to align children
    Container *recipeListContainer = new Container();
    recipeListContainer->setLayout(new DockLayout());

    // The UIConfig object that gives access to device dependent conversion routines.
    UIConfig *ui = recipeListContainer->ui();

    // A nine-sliced book image is used as background of the cookbook.
    ImageView *backgroundImage = ImageView::create("asset:///images/Book_background.amd");
    backgroundImage->setVerticalAlignment(VerticalAlignment::Fill);
    backgroundImage->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Create and set up a Container for the list
    Container *cookbookContainer = new Container();
    cookbookContainer->setTopPadding(ui->px(15.0f));
    cookbookContainer->setBottomPadding(ui->px(15.0f));
    cookbookContainer->setLayout(new DockLayout());
    cookbookContainer->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Set up and create the ListView for the recipe list
    mRecipeListView = createRecipeListView();
    mRecipeListView->setScrollRole(ScrollRole::Main);
    mRecipeListView->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Add the controls
    cookbookContainer->add(mRecipeListView);

    recipeListContainer->add(backgroundImage);
    recipeListContainer->add(cookbookContainer);

    recipeListPage->setContent(recipeListContainer);

    return recipeListPage;
}

ListView *CascadesCookbookApp::createRecipeListView()
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // The list view and its item manager (where item are created and set up)
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
    map["image"] = QString("asset:///images/items/lemon.png");
    mRecipeModel << map;
    map["title"] = QString("TextStyle");
    map["image"] = QString("asset:///images/items/flour.png");
    mRecipeModel << map;
    map["title"] = QString("Picker");
    map["image"] = QString("asset:///images/items/banana.png");
    mRecipeModel << map;
    map["title"] = QString("DropDown");
    map["image"] = QString("asset:///images/items/beer.png");
    mRecipeModel << map;
    map["title"] = QString("ActivityIndicator");
    map["image"] = QString("asset:///images/items/orange_sliced.png");
    mRecipeModel << map;
    map["title"] = QString("ProgressIndicator");
    map["image"] = QString("asset:///images/items/peas_in_pod.png");
    mRecipeModel << map;
    map["title"] = QString("Color");
    map["image"] = QString("asset:///images/items/strawberries.png");
    mRecipeModel << map;

    if (!UiValues::instance()->aspectType() != bb::device::DisplayAspectType::Square) {
        // The orientation recipe is not added for devices that does not support it.
        map["title"] = QString("Orientation");
        map["image"] = QString("asset:///images/items/cucumber.png");
        mRecipeModel << map;
    }

    map["title"] = QString("DockLayout");
    map["image"] = QString("asset:///images/items/docklayout.png");
    mRecipeModel << map;
    map["title"] = QString("GridLayout");
    map["image"] = QString("asset:///images/items/onion.png");
    mRecipeModel << map;
    map["title"] = QString("Animation");
    map["image"] = QString("asset:///images/items/egg.png");
    mRecipeModel << map;
    map["title"] = QString("Menu");
    map["image"] = QString("asset:///images/items/menu.png");
    mRecipeModel << map;
    map["title"] = QString("Stock Curve");
    map["image"] = QString("asset:///images/items/leek.png");
    mRecipeModel << map;
    map["title"] = QString("WebView");
    map["image"] = QString("asset:///images/items/orange.png");
    mRecipeModel << map;
    map["title"] = QString("Sheet");
    map["image"] = QString("asset:///images/items/eggplant.png");
    mRecipeModel << map;
    map["title"] = QString("Dialog");
    map["image"] = QString("asset:///images/items/sugarcandy.png");
    mRecipeModel << map;
    map["title"] = QString("GestureHandler");
    map["image"] = QString("asset:///images/items/cookie.png");
    mRecipeModel << map;
    map["title"] = QString("ImagePaint");
    map["image"] = QString("asset:///images/items/sugar_tile.png");
    mRecipeModel << map;
    map["title"] = QString("PixelBuffer");
    map["image"] = QString("asset:///images/items/pixelbuffer.png");
    mRecipeModel << map;
    map["title"] = QString("RichText");
    map["image"] = QString("asset:///images/items/alphabet_soup.png");
    mRecipeModel << map;
    map["title"] = QString("SegmentedControl");
    map["image"] = QString("asset:///images/items/segmented_soup.png");
    mRecipeModel << map;
    map["title"] = QString("Invocation");
    map["image"] = QString("asset:///images/items/chocolate.png");
    mRecipeModel << map;

    if (UiValues::instance()->isPhysicalKeyboardDevice()) {
        map["title"] = QString("Shortcuts");
        map["image"] = QString("asset:///images/items/shortcut.png");
        mRecipeModel << map;
    }

    map["title"] = QString("CustomPicker");
    map["image"] = QString("asset:///images/items/custompicker.png");
    mRecipeModel << map;
    map["title"] = QString("Accessibility");
    map["image"] = QString("asset:///images/items/sesame.png");
    mRecipeModel << map;
    map["title"] = QString("Theme Switch");
    map["image"] = QString("asset:///images/items/pear.png");
    mRecipeModel << map;

    recipeListView->setDataModel(&mRecipeModel);
    recipeListView->setListItemProvider(recipeItemManager);

    // Connect to the selection changed signal, used to update the recipe in the content area.
    connectResult = connect(recipeListView, SIGNAL(triggered(const QVariantList)), this,
            SLOT(onTriggered(const QVariantList)));
    Q_ASSERT(connectResult);

    return recipeListView;
}
/*
 void CascadesCookbookApp::addApplicationCover()
 {
 // A small UI consisting of just an ImageView in a Container is set up
 // and used as the cover for the application when running in minimized mode.
 Container *coverContainer = Container::create().layout(DockLayout::create());

 // A background image for the app cover.
 ImagePaint imagePaint(QUrl("asset:///images/application-cover.amd"));
 coverContainer->setBackground(imagePaint);

 // The UIConfig object that gives access to device dependent conversion routines.
 UIConfig *ui = coverContainer->ui();

 // In order for the image to look good on a 720x720 device a bit of Padding is added to force the image to be scaled on 720x720.
 Container *pepperContainer = Container::create().top(ui->du(3)).vertical(
 VerticalAlignment::Bottom).horizontal(HorizontalAlignment::Center);

 ImageView *pepperImage =
 ImageView::create("asset:///images/active_frames_pepper.png").scalingMethod(
 ScalingMethod::AspectFit);

 Container *headerContainer = Container::create().layout(DockLayout::create()).horizontal(
 HorizontalAlignment::Fill).preferredHeight(ui->sdu(6.2));

 Container *headerBackground =
 Container::create().background(Color::fromARGB(0xff36412d)).opacity(0.5).horizontal(
 HorizontalAlignment::Fill).vertical(VerticalAlignment::Fill);

 // A title for the "book" cover so that one can see that it is the
 // C++ version of the cookbook that is running.
 Label* title = Label::create("C++");
 title->textStyle()->setColor(Color::fromARGB(0xffebebeb));

 // Some padding is added for title Label by adding it to a Container.
 Container *titleContainer = Container::create().left(ui->du(3)).vertical(
 VerticalAlignment::Center);
 titleContainer->add(title);

 // Setting up the title Container with a title and a header background.
 headerContainer->add(headerBackground);
 headerContainer->add(titleContainer);

 //Add the pepper image to the pepper Container.
 pepperContainer->add(pepperImage);

 // Adding the background images and the title Container.
 coverContainer->add(pepperContainer);
 coverContainer->add(headerContainer);

 // Create a SceneCover and set the application cover
 SceneCover *sceneCover = SceneCover::create().content(coverContainer);
 Application::instance()->setCover(sceneCover);
 }
 */

void CascadesCookbookApp::addApplicationCover()
{
    // A small UI consisting of just an ImageView in a Container is set up
    // and used as the cover for the application when running in minimized mode.
    Container *coverContainerHigh = Container::create().layout(DockLayout::create());

    // A background image for the app cover.
    ImagePaint imagePaint(QUrl("asset:///images/application-cover.amd"));
    coverContainerHigh->setBackground(imagePaint);

    // The UIConfig object that gives access to device dependent conversion routines.
    UIConfig *ui = coverContainerHigh->ui();

    // In order for the image to look good on a 720x720 device a bit of Padding is added to force the image to be scaled on 720x720.
    Container *pepperContainer = Container::create().top(ui->du(3)).vertical(
            VerticalAlignment::Bottom).horizontal(HorizontalAlignment::Center);

    ImageView *pepperImage =
            ImageView::create("asset:///images/active_frames_pepper.png").scalingMethod(
                    ScalingMethod::AspectFit);

    Container *headerContainer = Container::create().layout(DockLayout::create()).horizontal(
            HorizontalAlignment::Fill).preferredHeight(ui->sdu(6.2));

    Container *headerBackground =
            Container::create().background(Color::fromARGB(0xff36412d)).opacity(0.5).horizontal(
                    HorizontalAlignment::Fill).vertical(VerticalAlignment::Fill);

    // A title for the "book" cover so that one can see that it is the
    // C++ version of the cookbook that is running.
    Label* title = Label::create("C++");
    title->textStyle()->setColor(Color::fromARGB(0xffebebeb));

    // Some padding is added for title Label by adding it to a Container.
    Container *titleContainer = Container::create().left(ui->du(3)).vertical(
            VerticalAlignment::Center);
    titleContainer->add(title);

    // Setting up the title Container with a title and a header background.
    headerContainer->add(headerBackground);
    headerContainer->add(titleContainer);

    //Add the pepper image to the pepper Container.
    pepperContainer->add(pepperImage);

    // Adding the background images and the title Container.
    coverContainerHigh->add(pepperContainer);
    coverContainerHigh->add(headerContainer);

    // Create a SceneCover and set the application cover
    SceneCover *sceneCoverHigh = SceneCover::create().content(coverContainerHigh);


    // SceneCover medium.
    // A small UI consisting of just an ImageView in a Container is set up
    // and used as the cover for the application when running in minimized mode and low detail view.
    Container *coverContainerMedium = Container::create().layout(DockLayout::create()).background(Color::fromARGB(0xff060606));

    // The UIConfig object that gives access to device dependent conversion routines.
    UIConfig *uiMedium = coverContainerMedium->ui();

    Container *pepperContainerMedium = Container::create().vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Center);

    pepperContainerMedium->setMaxHeight(ui->px(150));

    ImageView *pepperImageMedium = ImageView::create("asset:///images/active_frames_pepper.png").scalingMethod(
            ScalingMethod::AspectFit);

    Container *headerContainerMedium = Container::create().layout(DockLayout::create()).horizontal(
            HorizontalAlignment::Fill).preferredHeight(uiMedium->sdu(6.2));

    Container *headerBackgroundMedium = Container::create().background(Color::fromARGB(0xff36412d)).opacity(0.5).horizontal(
                    HorizontalAlignment::Fill).vertical(VerticalAlignment::Fill);

    // A title for the "book" cover so that one can see that it is the
    // C++ version of the cookbook that is running.
    Label* titleMedium = Label::create("C++");
    titleMedium->textStyle()->setColor(Color::fromARGB(0xffebebeb));

    // Some padding is added for title Label by adding it to a Container.
    Container *titleContainerMedium = Container::create().left(uiMedium->du(3)).vertical(
            VerticalAlignment::Center);

    titleContainerMedium->add(titleMedium);

    // Setting up the title Container with a title and a header background.
    headerContainerMedium->add(headerBackgroundMedium);
    headerContainerMedium->add(titleContainerMedium);

    //Add the pepper image to the pepper Container.
    pepperContainerMedium->add(pepperImageMedium);

    // Adding the background images and the title Container.
    coverContainerMedium->add(pepperContainerMedium);
    coverContainerMedium->add(headerContainerMedium);

    SceneCover *sceneCoverMedium = SceneCover::create().content(coverContainerMedium);

    AbstractCover* cover = MultiCover::create().add(sceneCoverHigh, CoverDetailLevel::High).add(
            sceneCoverMedium, CoverDetailLevel::Medium);
    Application::instance()->setCover(cover);

}

void CascadesCookbookApp::onActionTriggerd()
{

    //We create a SystemToast that shows that shows what you pressed
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

void CascadesCookbookApp::onTopChanged(bb::cascades::Page* page)
{
    if (page != mContentPage) {
        // Reset list selection when the main container Page is not the Content Page
        mRecipeListView->clearSelection();
    }

    // We only want the app to be in Portrait mode when viewing ListView
    OrientationSupport *support = OrientationSupport::instance();
    if (support->orientation() == UIOrientation::Landscape) {
        support->setSupportedDisplayOrientation(SupportedDisplayOrientation::DisplayPortrait);
    }
}

void CascadesCookbookApp::onPopTransitionEnded(bb::cascades::Page *page)
{
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

void CascadesCookbookApp::onTriggered(const QVariantList indexPath)
{
    CustomControl *recipe = 0;
    // Get the content Container of the ContentPage, add the new recipe, it will be removed when
    // the popTransitionEnded signal is received in the onPopTransitionEnded function.
    Container *content = qobject_cast<Container *>(mContentPage->content());

    if (content) {
        // Get the selected item title
        QVariantMap map = mRecipeModel.data(indexPath).toMap();
        QString title = map.value("title").toString();
        bool resizePage = true;
        bool centerAlign = true;

        // Create a recipe based on the selected item title
        if (title.compare("Introduction") == 0) {
            recipe = new Intro(content);
            resizePage = false;
            centerAlign = false;
        } else if (title.compare("Image") == 0) {
            recipe = new ImageRecipe(content);
            centerAlign = false;
        } else if (title.compare("Nine Slice") == 0) {
            recipe = new NineSliceRecipe(content);
            centerAlign = false;
        } else if (title.compare("Button") == 0) {
            recipe = new ButtonRecipe(content);
            centerAlign = false;
        } else if (title.compare("Slider") == 0) {
            recipe = new SliderRecipe(content);
        } else if (title.compare("Selection") == 0) {
            recipe = new SelectionRecipe(content);
            centerAlign = false;
        } else if (title.compare("TextStyle") == 0) {
            recipe = new TextStyleRecipe(content);
            centerAlign = false;
        } else if (title.compare("Input") == 0) {
            recipe = new InputRecipe(content);
            centerAlign = false;
        } else if (title.compare("DockLayout") == 0) {
            recipe = new DockLayoutRecipe(content);
            centerAlign = false;
        } else if (title.compare("GridLayout") == 0) {
            recipe = new GridLayoutRecipe(content);
            centerAlign = false;
        } else if (title.compare("Color") == 0) {
            recipe = new ColorRecipe(content);
            centerAlign = false;
        } else if (title.compare("Orientation") == 0) {
            recipe = new OrientationRecipe(content);
        } else if (title.compare("Animation") == 0) {
            recipe = new AnimationRecipe(content);
            centerAlign = false;
        } else if (title.compare("Stock Curve") == 0) {
            recipe = new StockCurveRecipe(content);
            centerAlign = false;
        } else if (title.compare("Picker") == 0) {
            recipe = new DateTimePickerRecipe(content);
            centerAlign = false;
        } else if (title.compare("DropDown") == 0) {
            recipe = new DropDownRecipe(content);
            centerAlign = false;
        } else if (title.compare("ActivityIndicator") == 0) {
            recipe = new ActivityIndicatorRecipe(content);
            centerAlign = false;
        } else if (title.compare("ProgressIndicator") == 0) {
            recipe = new ProgressIndicatorRecipe(content);
        } else if (title.compare("WebView") == 0) {
            recipe = new WebViewRecipe(content);
            centerAlign = false;
        } else if (title.compare("Sheet") == 0) {
            recipe = new SheetRecipe(content);
        } else if (title.compare("Dialog") == 0) {
            recipe = new CustomDialogRecipe(content);
            centerAlign = false;
        } else if (title.compare("GestureHandler") == 0) {
            recipe = new GestureHandlerRecipe(content);
            centerAlign = false;
        } else if (title.compare("ImagePaint") == 0) {
            recipe = new ImagePaintRecipe(content);
            centerAlign = false;
        } else if (title.compare("PixelBuffer") == 0) {
            recipe = new PixelBufferRecipe(content);
        } else if (title.compare("Menu") == 0) {
            recipe = new MenuRecipe(content);
        } else if (title.compare("RichText") == 0) {
            recipe = new RichTextRecipe(content);
            centerAlign = false;
        } else if (title.compare("SegmentedControl") == 0) {
            recipe = new SegmentedControlRecipe(content);
            centerAlign = false;
        } else if (title.compare("Invocation") == 0) {
            recipe = new InvocationRecipe(content);
            centerAlign = false;
        } else if (title.compare("Shortcuts") == 0) {
            recipe = new ShortcutRecipe(content);
            centerAlign = false;
        } else if (title.compare("CustomPicker") == 0) {
            recipe = new CustomPickerRecipe(content);
            centerAlign = false;
        } else if (title.compare("Accessibility") == 0) {
            recipe = new AccessabilityRecipe(content);
            centerAlign = false;
        } else if (title.compare("Theme Switch") == 0) {
            recipe = new ThemeSwitchRecipe(content);
            centerAlign = false;
        }

        if (recipe) {
            mContentPage->titleBar()->setTitle(title);
            mNavPane->push(mContentPage);

            // Set the resize behavior of the Page.
            if (resizePage) {
                // Default behavior.
                mContentPage->setResizeBehavior(PageResizeBehavior::Resize);
            } else {
                // Don't resize when showing the virtual keyboard.
                mContentPage->setResizeBehavior(PageResizeBehavior::None);
            }

            if (centerAlign) {
                recipe->setVerticalAlignment(VerticalAlignment::Center);
                recipe->setHorizontalAlignment(HorizontalAlignment::Center);
            } else {
                recipe->setVerticalAlignment(VerticalAlignment::Fill);
                recipe->setHorizontalAlignment(HorizontalAlignment::Fill);
            }
        }
    } else {
        qDebug("No recipe created for this item yet.");
    }
}
