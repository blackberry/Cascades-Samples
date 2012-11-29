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

#ifndef _ORIENTATIONRECIPE_H_
#define _ORIENTATIONRECIPE_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/ImageView>
#include <bb/cascades/OrientationSupport>

using namespace bb::cascades;

/** 
 * OrientationRecipe Description:
 * 
 * When creating an UI, often the UI has to be able to be shown in multiple orientations.
 * Some modification to the UI is often necessary when switching orientations, for example
 * switching between Landscape and Portrait mode. In this recipe, it is shown how to set
 * what orientation an app or a page supports and how to listen to orientation changes.
 */
class OrientationRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the orientation recipe.
     * @param parent The parent Container
     */
    OrientationRecipe(Container *parent = 0);

private slots:
    /**
     * This Slot function is for signals sent out just as the orientation is changing.
     *
     * @param uiOrientation The type of orientation changing either Portrait or Landscape
     */
    void onUIOrientationChanging(bb::cascades::UIOrientation::Type uiOrientation);

    /**
     * This Slot function is for signals sent out when the orientation has changed.
     *
     * @param uiOrientation The type of orientation, either Portrait or Landscape
     */
    void onUIOrientationChanged(bb::cascades::UIOrientation::Type uiOrientation);

private:
    ImageView *mImage;
};

#endif // ifndef _ORIENTATIONRECIPE_H_
