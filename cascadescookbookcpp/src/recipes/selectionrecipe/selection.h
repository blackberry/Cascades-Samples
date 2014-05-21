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
#ifndef _SELECTIONRECIPE_H_
#define _SELECTIONRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Option;
    }
}

/**
 * SelectionRecipe Description:
 *
 * In this recipe we show how the RadioGroup and CheckBox Controls are
 * used to compose a delicious mix of olives.
 */
class SelectionRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; Sets up the Labels, SelectionCheckBox, RadioGroup, and other components
     * for the SelectionRecipe sample.
     *
     * @param parent The parent Container, if not specified, 0 is used.
     */
    SelectionRecipe(Container * parent = 0);

public slots:
    /**
     * This Slot function is connected to the RadioGroup presenting the different
     * fillings of olives when the selection changes. It is called when a new selection
     * is made in the group. The function will also perform a debug printout to console of the new filling.
     *
     * @param selected The newly selected option.
     */
    void fillingSelectedOptionChanged(int selected);

};

#endif // ifndef _SELECTIONRECIPE_H_
