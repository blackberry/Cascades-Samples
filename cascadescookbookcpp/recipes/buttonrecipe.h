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

#ifndef _BUTTONRECIPE_H_
#define _BUTTONRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Button;
    }
}

/* ButtonRecipe
 *
 * This recipe shows how to create a variety of different Buttons, both with and without images
 *
 */
class ButtonRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    ButtonRecipe(Container *parent = 0)
;
public slots:

// Slots for our two different buttons, called when the buttons are clicked.
void onNewButtonClicked();
void onEatButtonClicked();
void onRipenButtonClicked();

private:
Button *mFruitButton;
Button *mEatButton;
Button *mNewButton;

int mState;
};

#endif // ifndef _BUTTONRECIPE_H_
