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
#include "shortcutrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/StackLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/Shortcut>
#include <bb/cascades/SystemShortcut>
#include <bb/cascades/KeyListener>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/KeyEvent>

using namespace bb::cascades;

ShortcutRecipe::ShortcutRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The recipe container
    mRecipeContainer = new Container();
    mRecipeContainer->setLayout(new DockLayout());
    mRecipeContainer->setFocusPolicy(FocusPolicy::None);

    // Create a shortcut and connect it to the recipeContainer.
    // It will be used for changing the focus between the key image and the recipeContainer.
    Shortcut *shortcutFocus = Shortcut::create().key(tr("l"))
                                    .onTriggered(this, SLOT(onCShortcutFocusTriggered()));
    mRecipeContainer->addShortcut(shortcutFocus);

    // A container to layout the label and imageContainer in.
    Container *stackContainer = new Container();
    stackContainer->setTopPadding(ui->du(2));
    stackContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    stackContainer->setFocusPolicy(FocusPolicy::KeyAndTouch);

    // A Container for layout the images. Uses also spaceQuota to a predefined max height.
    Container *imageContainer = new Container();
    imageContainer->setLayout(new DockLayout());
    imageContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    imageContainer->setFocusPolicy(FocusPolicy::KeyAndTouch);
    imageContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1.0f));

    // Keyboard keys image, center aligned from start.
    mKeyImage = ImageView::create("asset:///images/shortcut/keys.png");
    mKeyImage->setHorizontalAlignment(HorizontalAlignment::Center);
    mKeyImage->setVerticalAlignment(VerticalAlignment::Center);
    mKeyImage->setFocusPolicy(FocusPolicy::None);

    // KeyListener for the keyImage.
    KeyListener *imagekeyListener = KeyListener::create()
                    .onKeyPressed(this, SLOT(onCKeyPressed(bb::cascades::KeyEvent *)))
                    .onKeyReleased(this, SLOT(onCKeyReleased(bb::cascades::KeyEvent *)));
    mKeyImage->addKeyListener(imagekeyListener);

    // System shortcut object for undo movement. Press U to use it.
    SystemShortcut *systemShortcutUndo = new SystemShortcut(SystemShortcuts::Undo);
    connectResult = connect(systemShortcutUndo, SIGNAL(triggered()), this, SLOT(onCSystemShortcutUndo()));
    Q_ASSERT(connectResult);

    // Add the shortcut to an image.
    mKeyImage->addShortcut(systemShortcutUndo);

    // Connects translation signals for the mKeyImage connected to the onCKeyImageTranslation handler.
    // If images translation equals position 0.0 the mUndoLabel will be hidden.
    connectResult = connect(mKeyImage, SIGNAL(translationXChanged(float)), this, SLOT(onCKeyImageTranslation()));
    Q_ASSERT(connectResult);

    connectResult = connect(mKeyImage, SIGNAL(translationYChanged(float)), this, SLOT(onCKeyImageTranslation()));
    Q_ASSERT(connectResult);

    // Connect the mkeyImage's focusChanged signal to a signal handler
    // to be able to reset control focus and change mlockImage properties.
    connectResult = connect(mKeyImage, SIGNAL(focusedChanged(bool)), this, SLOT(onKeyImageFocusChanged(bool)));
    Q_ASSERT(connectResult);

    // Lock icon image. Will be shown when the keyImages shortcuts are not usable.
    mLockImage = ImageView::create("asset:///images/shortcut/lockImage.png");
    mLockImage->setHorizontalAlignment(HorizontalAlignment::Center);
    mLockImage->setVerticalAlignment(VerticalAlignment::Center);
    mLockImage->setFocusPolicy(FocusPolicy::KeyAndTouch);
    mLockImage->setOpacity(0.0);

    // An animation to show the lock image when the page is started.
    FadeTransition *initLockAnim =
            FadeTransition::create(mLockImage).from(0).to(1).delay(200).duration(500).parent(this);

    // We connect to the end signal of the animation in order to set initial focus on the mRecipeContainer.
    // This is needed since we want to have the focus on the recipeContainer to be bale to use the shortcut
    // that will be triggered when we press the 'l'-key.
    connectResult = connect(initLockAnim, SIGNAL(ended()), this, SLOT(onInitLockAnimEnded()));
    Q_ASSERT(connectResult);

    // Add the images into the imageContainer. The container will later be added to the stackContainer.
    imageContainer->add(mKeyImage);
    imageContainer->add(mLockImage);

    // A Label with instructional text that the button should be pressed to unlock the example.
    mActionLabel = new Label();
    mActionLabel->setText((const QString) "Press 'L'-key to unlock the buttons.");
    mActionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mActionLabel->textStyle()->setColor(Color::Gray);
    mActionLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mActionLabel->setFocusPolicy(FocusPolicy::KeyAndTouch);

    // A Label with instructional text that the button should be tapped to undo the movement of the mKeyImage.
    mUndoLabel = new Label();
    mUndoLabel->setText((const QString) "Press 'U' to undo movement");
    mUndoLabel->setOpacity(0.0);
    mUndoLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mUndoLabel->textStyle()->setColor(Color::Gray);
    mUndoLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mUndoLabel->setFocusPolicy(FocusPolicy::None);

    // Add imageContainer, mActionLabel, mUndoLabel to the stackContainer.
    stackContainer->add(mActionLabel);
    stackContainer->add(imageContainer);
    stackContainer->add(mUndoLabel);

    // Add the controls to the recipeContainer and set it as the CustomControl root.
    mRecipeContainer->add(stackContainer);
    setRoot(mRecipeContainer);

    // Start the animation when the recipe is loaded.
    initLockAnim->play();

}

