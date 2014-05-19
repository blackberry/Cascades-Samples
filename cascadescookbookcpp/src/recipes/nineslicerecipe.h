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
#ifndef _NINESLICERECIPE_H_
#define _NINESLICERECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class Label;
        class Option;
    }
}

/**
 * NineSliceRecipe Description:
 *
 * This recipe shows how a nine-sliced image works. The nice thing
 * with images that can be nine sliced is that the size can be altered
 * without scaling the entire image. A file accompanying the image
 * contains margins, which will be unaffected by scaling. If setup
 * properly the image can scale without getting scaling artifacts and
 * distortions around the edges.
 */
class NineSliceRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the nine-sliced photo recipe.
     * @param parent The parent Container.
     */
    NineSliceRecipe(Container *parent = 0);

public slots:
    /**
     * Slot function connected to the RadioGroup presenting two different
     * options for the text placed on top of the nine-sliced image. Changing the text
     * will cause the TextArea to change size and consequently the image will be changing size as well.
     *
     * @param selected The newly selected option.
     */
    void selectedRecipeChanged(int selected);

private:
    Container *createControlPanel();

    Label *mRecipeText;
    Label *mRecipeText2;
};

#endif // ifndef _NINESLICERECIPE_H_
