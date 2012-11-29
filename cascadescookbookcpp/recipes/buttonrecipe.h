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
        class ImageButton;
    }
}

/* ButtonRecipe Description:
 *
 * This recipe shows you how to create a variety of different Buttons, both with and without images
 *
 */
class ButtonRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the button recipe
     * @param parent A pointer to the parent Container.
     */
    ButtonRecipe(Container *parent = 0);

public slots:

    /**
     * Slot function for when the "new" Button is clicked, so we reset Buttons to their original state.
     */
    void onNewButtonClicked();

    /**
     * Slot function for when the "eat" Button is clicked, so we change the text of this Button and
     * and hide the other Button.
     */
    void onEatButtonClicked();

    /**
     * Slot function for when the "ripen" Button is clicked, so depending on the state of mState, we adjust
     * the image on the Button.
     */
    void onRipenButtonClicked();

private:
    // Buttons
    Button *mFruitButton;
    Button *mEatButton;
    ImageButton *mNewButton;

    // State variable
    int mState;
};

#endif // ifndef _BUTTONRECIPE_H_
