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
#include "sheetrecipe.h"
#include "fruititemfactory.h"
#include "uivalues.h"

#include <bb/cascades/ActionItem>
#include <bb/cascades/Button>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DataModel>
#include <bb/cascades/DockLayout>
#include <bb/cascades/GridListLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/Page>
#include <bb/cascades/ScrollView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/Sheet>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TapHandler>
#include <bb/cascades/TextField>
#include <bb/cascades/TextStyle>
#include <bb/cascades/TitleBar>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/XmlDataModel>

using namespace bb::cascades;

SheetRecipe::SheetRecipe(Container *parent) :
        CustomControl(parent)
{
    // Set up the initial UI content of the recipe Page
    Container *recipeContainer = setUpRecipeContent();

    // Create the Sheets for later viewing
    setUpModifySheet();
    setUpFruitSheet();

    setRoot(recipeContainer);
}

void SheetRecipe::onModifyRecipe(bb::cascades::TapEvent* tapEvent)
{
    Q_UNUSED(tapEvent);

    // Show the first sheet by setting its visibility to true
    mSheetModify->open();
}

void SheetRecipe::onNewFruitTap(bb::cascades::TapEvent* tapEvent)
{
    Q_UNUSED(tapEvent);

    // The user has pressed the fruit and wants to select a new one,
    // so show a second Sheet on top of this one.
    mSheetFruit->open();
}

void SheetRecipe::onModifySave()
{
    // Save modification has occurred so we hide the Sheet, update the fruit,
    // and update the greetings text, provided that the text has length greater than 0.
    mFruitImage->setImageSource(mModifyFruitImage->imageSource());
    if (mGreetingsText.length() > 0) {
        mGreetingsLabel->setText(mGreetingsText);
    }

    // Hide the sheet
    mSheetModify->close();
}

void SheetRecipe::onModifyCancel()
{
    // Cancel has been pressed, emit cancel (will hide the sheet).
    mSheetModify->close();
}

void SheetRecipe::onNewFruitCancel()
{
    // Cancel fruit selection, so we hide this Sheet.
    mSheetFruit->close();
}

void SheetRecipe::onNewFruitChanged(const QVariantList indexPath)
{
    ListView *listView = dynamic_cast<ListView*>(sender());

    if (listView) {
        // A new fruit has been selected, update the currently selected
        // fruit on the modification sheet. The user might still cancel
        // the modification, so we do not update the fruitImagePath until
        // the save signal from that Sheet has been received.
        DataModel *dataModel = listView->dataModel();
        QVariantMap map = dataModel->data(indexPath).toMap();
        QString newFruitPath = map.value("fruit").toString();
        mModifyFruitImage->setImageSource(newFruitPath);
        mSheetFruit->close();
    }
}

void SheetRecipe::onTextChanging(const QString &newText)
{
    // The new greetings text is stored so the main text can be updated
    // if the save button is hit.
    mGreetingsText = newText;
}

Container *SheetRecipe::setUpRecipeContent()
{
    // The bowl of fruit; the contents can be altered by showing the mSheetModify Sheet.
    Container *recipeContainer = new Container();

    mGreetingsLabel = Label::create("Tap fruit for Sheet.");
    mGreetingsLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mGreetingsLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());
    mGreetingsLabel->textStyle()->setFontWeight(FontWeight::W200);

    Container *basketContainer = Container::create().layout(DockLayout::create())
                    .topMargin(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));

    // The fruit is put inside a Container to offset it with the bottom padding and
    // is aligned correctly (otherwise it would be hidden by the basket image below).
    Container *fruitContainer = Container::create().layout(StackLayout::create());
    fruitContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    fruitContainer->setVerticalAlignment(VerticalAlignment::Center);
    fruitContainer->setBottomPadding(160);
    mFruitImage = ImageView::create("assets/images/sheet/fruit5.png");

    fruitContainer->add(mFruitImage);

    ImageView *basketImage = ImageView::create("asset:///images/sheet/basket.png");
    basketImage->setHorizontalAlignment(HorizontalAlignment::Center);
    basketImage->setVerticalAlignment(VerticalAlignment::Bottom);

    basketContainer->add(fruitContainer);
    basketContainer->add(basketImage);

    // Add a tap handler to the basket Container, so that a Sheet can be shown when
    // the fruit basket is tapped.
    TapHandler *tapHandler = TapHandler::create().onTapped(this,
            SLOT(onModifyRecipe(bb::cascades::TapEvent*)));

    basketContainer->addGestureHandler(tapHandler);

    recipeContainer->add(mGreetingsLabel);
    recipeContainer->add(basketContainer);

    return recipeContainer;

}

