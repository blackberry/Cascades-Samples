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
 * OrientationRecipe
 * 
 * When creating an UI it is often the case that it has to be able to be shown in more
 * then one orientations. Some modification to the UI is often necessary when for example
 * switching between Landscape and Portrait mode. In this recipe it is shown how to set
 * what orientation an app or a page supports and how to listen to orientation changes.
 */
class OrientationRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
	OrientationRecipe(Container *parent = 0);

private slots:
    /**
     * Slot for signals sent out just as the orientation is changing.
     *
     * @param uiOrientation the type of orientation changing either Portrait or Landscape
     */
	void onUiOrientationChanging(bb::cascades::UiOrientation::Type uiOrientation);

	/**
	 * Slot for signals sent out when the orientation has changed.
	 *
	 * @param uiOrientation the type of orientation, either Portrait or Landscape
	 */
	void onUiOrientationChanged(bb::cascades::UiOrientation::Type uiOrientation);

private:
	ImageView *mImage;
};

#endif // ifndef _ORIENTATIONRECIPE_H_
