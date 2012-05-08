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
#include "labelrecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

LabelRecipe::LabelRecipe(Container *parent) :
        CustomControl(parent)
{
    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setLayoutDirection(LayoutDirection::LeftToRight);
    recipeContainer->setLayout(recipeLayout);

    // Here all the available bold type fonts are stacked in a Container.
    Container *headerStyleContainer = new Container();
    headerStyleContainer->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(
                    VerticalAlignment::Center));

    headerStyleContainer->setRightMargin(60);
 
    headerStyleContainer->add(
            setUpLabelWithStyle((const QString) "BigText", SystemDefaults::TextStyles::bigText(),
                    true, Color::Gray , true));
    headerStyleContainer->add(
            setUpLabelWithStyle((const QString) "TitleText", SystemDefaults::TextStyles::titleText(),
                    true, Color::Gray, true));
    headerStyleContainer->add(
            setUpLabelWithStyle((const QString) "BodyText", SystemDefaults::TextStyles::bodyText(),
                    true, Color::Gray, true));
    headerStyleContainer->add(
            setUpLabelWithStyle((const QString) "SubtitleText", SystemDefaults::TextStyles::subtitleText(),
                    true, Color::Gray, true ));
    headerStyleContainer->add(
            setUpLabelWithStyle((const QString) "SmallText", SystemDefaults::TextStyles::smallText(),
                    true, Color::Gray, true));

    // The second stacked Container show the regular style fonts.
    Container *paragraphStyleContainer = new Container();

    paragraphStyleContainer->add(
            setUpLabelWithStyle((const QString) "BigText", SystemDefaults::TextStyles::bigText(),
                    false, Color::Gray, false));
    paragraphStyleContainer->add(
            setUpLabelWithStyle((const QString) "TitleText", SystemDefaults::TextStyles::titleText(),
                    false, Color::Gray, false));
    paragraphStyleContainer->add(
            setUpLabelWithStyle((const QString) "BodyText", SystemDefaults::TextStyles::bodyText(),
                    false, Color::Gray, false));
    paragraphStyleContainer->add(
            setUpLabelWithStyle((const QString) "SubtitleText", SystemDefaults::TextStyles::subtitleText(),
                    false, Color::Gray, false));
    paragraphStyleContainer->add(
            setUpLabelWithStyle((const QString) "SmallText", SystemDefaults::TextStyles::smallText(),
                    false, Color::Gray, false));
 
    recipeContainer->add(headerStyleContainer);
    recipeContainer->add(paragraphStyleContainer);

    setRoot(recipeContainer);
}

Label *LabelRecipe::setUpLabelWithStyle(const QString labelText, const TextStyle &textStyle,
        bool rightAlign, Color color,bool isBold)
{
    Label *label = new Label();
    label->setText(labelText);
    label->textStyle()->setBase(textStyle);

    // Add some space below the Label.
    label->setBottomMargin(50.0f);
    label->textStyle()->setColor(color) ;
    
    if(isBold)
            label->textStyle()->setFontWeight(FontWeight::Bold);
    // We have the option of setting the alignment to the right since
    // Containers with Labels and we like on of the Containers to contain right aligned Labels.
    if (rightAlign == true) {
        label->setLayoutProperties(
                StackLayoutProperties::create().horizontal(HorizontalAlignment::Right));
    }

    return label;
}
