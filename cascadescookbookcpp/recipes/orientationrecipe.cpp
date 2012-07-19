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
#include "orientationrecipe.h"
#include <bb/cascades/OrientationSupport>


using namespace bb::cascades;

OrientationRecipe::OrientationRecipe(Container *parent) :
        CustomControl(parent)
{
    // Indicate that this recipe supports all display orientations.
    OrientationSupport::instance().setSupportedDisplayOrientation(SupportedDisplayOrientation::All);


    // Create the ImageView with an image in the assets folder as imageSource.
    mImage = ImageView::create("asset:///images/orientation/portrait.png");

    // Connect to the orientation changing and changed signals so that it
    // is possible to respond be altering the UI appropriately.
    connect(
         &OrientationSupport::instance(),
         SIGNAL(uiOrientationChanging(bb::cascades::UiOrientation::Type)),
         this,
         SLOT(onUiOrientationChanging(bb::cascades::UiOrientation::Type)));

     connect(
         &OrientationSupport::instance(),
         SIGNAL(uiOrientationChanged(bb::cascades::UiOrientation::Type)),
         this,
         SLOT(onUiOrientationChanged(bb::cascades::UiOrientation::Type)));

    setRoot(mImage);
}

void OrientationRecipe::onUiOrientationChanging(bb::cascades::UiOrientation::Type uiOrientation)
{
    // Nothing special is done when receiving this is signal, just print a debug message to console.
 	qDebug()<< (uiOrientation == UiOrientation::Landscape ?
        "Changing to Landscape!" : "Changing to Portrait!");
}

void OrientationRecipe::onUiOrientationChanged(bb::cascades::UiOrientation::Type uiOrientation)
{
    // Let's change the image depending on orientation.
    if(uiOrientation == UiOrientation::Landscape)
    	mImage->setImageSource(QUrl("asset:///images/orientation/landscape.png"));
    else
        mImage->setImageSource(QUrl("asset:///images/orientation/portrait.png"));
}