void SheetRecipe::setUpModifySheet()
{
    // A Sheet used for modifying the Sheet recipe Page. This is the
    // first Sheet in the drill-down navigation from the Sheet recipe.
    mSheetModify = Sheet::create();
    mSheetModify->setParent(this);

    Page *page = Page::create();

    // The cancel and add/update actions for the page title bar
    ActionItem* cancelAction = ActionItem::create().title("Cancel");
    ActionItem* saveAction = ActionItem::create().title("Save");

    connect(cancelAction, SIGNAL(triggered()), this, SLOT(onModifyCancel()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(onModifySave()));

    TitleBar *modifyBar = TitleBar::create().title("Modify").acceptAction(saveAction).dismissAction(
            cancelAction);

    page->setTitleBar(modifyBar);

    // The buttons for cancel and add/update actions
    Container *modifyContent = Container::create()
                .top(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .right(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));

    // A text field for changing the greetings text
    TextField *greetingsField = TextField::create().bottomMargin(150).hintText(
            "Enter greetings text");
    greetingsField->textStyle()->setBase(SystemDefaults::TextStyles::titleText());

    connect(greetingsField, SIGNAL(textChanging(const QString &)), this,
            SLOT(onTextChanging(const QString &)));

    // Set up the Image that will trigger drill down to another Sheet if tapped. To get
    // the user to understand that the image can be tapped, some decoration is needed. So
    // a background image with a nice drop shadow, an overlaid text, and the image are combined to create this effect.
    Container *imageSelectionContainer = Container::create().layout(DockLayout::create());
    imageSelectionContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    ImageView *backPlate =
            ImageView::create("asset:///images/title_gui_buffet_empty_box.amd").preferredWidth(170);
    backPlate->setHorizontalAlignment(HorizontalAlignment::Fill);
    backPlate->setVerticalAlignment(VerticalAlignment::Fill);

    Container *imageContainer = Container::create().left(4).right(4).bottom(6);
    mModifyFruitImage = ImageView::create("asset:///images/sheet/fruit5.png");
    mModifyFruitImage->setHorizontalAlignment(HorizontalAlignment::Center);

    Container *instructionContainer = Container::create()
                .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .right(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .top(10)
                .bottom(10)
                .background(Color::fromARGB(0xaa272727));

    instructionContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    instructionContainer->setVerticalAlignment(VerticalAlignment::Fill);

    Label *instruction = Label::create("Tap to change");
    instruction->setHorizontalAlignment(HorizontalAlignment::Center);
    instruction->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    instruction->textStyle()->setColor(Color::LightGray);

    instructionContainer->add(instruction);
    imageContainer->add(mModifyFruitImage);
    imageContainer->add(instructionContainer);

    imageSelectionContainer->add(backPlate);
    imageSelectionContainer->add(imageContainer);

    TapHandler *tapHandler = TapHandler::create().onTapped(this,
            SLOT(onNewFruitTap(bb::cascades::TapEvent*)));

    imageSelectionContainer->addGestureHandler(tapHandler);

    // Add the two different components to the Sheet content: Input field and image control.
    modifyContent->add(greetingsField);
    modifyContent->add(imageSelectionContainer);

    page->setContent(modifyContent);
    mSheetModify->setContent(page);

}

void SheetRecipe::setUpFruitSheet()
{
    mSheetFruit = Sheet::create();
    mSheetFruit->setParent(this);

    Page *page = Page::create();

    Container *newFruitContainer = Container::create()
                .top(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .left(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD))
                .right(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));

    // The cancel action for the page title bar
    ActionItem* cancelAction = ActionItem::create().title("Cancel");

    connect(cancelAction, SIGNAL(triggered()), this, SLOT(onNewFruitCancel()));

    TitleBar *modifyBar = TitleBar::create().title("Modify").dismissAction(cancelAction);

    page->setTitleBar(modifyBar);

    // An XMLModel is used for the small grid list of fruits, this
    // kind of data model is suitable for small lists with static content.
    XmlDataModel *model = new XmlDataModel(this);
    model->setSource(QUrl("../assets/models/sheetmodel.xml"));

    // A grid list with some fruits that can be put in the fruit basket.
    // Spacequota needs to be set in order for the list not to take precedence
    // in the layouting of the page (if not set it will fill the entire screen).
    ListView *listView = new ListView(model);
    listView->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    GridListLayout *listLayout = GridListLayout::create().columnCount(2).cellAspectRatio(1.6);
    listView->setLayout(listLayout);

    FruitItemFactory *itemfactory = new FruitItemFactory();
    listView->setListItemProvider(itemfactory);

    connect(listView, SIGNAL(triggered(const QVariantList)), this,
            SLOT(onNewFruitChanged(const QVariantList)));

    newFruitContainer->add(listView);

    page->setContent(newFruitContainer);

    mSheetFruit->setContent(page);
}

