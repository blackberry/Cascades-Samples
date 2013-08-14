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
#include "richtextrecipe.h"

#include <bb/cascades/ImageView>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>

using namespace bb::cascades;

RichTextRecipe::RichTextRecipe(Container *parent) :
		CustomControl(parent)
{
	// This recipe shows how to use certain HTML and CSS elements to style and layout text.

	ScrollView *scrollView = new ScrollView();
	ScrollViewProperties* scrollViewProp = scrollView->scrollViewProperties();
	scrollViewProp->setScrollMode(ScrollMode::Vertical);

	Container *recipeContainer = new Container();
	StackLayout *recipeLayout = new StackLayout();
	recipeLayout->setOrientation(LayoutOrientation::TopToBottom);
	recipeContainer->setLayout(recipeLayout);
	recipeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);

	// This container holds all the text.
	Container *textContainer = new Container();
	textContainer->setLeftPadding(20);
	textContainer->setRightPadding(20);
	textContainer->setBottomPadding(20);
	textContainer->setHorizontalAlignment(HorizontalAlignment::Center);

	Label *title = new Label();
	title->setMultiline(true);
	// The HTML tag is used in the string to start using CSS/HTML.
	// In the paragraph tag after that "style" is used to activate CSS properties.
	title->setText("<html><p style='color: #262626; font-size: x-large; line-height: 0.8;'><br/>Alphabet Soup with CSS</p></html>");

	// Ingredients text
	Label *ingredients = new Label();
	ingredients->setMultiline(true);
	// Here is an example of using several paragraphs to change row.
	// The first paragraph has different styling then the others.
	// The "span" tag is used to insert different styling within a paragraph.
	// For your benefit we've split the string into several lines, concatenating them with "" so they are easier for you to read.
	ingredients->setText("<html><p style='font-size: large; color: #d60000; line-height: 1.2;' >Ingredients</p>"
								"<p style='font-size: medium;'><p>Alphabet Soup - <span style='font-style: italic; color: #555555;'>1 can </span></p>"
								"<p>HTML tags - <span style='font-style: italic; color: #555555;'>4 teaspoons</span></p>"
								"<p>Body text - <span style='font-style: italic; color: #555555;'>3 cups </span></p>"
								"CSS properties - <span style='font-style: italic; color: #555555;'>For seasoning </span></p></html>");

	// Directions text
	Label *directions = new Label();
	directions->setMultiline(true);
	// Some more examples of different CSS properties.
	directions->setText("<html><p style='font-size: large; color: #d60000; line-height: 1.2;'>Directions</p>"
								"<p style='font-size: medium;'>Open can, heat up soup and eat. Use css to create some awesome text styling like "
								"<span style='color: #e6b400; background-color: #262626; font-weight: bolder;'> this,"
								"</span> <span style=' color: #0073bc; text-decoration: underline;'>this</span> or"
								"<span style='font-style: italic; color: #555555; font-size: x-small; font-weight: lighter;'> this</span>.</p></html>");

	ImageView *image = ImageView::create("asset:///images/alphabet_soup.png");

	scrollView->setContent(recipeContainer);
	recipeContainer->add(textContainer);
	textContainer->add(title);
	textContainer->add(ingredients);
	textContainer->add(directions);
	textContainer->add(image);
	setRoot(scrollView);
}
