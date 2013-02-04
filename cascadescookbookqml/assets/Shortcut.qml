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
import bb.cascades 1.0
import "Common"

// This recipe shows how shortcut objects can be created and connected to controls.
// Shortcuts provide a way to perform some action using a predefined input trigger.
// In this example we use the keyboard as input trigger.
// A controls shortcut does only work when the control is in focus.

RecipePage {
    RecipeContainer {
        id: recipeCon
        focusPolicy: FocusPolicy.None

        // Shortcuts property for the recipeCon. It takes a list of shortcut objects, in this case there is just one object.
        shortcuts: [
            Shortcut {
                key: "l"
                onTriggered: {
                    if (keyImage.focused == false) {
                        // Set focus on the image.
                        keyImage.requestFocus();
                    } else {
                        // Set focus on the RecipeContainer.
                        recipeCon.requestFocus();
                    }
                }
            }
        ]

        Container {
            id: stackContainer
            topPadding: 20
            focusPolicy: FocusPolicy.KeyAndTouch
            horizontalAlignment: HorizontalAlignment.Fill

            Label {
                id: tapLabel
                focusPolicy: FocusPolicy.None
                text: (keyImage.focused == 0) ? "Press 'l'-key to unlock the buttons." : "Press 'l'-key to lock the buttons again."
                horizontalAlignment: HorizontalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.DarkGray
                }
            }

            Container {
                id: imageContainer
                layout: DockLayout {
                }
                maxHeight: 400
                focusPolicy: FocusPolicy.KeyAndTouch
                horizontalAlignment: HorizontalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                // Keyboard image and it´s shotcuts.
                ImageView {
                    id: keyImage
                    focusPolicy: FocusPolicy.None
                    imageSource: "asset:///images/shortcut/keys.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    // Shortcuts for the image.
                    shortcuts: [
                        Shortcut {
                            key: "a"
                            onTriggered: {
                                keyImage.translationX -= 20;
                            }
                        },
                        Shortcut {
                            key: "d"
                            onTriggered: {
                                keyImage.translationX += 20;
                            }
                        },
                        Shortcut {
                            key: "w"
                            onTriggered: {
                                keyImage.translationY -= 20;
                            }
                        },
                        Shortcut {
                            key: "s"
                            onTriggered: {
                                keyImage.translationY += 20;
                            }
                        }
                    ]

                    // This signal handler sets the opacity for the lock image.
                    // Maybe more important, it set focus on the recipeCon when focus is moved from the keyImage.
                    onFocusedChanged: {
                        if (focused) {
                            lockImage.opacity = 0
                        } else {
                            lockImage.opacity = 1
                            recipeCon.requestFocus();
                        }
                    }
                } // ImageView

                ImageView {
                    id: lockImage
                    focusPolicy: FocusPolicy.KeyAndTouch
                    imageSource: "asset:///images/shortcut/lockImage.png"
                    opacity: 0
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    animations: FadeTransition {
                        id: initLockAnim
                        fromOpacity: 0
                        toOpacity: 1
                        delay: 200
                        duration: 500
                        onEnded: {
                            // We can't request focus directly when the page is loaded. Normally one could use on creation completed but not is this case.
                            // Please see the api for more specific information.
                            // https://developer.blackberry.com/cascades/reference/bb__cascades__control.html#requestfocus

                            // Because of this we start an animation, see below, on the signal "onCreationCompleted" which in case works fine,
                            // and when that has been played we can use the animations signal handler "onEnded" to set the focus on the receipeContainer.
                            // We need to set focus on the recipeContainer to be able to use its shortcut object.
                            recipeCon.requestFocus();
                        }
                    }
                } // ImageView

            } // imageContainer
        } // stackContainer
        onCreationCompleted: {
            initLockAnim.play();
        }
    } // recipeContainer
}// RecipePage