// Slot function for the shortcut for the mRecipeContainer.
void ShortcutRecipe::onCShortcutFocusTriggered()
{
    if (mKeyImage->isFocused() == 0) {
        mKeyImage->requestFocus();
    } else {
        mRecipeContainer->requestFocus();
    }
}

// Slot functions handling the mKeyImages keyListeners.
// Handler for key presses.
void ShortcutRecipe::onCKeyPressed(bb::cascades::KeyEvent *keyEvent)
{
    UIConfig *ui = this->ui();

    if (keyEvent->key() == 97) {
        mKeyImage->setImage("asset:///images/shortcut/keysA.png");
        mKeyImage->setTranslationX(mKeyImage->translationX() - ui->px(35.0));
    } else if (keyEvent->key() == 100) {
        mKeyImage->setImage("asset:///images/shortcut/keysD.png");
        mKeyImage->setTranslationX(mKeyImage->translationX() + ui->px(35.0));
    } else if (keyEvent->key() == 119) {
        mKeyImage->setImage("asset:///images/shortcut/keysW.png");
        mKeyImage->setTranslationY(mKeyImage->translationY() - ui->px(35.0));
    } else if (keyEvent->key() == 115) {
        mKeyImage->setImage("asset:///images/shortcut/keysS.png");
        mKeyImage->setTranslationY(mKeyImage->translationY() + ui->px(35.0));
    }
}

// Handler for key releases.
void ShortcutRecipe::onCKeyReleased(bb::cascades::KeyEvent * keyEvent)
{
    if ((keyEvent->key() == 97) || (keyEvent->key() == 100) || (keyEvent->key() == 119)
            || (keyEvent->key() == 115)) {
        mKeyImage->setImage("asset:///images/shortcut/keys.png");
    }
}

// Signal handler for the System shortcut.
void ShortcutRecipe::onCSystemShortcutUndo()
{
    // Move the keyImage to 0.0
    mKeyImage->setTranslation(0, 0);
}

// Signal handler for focused changed on the mKeyImage.
// To show or hide the mLookImage and redirect the focus is moved from the image.
void ShortcutRecipe::onKeyImageFocusChanged(bool newFocusedState)
{
    if (newFocusedState) {
        mLockImage->setOpacity(0.0);
        mActionLabel->setText(((const QString) "Press 'L'-key to lock the buttons again."));
    } else {
        // Sets the focus on the mRecipeContainer.
        mRecipeContainer->requestFocus();
        mLockImage->setOpacity(1.0);
        mActionLabel->setText(((const QString) "Press 'L'-key to unlock the buttons."));
    }
}

// Since we need to set focus inside the recipe from start we use the animations
// slot function to invoke the requestFocus function.
void ShortcutRecipe::onInitLockAnimEnded()
{
    // Sets the focus on the mRecipeContainer.
    mRecipeContainer->requestFocus();
}

// When the mKeyImage is moved around an instructional text will be visible.
void ShortcutRecipe::onCKeyImageTranslation()
{
    if (mKeyImage->translationX() == 0 && mKeyImage->translationY() == 0) {
        mUndoLabel->setOpacity(0.0);
    } else {
        mUndoLabel->setOpacity(1.0);
    }
}

