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
#include "segmentedcontrolrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/ScrollView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/SegmentedControl>
#include <bb/cascades/Button>
#include <bb/cascades/ImageView>

using namespace bb::cascades;

SegmentedControlRecipe::SegmentedControlRecipe(Container *parent) :
		CustomControl(parent) {

	// A recipe showing how to listen to changes in SegmentedControl
	// and use this to change the look of the interface.

	ScrollView *scrollView = new ScrollView();
	ScrollViewProperties* scrollViewProp = scrollView->scrollViewProperties();
	scrollViewProp->setScrollMode(ScrollMode::Vertical);

	Container *recipeContainer = Container::create().left(20.0).right(20.0).bottom(20);
	recipeContainer->setHorizontalAlignment(HorizontalAlignment::Center);
	recipeContainer->setVerticalAlignment(VerticalAlignment::Top);

	StackLayout *recipeLayout = new StackLayout();
	recipeContainer->setLayout(recipeLayout);

	// This labels text is altered when the segmentedControl value is changed.
	mTitleLabel = new Label();
	mTitleLabel->setText("Potato Leek Soup");
	mTitleLabel->textStyle()->setBase(SystemDefaults::TextStyles::bigText());

	// This ImageView's imageSource is altered when the segmentedControl value is changed.
	mImage = ImageView::create("asset:///images/segmentedcontrol/soup_green.png");

	Label *ingredientsTitleLabel = new Label();
	ingredientsTitleLabel->setMultiline(true);
	ingredientsTitleLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
	ingredientsTitleLabel->setText("Ingredients");

	// The segmented control is implemented and given three values and text
	// to represent those values.
	SegmentedControl *segmented = SegmentedControl::create().add(
			bb::cascades::Option::create().text("Leek").value("1").selected(
					true)).add(
			bb::cascades::Option::create().text("Beetroot").value("2")).add(
			bb::cascades::Option::create().text("Garlic").value("3"));

	connect(segmented, SIGNAL(selectedIndexChanged(int)), this, SLOT(onSegmentedIndexChanged(int)));

	Label *ingredientsLabel = new Label();
	ingredientsLabel->setMultiline(true);
	ingredientsTitleLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
	ingredientsLabel->setText("3 cups chicken broth \n2-3 chopped potatoes \nSalt & Pepper");

	// Add the controls to the recipe Container and scrollView and set it as the root component.
	scrollView->setContent(recipeContainer);
	recipeContainer->add(mTitleLabel);
	recipeContainer->add(mImage);
	recipeContainer->add(ingredientsTitleLabel);
	recipeContainer->add(segmented);
	recipeContainer->add(ingredientsLabel);

	setRoot(scrollView);
}

// This is the slot that is triggered when the value is changed in segmented control.
// The index of the selected value is taken as a in parameter.
void SegmentedControlRecipe::onSegmentedIndexChanged(int selectedIndex) {

	SegmentedControl *segmented = dynamic_cast<SegmentedControl*>(sender());

	// Depending on what value is chosen the title and and image is
	// changed to match the ingredient of choice.
	if (segmented->selectedValue() == 1) {
		mTitleLabel->setText(QString("Potato Leek Soup"));
		mImage->setImageSource(
				QUrl("asset:///images/segmentedcontrol/soup_green.png"));
	} else if (segmented->selectedValue() == 2) {
		mTitleLabel->setText(QString("Borscht"));
		mImage->setImageSource(
				QUrl("asset:///images/segmentedcontrol/soup_beet.png"));
	} else if (segmented->selectedValue() == 3) {
		mTitleLabel->setText(QString("French Farmers Soup"));
		mImage->setImageSource(
				QUrl("asset:///images/segmentedcontrol/soup_white.png"));
	}
}
