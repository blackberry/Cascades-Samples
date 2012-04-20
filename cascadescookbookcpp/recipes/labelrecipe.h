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


#ifndef _LABELRECIPE_H_
#define _LABELRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
        class Font;
        class TextStyle;
        class Color;
    }
}

/**
 * LabelRecipe
 *
 * Show how to set up Labels with different system fonts.
 */
class LabelRecipe: public bb::cascades::CustomControl
{

public:
    LabelRecipe(Container *parent = 0);

private:
    /**
     * Helper function for setting up a label with a specific system font.
     *
     * @param labelText the text of the Label.
     * @param font the system font of the Label.
     * @param rightAlign if true align to the right if false use default alignment (left).
     */
    Label *setUpLabelWithStyle(const QString labelText, const TextStyle &textStyle,
            bool rightAlign, Color color, bool isBold);
};

#endif // ifndef _LABELRECIPE_H_
