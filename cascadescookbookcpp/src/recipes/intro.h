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

#ifndef _INTRO_H_
#define _INTRO_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
    }
}

/**
 * Intro Description:
 *
 * The application introduction recipe. Sets up a view with some text
 * explaining the purpose of the application and a small example UI
 * showing a couple of Cascades Core Controls.
 *
 */
class Intro: public bb::cascades::CustomControl
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the introduction recipe.
     * @param parent The parent Container.
     */
    Intro(Container *parent = 0);

private:
    /**
     * This function sets up a Container showcasing some of the Core Controls
     * available in Cascades. A text area, Button, Slider, and ToggleButton is
     * added on top of a nice looking Background image.
     *
     * @return A Container with a small example UI.
     */
    Container *setUpExampleUI();
};

#endif // ifndef _INTRO_H_
