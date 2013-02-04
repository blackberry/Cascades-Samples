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
#include "shortcutrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/StackLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/Shortcut>
#include <bb/cascades/FadeTransition>

using namespace bb::cascades;

ShortcutRecipe::ShortcutRecipe(Container *parent) :
        CustomControl(parent)
{
    // The recipe container
    mRecipeContainer = new Container();
    mRecipeContainer->setLayout(new DockLayout());
    mRecipeContainer->setFocusPolicy(FocusPolicy::None);
    mRecipeContainer->setPreferredSize(UiValues::instance()->intValue(UiValues::SCREEN_WIDTH),
            UiValues::instance()->intValue(UiValues::SCREEN_HEIGHT));

    // Create a shortcut and connect it to the recipeContainer.
    // It will be used for changing the focus between the key image and the recipeContainer.
    Shortcut *shortcutFocus = Shortcut::create().key(tr("l")).onTriggered(this,
            SLOT(onCShortcutFocusTriggered()));
    mRecipeContainer->addShortcut(shortcutFocus);

    // A container to layout the label and imageContainer in.
    Container *stackContainer = new Container();
    stackContainer->setLayout(new StackLayout());
    stackContainer->setTopPadding(UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD));
    stackContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    stackContainer->setFocusPolicy(FocusPolicy::KeyAndTouch);

    // A Container for layout the images. Uses also spaceQuota to a predefined max height.
    Container *imageContainer = new Container();
    imageContainer->setLayout(new DockLayout());
    imageContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    imageContainer->setFocusPolicy(FocusPolicy::KeyAndTouch);
    imageContainer->setMaxHeight(400.0f);
    imageContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1.0f));

    // Keyboard keys image, center aligned from start.
    mKeyImage = ImageView::create("asset:///images/shortcut/keys.png");
    mKeyImage->setHorizontalAlignment(HorizontalAlignment::Center);
    mKeyImage->setVerticalAlignment(VerticalAlignment::Center);
    mKeyImage->setFocusPolicy(FocusPolicy::None);

    // Shortcut objects for the image.
    Shortcut *shortcutUp = Shortcut::create().key(tr("w")).onTriggered(this,
            SLOT(onCShortcutUpTriggered()));
    mKeyImage->addShortcut(shortcutUp);

    Shortcut *shortcutDown = Shortcut::create().key(tr("s")).onTriggered(this,
            SLOT(onCShortcutDownTriggered()));
    mKeyImage->addShortcut(shortcutDown);

    Shortcut *shortcutLeft = Shortcut::create().key(tr("a")).onTriggered(this,
            SLOT(onCShortcutLeftTriggered()));
    mKeyImage->addShortcut(shortcutLeft);

    Shortcut *shortcutRight = Shortcut::create().key(tr("d")).onTriggered(this,
            SLOT(onCShortcutRightTriggered()));
    mKeyImage->addShortcut(shortcutRight);

    // Connect the mkeyImage's focusChanged signal to a signal handler
    // to be able to reset control focus and change mlockImage properties.
    connect(mKeyImage,SIGNAL(focusedChanged(bool)), this, SLOT(onKeyImageFocusChanged(bool)));

    // Lockicon image. Will be shown when the keyImages shortcuts are not usable.
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
    connect(initLockAnim, SIGNAL(ended()), this, SLOT(onInitLockAnimEnded()));

    // Add the images into the imageContainer. The container will later be added to the stackContainer.
    imageContainer->add(mKeyImage);
    imageContainer->add(mLockImage);

    // A Label with instructional text that the background should be tapped
    mActionLabel = new Label();
    mActionLabel->setText((const QString) "Press 'l'-key to unlock the buttons.");
    mActionLabel->textStyle()->setBase(SystemDefaults::TextStyles::bodyText());
    mActionLabel->textStyle()->setColor(Color::Gray);
    mActionLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    mActionLabel->setFocusPolicy(FocusPolicy::KeyAndTouch);

    // Add imageContainer and mActionLabel to the stackContainer.
    stackContainer->add(mActionLabel);
    stackContainer->add(imageContainer);

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

// Slot functions handling mKeyImage shortcuts.
void ShortcutRecipe::onCShortcutUpTriggered()
{
    mKeyImage->setTranslationY(mKeyImage->translationY() - 20.0);
}

void ShortcutRecipe::onCShortcutDownTriggered()
{
    mKeyImage->setTranslationY(mKeyImage->translationY() + 20.0);
}

void ShortcutRecipe::onCShortcutLeftTriggered()
{
    mKeyImage->setTranslationX(mKeyImage->translationX() - 20.0);
}

void ShortcutRecipe::onCShortcutRightTriggered()
{
    mKeyImage->setTranslationX(mKeyImage->translationX() + 20.0);
}

// Signal handler for focused changed on the mKeyImage.
// To show or hide the mLookImage and redirect the focus is moved from the image.
void ShortcutRecipe::onKeyImageFocusChanged(bool newFocusedState)
{
    if (newFocusedState) {
        mLockImage->setOpacity(0.0);
        mActionLabel->setText(((const QString) "Press 'l'-key to lock the buttons again."));
    } else {
        // Sets the focus on the mRecipeContainer.
        mRecipeContainer->requestFocus();
        mLockImage->setOpacity(1.0);
        mActionLabel->setText(((const QString) "Press 'l'-key to unlock the buttons."));
    }
}

// Since we need to set focus inside the recipe from start we use the animations
// slot function to invoke the requestFocus function.
void ShortcutRecipe::onInitLockAnimEnded()
{
    // Sets the focus on the mRecipeContainer.
    mRecipeContainer->requestFocus();
}

