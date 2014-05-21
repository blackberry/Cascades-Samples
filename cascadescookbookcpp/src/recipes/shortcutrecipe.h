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
#ifndef _SHORTCUTRECIPE_H_
#define _SHORTCUTRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class ImageView;
        class KeyEvent;
        class Label;
    }
}

/**
 * ShortcutRecipe Description:
 *
 * This recipe shows how shortcut objects can be created and connected to controls.
 * Shortcuts provide a way to perform some action using a predefined input trigger.
 * In this example we use the keyboard as input trigger.
 * A controls shortcuts do only work when the control is in focus.
 *
 */
class ShortcutRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the dock shortcut recipe.
     * @param parent A pointer to the parent Container, @c if not specified, 0 is used
     */
    ShortcutRecipe(Container *parent = 0);

public slots:

    /**
     * This Slot function is called when the focus is set on the recipeContainer and the 'l'-key is pressed.
     * When called, the function alters the focus from the recipeContainer to the keyImage.
     */
    void onCShortcutFocusTriggered();

    /**
     * Slot function connected to the keyImages keyListeners.
     * When called, they will move the keyImage.
     */
    void onCKeyPressed(bb::cascades::KeyEvent *keyEvent);
    void onCKeyReleased(bb::cascades::KeyEvent *keyEvent);

    void onCSystemShortcutUndo();

    void onCKeyImageTranslation();

    /**
     * Slot function connected to the keyIamages focusChanged signal.
     *
     * @param value The current focus state of the keyImage.
     */
    void onKeyImageFocusChanged(bool newFocusedState);

    /**
     * Slot function connected to the InitLockAnim's ended signal.
     */
    void onInitLockAnimEnded();

private:
    Container *mRecipeContainer;
    Label *mActionLabel;
    Label *mUndoLabel;
    ImageView *mKeyImage;
    ImageView *mLockImage;
};

#endif // ifndef _SHORTCUTRECIPE_H_
