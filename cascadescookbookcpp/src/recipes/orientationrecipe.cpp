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
#include "orientationrecipe.h"
#include <bb/cascades/OrientationSupport>

using namespace bb::cascades;

OrientationRecipe::OrientationRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Indicate that this recipe supports all display orientations.
    OrientationSupport::instance()->setSupportedDisplayOrientation(
            SupportedDisplayOrientation::All);

    // Create the ImageView with an image in the assets folder as imageSource.
    mImage = ImageView::create("asset:///images/orientation/portrait.png");

    // Connect to the orientation changing and changed signals so that it
    // is possible to respond to altering orientation in the UI appropriately.
    connectResult = connect(OrientationSupport::instance(),
            SIGNAL(orientationAboutToChange(bb::cascades::UIOrientation::Type)), this,
            SLOT(onUIOrientationChanging(bb::cascades::UIOrientation::Type)));
    Q_ASSERT(connectResult);

    connectResult = connect(OrientationSupport::instance(),
            SIGNAL(orientationChanged(bb::cascades::UIOrientation::Type)), this,
            SLOT(onUIOrientationChanged(bb::cascades::UIOrientation::Type)));
    Q_ASSERT(connectResult);

    setRoot(mImage);
}

void OrientationRecipe::onUIOrientationChanging(bb::cascades::UIOrientation::Type uiOrientation)
{
    // Change the image depending on orientation.
    if (uiOrientation == UIOrientation::Landscape) {
        mImage->setImageSource(QUrl("asset:///images/orientation/landscape.png"));
    } else {
        mImage->setImageSource(QUrl("asset:///images/orientation/portrait.png"));
    }
}

void OrientationRecipe::onUIOrientationChanged(bb::cascades::UIOrientation::Type uiOrientation)
{
    // Nothing special is done when receiving this is signal, just print a debug message to console.
    QString orientationString = (uiOrientation == UIOrientation::Landscape ? "Changed to Landscape!" : "Changing to Portrait!");
    qDebug() << orientationString;

}
