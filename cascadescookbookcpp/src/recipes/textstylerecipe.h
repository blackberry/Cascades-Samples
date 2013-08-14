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

#ifndef _TEXTSTYLERECIPE_H_
#define _TEXTSTYLERECIPE_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/FontSize>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
        class Font;
        class TextStyle;
    }
}

/**
 * TextStyleRecipe Description:
 *
 * This recipe show how to set up Labels with different system fonts.
 */
class TextStyleRecipe: public bb::cascades::CustomControl
{

public:
    /**
     * Constructor; sets up the TextStyleRecipe.
     * @param parent The parent Container, if not specified, 0 is used.
     */
    TextStyleRecipe(Container *parent = 0);

private:
    /**
     * Helper function for setting up a label with a specific style font.
     *
     * @param labelText The text of the Label.
     * @param textStyle The textStyle of the Label.
     * @return The created Label based on arguments.
     */
    Label *setUpLabelWithStyle(const QString labelText, const TextStyle &textStyle);

    /**
     * Helper function for setting up a label with a specific size.
     *
     * @param labelText The text of the Label.
     * @param size The size of the Label.
     * @return The created Label based on arguments.
     */
    Label *setUpLabelWithSize(const QString labelText, bb::cascades::FontSize::Type size);

    /**
     * Helper function for setting up a label with a specific style font.
     *
     * @param labelText The text of the Label.
     * @param fontFamily The system font of the Label.
     * @return The created Label based on arguments.
     */
    Label *setUpLabelWithFontFamily(const QString labelText,const QString & fontFamily);
};

#endif // ifndef _TEXTSTYLERECIPE_H_
