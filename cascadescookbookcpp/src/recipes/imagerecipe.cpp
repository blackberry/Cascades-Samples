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
#include "imagerecipe.h"

#include <bb/cascades/CheckBox>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/DockLayout>

using namespace bb::cascades;

ImageRecipe::ImageRecipe(Container *parent) :
		CustomControl(parent) {
	// Get the UIConfig object in order to use resolution independent sizes.
	UIConfig *ui = this->ui();

	// Create the ImageView with an image in the assets folder as imageSource.
	Container *recipeContainer = Container::create().top(ui->du(2)).left(
			ui->du(2)).right(ui->du(2));

    DockLayout *recipeContainerLayout = new DockLayout();
    recipeContainer->setLayout(recipeContainerLayout);

	mImage = ImageView::create("asset:///images/peach_big.png").scalingMethod(
			ScalingMethod::AspectFit);
	mImage->setHorizontalAlignment(HorizontalAlignment::Center);
	mImage->setVerticalAlignment(VerticalAlignment::Center);

	// A checkbox so that we can turn on and off the filterColor property
	CheckBox *filterColorCheckBox = CheckBox::create().text("filterColor");
	filterColorCheckBox->setVerticalAlignment(VerticalAlignment::Bottom);

	connect(filterColorCheckBox, SIGNAL(checkedChanged(bool)), this,
			SLOT(onCheckedChanged(bool)));

	recipeContainer->add(mImage);
	recipeContainer->add(filterColorCheckBox);

	setRoot(recipeContainer);
}

void ImageRecipe::onCheckedChanged(bool checked) {

	// When the checked state changes for the CheckBox, a signal is
	// caught in this slot function and we change the filterColor property of the ImageView.
	if (checked) {
		mImage->setFilterColor(Color::fromARGB(0xff9b617d));
	} else {
		mImage->setFilterColor(QVariant()); //to clear the property we feed it a uninitialized qvariant.
	}
}
