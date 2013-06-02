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
#include "nineslicerecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Divider>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

NineSliceRecipe::NineSliceRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the application UI values instance for accessing styling data
    mUiValues = UiValues::instance();

    Container *recipeContainer = new Container();
    DockLayout *recipeLayout = new DockLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredSize(mUiValues->intValue(UiValues::SCREEN_WIDTH), mUiValues->intValue(UiValues::SCREEN_HEIGHT));

    // Create a Container for the nine-sliced image.
    Container *nineSliceContainer = Container::create().top(mUiValues->intValue(UiValues::UI_PADDING_SMALL));
    nineSliceContainer->setLayout(new DockLayout());
    nineSliceContainer->setHorizontalAlignment(HorizontalAlignment::Center);

    // Obtain the image that has nine-slice margins applied to it.
    // Scaling of this image will not cause scaling artifacts around
    // the image, but rather scale an area in the middle. The margins
    // that are applied can be found in the file border_image_Pasta.amd.
    ImageView *nineSliceImage = ImageView::create(
            "asset:///images/nineslice/border_image_Pasta.amd");
    nineSliceImage->setVerticalAlignment(VerticalAlignment::Fill);
    nineSliceImage->setHorizontalAlignment(HorizontalAlignment::Fill);

    // The length of the text in this TextArea will effect its height,
    // which will alter the height of the surrounding Container.
    // Since the ImageView is set to fill the entire Container, the
    // scaling of a nine-sliced image is illustrated by switching between
    // short (lazy) and long (diligent) recipe text (see RadioGroup below).
    Container *textContainer = Container::create()
                .top(mUiValues->intValue(UiValues::UI_NINESLICERECIPE_LASANGAPADTOP))
                .bottom(mUiValues->intValue(UiValues::UI_NINESLICERECIPE_LASANGAPADBOTTOM))
                .left(mUiValues->intValue(UiValues::UI_NINESLICERECIPE_LASANGAPADSIDE))
                .right(mUiValues->intValue(UiValues::UI_NINESLICERECIPE_LASANGAPADSIDE));

    mRecipeText = new Label();
    mRecipeText->setMultiline(true);

    mRecipeText->textStyle()->setBase(mUiValues->textStyleValue(UiValues::UI_NINESLICERECIPE_LASANGATEXTSTYLE));
    mRecipeText->textStyle()->setColor(Color::fromARGB(0x88000000));
    mRecipeText->textStyle()->setLineHeight(0.85f);

    textContainer->add(mRecipeText);

    if(mUiValues->device() == UiValues::DEVICETYPE_720X720) {
        // If running on a square the text is aligned left to right in two columns instead of in a single column.
        textContainer->setLayout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
        mRecipeText2 = new Label();
        mRecipeText2->setMultiline(true);

        mRecipeText2->textStyle()->setBase(mUiValues->textStyleValue(UiValues::UI_NINESLICERECIPE_LASANGATEXTSTYLE));
        mRecipeText2->textStyle()->setColor(Color::fromARGB(0x88000000));
        mRecipeText2->textStyle()->setLineHeight(0.85f);

        textContainer->add(mRecipeText2);
    }

    nineSliceContainer->add(nineSliceImage);
    nineSliceContainer->add(textContainer);

    recipeContainer->add(nineSliceContainer);
    recipeContainer->add(createControlPanel());

    setRoot(recipeContainer);
}

// To show how a nine-sliced image is scaled, we provide the possibility to
// alter the text between two different lengths. This will cause the text
// area to change size and consequently the image to change size.
Container *NineSliceRecipe::createControlPanel()
{
    Container *controlPanel = Container::create()
                    .left(mUiValues->intValue(UiValues::UI_PADDING_STANDARD))
                    .right(mUiValues->intValue(UiValues::UI_PADDING_STANDARD))
                    .bottom(mUiValues->intValue(UiValues::UI_PADDING_STANDARD));
    controlPanel->setVerticalAlignment(VerticalAlignment::Bottom);

    RadioGroup *ninesliceOptions = new RadioGroup();
    Option *fastRecipe = new Option();
    fastRecipe->setText("Five Minutes");
    fastRecipe->setObjectName("fast");

    Option *slowRecipe = new Option();
    slowRecipe->setText("Two Hours");
    slowRecipe->setObjectName("slow");

    ninesliceOptions->add(fastRecipe);
    ninesliceOptions->add(slowRecipe);

    connect(ninesliceOptions, SIGNAL(selectedIndexChanged(int)), this,
            SLOT(selectedRecipeChanged(int)));

    ninesliceOptions->setSelectedIndex(1);

    if (mUiValues->device() != UiValues::DEVICETYPE_720X720) {
        // No room for a divider on the smaller resolution
        controlPanel->add(new Divider());
    }
    controlPanel->add(ninesliceOptions);

    return controlPanel;
}

void NineSliceRecipe::selectedRecipeChanged(int selected)
{
    RadioGroup* myRadioGroup = dynamic_cast<RadioGroup*>(sender());
    QString selectedName = myRadioGroup->at(selected)->objectName();

    if (mUiValues->device() == UiValues::DEVICETYPE_720X720) {
        // Change the text in TextArea depending on which option was selected.
        if (selectedName.compare("fast") == 0) {
            mRecipeText->setText("1. Pour Mix.\n\n2. Add Water.\n\n3. Stir and heat.");
            mRecipeText2->setText("");
        } else {
            mRecipeText->setText("1. Grind tomatoes.\n\n2. Fry minced meat.\n\n3. Add lasagna plates.");
            mRecipeText2->setText("4. Grind Cheese.\n\n 5. Season with salt.\n\n6. Voilˆ! Its ready!");
        }
    } else {
        // Change the text in TextArea depending on which option was selected.
        if (selectedName.compare("fast") == 0) {
            mRecipeText->setText("1. Pour Mix.\n\n2. Add Water.\n\n3. Stir and heat.");
        } else {
            mRecipeText->setText(
                    "1. Grind tomatoes.\n\n2. Fry minced meat.\n\n3. Add lasagna plates.\n\n4. Grind Cheese.\n\n5. Season with salt.");
        }
    }
}
