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
#include "accessabilityrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/Option>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SegmentedControl>

using namespace bb::cascades;

AccessabilityRecipe::AccessabilityRecipe(Container *parent) :
		CustomControl(parent) {
	Container *recipeContainer = Container::create().top(30.0).left(50.0).right(
			50.0);

	// Introductionary label, explaining how to start the screenreader
	Label *descriptionLabel = new Label();
	descriptionLabel->setMultiline(true);
	descriptionLabel->setText(
			(const QString) "Start the screenreader by pressing the powerbutton 3 times. Listen to accessibility data either by tapping or dragging on the screen. Double tap anywhere to execute an action \n\n/The Chef");

 	// Label of the soup image, will be used for accessibility functionality.
	Label *soupLabel = new Label();
	soupLabel->setText("Image of the tasty soup");
	soupLabel->textStyle()->setFontSize(FontSize::XSmall);

	// Image of the soup
	ImageView* soupImage = new ImageView();
	soupImage->setImage(Image("asset:///images/mushroom_soup.png"));
	//This adds additional information about the image that the screenreader utilizes.
	soupImage->accessibility()->addLabel(soupLabel);

	// This container contains the segmented control and a label that let's you change the units of the recipe
	Container *selectionContainer = Container::create().top(25.0);
	StackLayout *selectionContainerLayout = new StackLayout();
	selectionContainerLayout->setOrientation(LayoutOrientation::LeftToRight);
	selectionContainer->setLayout(selectionContainerLayout);

	Label *unitLabel = new Label();
	unitLabel->setText("Choose Unit");
	unitLabel->setVerticalAlignment(VerticalAlignment::Center);
	unitLabel->setLayoutProperties(
			StackLayoutProperties::create().spaceQuota(1));

	SegmentedControl* segmentedControl = SegmentedControl::create().add(
			bb::cascades::Option::create().text("Metric").value("1").selected(
					true)).add(
			bb::cascades::Option::create().text("US").value("2"));
	segmentedControl->setLayoutProperties(
			StackLayoutProperties::create().spaceQuota(1));

	connect(segmentedControl, SIGNAL(selectedIndexChanged(int)), this,
			SLOT(onSegmentedIndexChanged(int)));

	selectionContainer->add(unitLabel);
	selectionContainer->add(segmentedControl);

	// A section of ingredients and instructions.
	Label *ingridientsLabel = new Label();
	ingridientsLabel->setTopMargin(50);
	ingridientsLabel->setText("Ingredients:");
	ingridientsLabel->textStyle()->setFontSize(FontSize::Large);
	ingridientsLabel->textStyle()->setFontWeight(FontWeight::Bold);

	mMuschroomLabel = new Label();
	mMuschroomLabel->setText("200g of mushrooms.");

	mCreamLabel = new Label();
	mCreamLabel->setText("1 dl of cream.");

	Label *saltLabel = new Label();
	saltLabel->setText("a pinch of salt.");

	Label *parsnipLabel = new Label();
	parsnipLabel->setText("3 straws of parsnip.");
	//setting a description can add value to a controller if it needs more info that is hidden for someone hard of seeing.
	parsnipLabel->accessibility()->setDescription(
			"Get three straws of parsnip that are about 10cm in length.");

	// A section That describes the method.
	Label *preparationMethodLabel = new Label();
	preparationMethodLabel->setTopMargin(50);
	preparationMethodLabel->setText("Preparation Method:");
	preparationMethodLabel->textStyle()->setFontSize(FontSize::Large);
	preparationMethodLabel->textStyle()->setFontWeight(FontWeight::Bold);

	Label *todoLabel = new Label();
	todoLabel->setText(
			"Put all in a mixer, \nMix until smooth. \nHeat on stove until hot. \nEnjoy");
	todoLabel->setMultiline(true);

	// Add the Controls top the recipe Container.
	recipeContainer->add(descriptionLabel);
	recipeContainer->add(soupImage);
	recipeContainer->add(soupLabel);
	recipeContainer->add(selectionContainer);

	recipeContainer->add(ingridientsLabel);
	recipeContainer->add(mMuschroomLabel);
	recipeContainer->add(mCreamLabel);
	recipeContainer->add(saltLabel);
	recipeContainer->add(parsnipLabel);
	recipeContainer->add(todoLabel);

	ScrollView *scrollRecipe = ScrollView::create().scrollMode(
			ScrollMode::Vertical).content(recipeContainer);
	setRoot(scrollRecipe);

}

// This is the slot that is triggered when the value is changed in segmented control.
// The index of the selected value is taken as a in parameter.
void AccessabilityRecipe::onSegmentedIndexChanged(int selectedIndex) {

	SegmentedControl *segmented = dynamic_cast<SegmentedControl*>(sender());

	if (segmented->selectedValue() == 1) {
		mMuschroomLabel->setText("200g of mushrooms.");
		mCreamLabel->setText("1 dl of cream.");

	} else if (segmented->selectedValue() == 2) {
		mMuschroomLabel->setText("7 ounces of mushrooms.");
		mCreamLabel->setText("Half a cup of cream.");

	}
}
