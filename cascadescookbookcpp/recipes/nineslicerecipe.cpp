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
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

NineSliceRecipe::NineSliceRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    DockLayout *recipeLayout = new DockLayout();
    recipeContainer->setLayout(recipeLayout);
    recipeContainer->setPreferredSize(768, 1280);
 
    // This is the where the nine sliced image is put.
    Container *nineSliceContainer = new Container();
    DockLayout *nineSliceLayout = new DockLayout();
    nineSliceLayout->setTopPadding(35);
    nineSliceLayout->setRightPadding(100);
    nineSliceLayout->setLeftPadding(100);
    nineSliceContainer->setLayout(nineSliceLayout);
    nineSliceContainer->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Center));

    // This is the image that has nine slice margins applied to it,
    // thus scaling of this image will not cause scaling artifacts around
    // the image, but rather an area in the middle is scaled. The margins
    // that are applied can be found in the file border_image_Pasta.amd.
    ImageView *nineSliceImage =
            ImageView::create("asset:///images/nineslice/border_image_Pasta").layoutProperties(
                    DockLayoutProperties::create().vertical(VerticalAlignment::Fill).horizontal(
                            HorizontalAlignment::Fill));

    // The length of the text in this TextArea will effect its height,
    // which will in turn alter the height of the surrounding Container.
    // Since the ImageView above is set to fill the entire Container the
    // scaling of a nine sliced image is illustrated by switching between
    // on short (lazy) and long (diligent) recipe text (see RadioGroup below).
    Container *textContainer = new Container();
    StackLayout *textContainerLayout = new StackLayout();

    textContainerLayout->setTopPadding(40.0);
    textContainerLayout->setBottomPadding(110.0);
    textContainerLayout->setLeftPadding(30.0);
    textContainerLayout->setRightPadding(30.0);

    textContainer->setLayout(textContainerLayout);

    mRecipeText = new TextArea();
    mRecipeText->setText("1. Pour Mix.\n\n2. Add Water.\n\n3. Stir and heat.");
    mRecipeText->setEditable(false);

    mRecipeText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mRecipeText->textStyle()->setColor(Color::fromARGB(0x88000000)) ;

    textContainer->add(mRecipeText);

    nineSliceContainer->add(nineSliceImage);
    nineSliceContainer->add(textContainer);

    Container *controlPanel = createControlPanel();

    recipeContainer->add(nineSliceContainer);
    recipeContainer->add(controlPanel);

    setRoot(recipeContainer);
}

// To show how a nine sliced image is scaled we provide the possibility to
// alter the text between two different lengths. This will cause the text
// area to change size and consequently the image will be changing size.
Container *NineSliceRecipe::createControlPanel()
{
    Container *controlPanel = new Container();
    StackLayout *controlLayout = StackLayout::create().left(20).right(20);
    controlPanel->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Bottom));
    controlPanel->setLayout(controlLayout);

    RadioGroup *ninesliceOptions = new RadioGroup();
    Option *fastRecipe = new Option();
    fastRecipe->setText("Five Minutes");
    fastRecipe->setObjectName("fast");

    Option *slowRecipe = new Option();
    slowRecipe->setText("Two Hours");
    slowRecipe->setSelected(true);
    slowRecipe->setObjectName("slow");

    ninesliceOptions->add(fastRecipe);
    ninesliceOptions->add(slowRecipe);
    
    ninesliceOptions->setSelectedIndex(0);

    connect(ninesliceOptions, SIGNAL(selectedIndexChanged(int)), this,
            SLOT(selectedRecipeChanged(int)));

    controlPanel->add(new Divider());
    controlPanel->add(ninesliceOptions);
    
    return controlPanel;
}

void NineSliceRecipe::selectedRecipeChanged(int selected)
{
    RadioGroup* myRadioGroup = dynamic_cast<RadioGroup*>(sender());
    QString selectedName =  myRadioGroup->at(selected)->objectName();

    // Change the text in TextArea depending on which option was selected.
    if (selectedName.compare("fast") == 0) {
        mRecipeText->setText("1. Pour Mix.\n\n2. Add Water.\n\n3. Stir and heat.");
    } else {
        mRecipeText->setText(
                "1. Grind tomatoes.\n\n2. Fry minced meat.\n\n3. Add lasagna plates.\n\n4. Grind Cheese.\n\n5. Season with salt.");
    }
}
