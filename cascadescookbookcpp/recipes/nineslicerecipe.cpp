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
    Container *recipeContainer = new Container();
    DockLayout *recipeLayout = new DockLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredSize(768, 1280);

    // Create a Container for the nine-sliced image.
    Container *nineSliceContainer = Container::create().top(35).right(100).left(100);
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
    Container *textContainer = Container::create().top(40.0).bottom(110.0).left(30.0).right(30.0);

    mRecipeText = new Label();
    mRecipeText->setMultiline(true);

    mRecipeText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mRecipeText->textStyle()->setColor(Color::fromARGB(0x88000000));
    mRecipeText->textStyle()->setLineHeight(0.85f);

    textContainer->add(mRecipeText);

    nineSliceContainer->add(nineSliceImage);
    nineSliceContainer->add(textContainer);

    Container *controlPanel = createControlPanel();

    recipeContainer->add(nineSliceContainer);
    recipeContainer->add(controlPanel);

    setRoot(recipeContainer);
}

// To show how a nine-sliced image is scaled, we provide the possibility to
// alter the text between two different lengths. This will cause the text
// area to change size and consequently the image to change size.
Container *NineSliceRecipe::createControlPanel()
{
    Container *controlPanel = Container::create().left(20).right(20);
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

    controlPanel->add(new Divider());
    controlPanel->add(ninesliceOptions);

    return controlPanel;
}

void NineSliceRecipe::selectedRecipeChanged(int selected)
{
    RadioGroup* myRadioGroup = dynamic_cast<RadioGroup*>(sender());
    QString selectedName = myRadioGroup->at(selected)->objectName();

    // Change the text in TextArea depending on which option was selected.
    if (selectedName.compare("fast") == 0) {
        mRecipeText->setText("1. Pour Mix.\n\n2. Add Water.\n\n3. Stir and heat.");
    } else {
        mRecipeText->setText(
                "1. Grind tomatoes.\n\n2. Fry minced meat.\n\n3. Add lasagna plates.\n\n4. Grind Cheese.\n\n5. Season with salt.");
    }
}
