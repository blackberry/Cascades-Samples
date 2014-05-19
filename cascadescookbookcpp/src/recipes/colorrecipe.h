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

#ifndef _COLORRECIPE_H_
#define _COLORRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class Color;
    }
}

/**
 * ColorRecipe Description
 *
 * This recipe shows how Color can be set on text and Containers
 * and gives examples of colors used in the blackberry 10 platform.
 *
 */
class ColorRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the color recipe.
     * @param parent A pointer to the parent Container, @c if not specified, 0 is used
     */
    ColorRecipe(Container *parent = 0);

private:
    /**
     * Helper function that give the standard hex string representation
     * (like e.g #FFFF0000 for red) of a Color.
     *
     * @param Color the color for which a hex string is needed
     * @return The color in standard hex string format
     */
    QString colorToHexString(const Color color);

    /**
     * This function creates a Container containing another Container that
     * shows a rectangle with the specified color. That color is also used in the Label.
     *
     * @param myColor the color of the text and Container.
     * @return The created Container
     */
    Container* createLabel(bb::cascades::Color myColor);

    /**
     * This function creates a Container containing another Container that
     * shows a rectangle with the specified color. That color is also used in the Label.
     *
     * @param myColor the color of the text and Container.
     * @param background the background color of surrounding Container.
     * @return The created Container
     */
    Container* createLabelWithBackground(bb::cascades::Color myColor, bb::cascades::Color background);

    /**
     * This function creates a Container containing a colored icon and a text
     * for which color to typically use for representing the icon.
     *
     * @param myColor the color of the text and Container.
     * @param iconSrc The IconImage which should be shown.
     * @return The created Container
     */
    Container* createIcon(bb::cascades::Color myColor, const QString iconSrc);

};

#endif // ifndef _COLORRECIPE_H_
