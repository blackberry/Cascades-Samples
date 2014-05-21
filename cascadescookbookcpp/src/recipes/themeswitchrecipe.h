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

#ifndef _THEMESWITCHRECIPE_H_
#define _THEMESWITCHRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class Option;
    }
}

/**
 * ThemeSwitchRecipe Description:
 *
 * ImagePaint is typically used for tileable images. A tileable
 * image is one that can be repeated as a pattern over a surface.
 * This is illustrated in this recipe by showing how a Container
 * background can be filled with tiled image patterns.
 *
 */
class ThemeSwitchRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the ImagePaint recipe.
     * @param parent A pointer to the parent Container.
     */
    ThemeSwitchRecipe(Container *parent = 0);

private slots:
    /**
     * This Slot function is used when connecting to the selectedOptionChanged
     * signal for the DropDown Control.
     *
     * @param selectedOption The selected Option.
     */
    void onSelectedOptionChanged(bb::cascades::Option* selectedOption);

private:
    // Recipe Container
    Container *mRecipeContainer;
};

#endif // ifndef _THEMESWITCHRECIPE_H_
