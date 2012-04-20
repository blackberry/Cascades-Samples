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
#include "activityindicatorrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/Textstyle>
#include <bb/cascades/Systemdefaults>
#include <bb/cascades/Label>
#include <bb/cascades/TextArea>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/StackLayout>
#include <bb/cascades/Button>
#include <bb/cascades/ImageView>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>

using namespace bb::cascades;

ActivityIndicatorRecipe::ActivityIndicatorRecipe(Container *parent) :
CustomControl(parent)
{
    // The recipe Container.
    Container *recipeContainer = new Container();
    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setLeftPadding(20.0);
    recipeLayout->setRightPadding(20.0);
    recipeContainer->setLayout(recipeLayout);
    
    // The introduction text.
    TextArea *introText = new TextArea();
    introText->setText((const QString) "This is a milk boiling simulator recepie");
    introText->setEditable(false);
    introText->textStyle()->setColor(Color::Gray);
    introText->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    introText->setBottomMargin(100);
     
    Container* smashContainer = new Container();
    smashContainer->setLayout(new DockLayout());
    
    
    // This the big image that was taking during the night
    // it's at the same position as the day one, but further from the viewer.
    mUnbroken = ImageView::create("asset:///images/stockcurve/egg.png");
    
    // Center it using dock layout info.
    mUnbroken->setLayoutProperties( DockLayoutProperties::create()
                                   .horizontal(HorizontalAlignment::Center)
                                   .vertical(VerticalAlignment::Center));
    
    // Since this image is on top of the night one, we can hide the
    // night image with changing the opacity value of this image.
    mBroken = ImageView::create("asset:///images/stockcurve/broken_egg.png").opacity(0.0);
    
    // Center it using dock layout info.
    mBroken->setLayoutProperties( DockLayoutProperties::create()
                                 .horizontal(HorizontalAlignment::Center)
                                 .vertical(VerticalAlignment::Center));
    
    mActivityIndicator = new ActivityIndicator();
    mActivityIndicator->setPreferredSize(130, 130);
    
    smashContainer->add(mUnbroken);
    smashContainer->add(mActivityIndicator);
    smashContainer->add(mBroken);
     
    mButton = new Button();
    mButton->setTopMargin(100);
    mButton->setText((const QString) "start cooking");
    connect(mButton, SIGNAL(clicked()), this, SLOT(onClicked()));
    
    
    // Add the controls to the recipe Container and set it as root.
    recipeContainer->add(introText);
    recipeContainer->add(smashContainer);
    
    recipeContainer->add(mButton);
    
    setRoot(recipeContainer);
    
    
}
void ActivityIndicatorRecipe::onClicked()
{ 
    
    if(mButton->text() == "start cooking")
    {
        mActivityIndicator->start();
        mButton->setText((const QString) "look away");
    }
    else
    {  
        mActivityIndicator->stop();
        mButton->setEnabled(false);
        mButton->setText((const QString) "clean up"); 
        mUnbroken->setOpacity(0.0);
        mBroken->setOpacity(1.0);
    }
    
}

