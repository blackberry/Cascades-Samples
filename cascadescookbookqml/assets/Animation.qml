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

// This recipe shows how to create your own animations and transitions
// on visual items.
Page {
    property bool hideAnimStopped: false
    property bool showAnimStopped: false

    content: RecipeContainer {
        id: animationRecipe
        property int eggOffset: 460
        preferredWidth: 768

        // The top Container contains the eggs that will be animated.
        Container {

            layout: AbsoluteLayout {
            }

            // Background image.
            ImageView {
                id: "animationBackground"
                imageSource: "asset:///images/dark_background.png"
                preferredWidth: 768
                preferredHeight: 780
            }

            // The two "super" eggs, two big eggs stacked side by side.
            // This entire Container i scaled by an implicit animation when the toggle is switched.
            Container {
                id: superEggs
                layout: StackLayout {
                    layoutDirection: StackLayout.LeftToRight
                }
                layoutProperties: AbsoluteLayoutProperties {
                    positionY: animationRecipe.eggOffset
                }

                // When scaling the entire Container down it should be done on a point corresponding
                // to left edge. That is half the preferred width of the parent Container.
                pivotX: -animationRecipe.preferredWidth / 2;

                // Two eggs, the eggs are set up in the Egg.qml file
                AnimationEgg {
                }
                AnimationEgg {
                }
            }

            // This egg will be animated in on the scene when the toggle is switched.
            AnimationEgg {
                id: moreEgg
                layoutProperties: AbsoluteLayoutProperties {
                    positionY: animationRecipe.eggOffset
                }

                // The egg starts outside the screen in x direction and is already scaled down.
                translationX: animationRecipe.preferredWidth
                scaleX: 0.7
                scaleY: 0.7;

                // Show and hide animations of the egg.
                animations: [
                    TranslateTransition {
                        id: showEgg
                        toX: 430;       // The egg slides in from outside the screen.
                        duration: 600
                        onStopped: {
                            playStoppedAnimations ();
                        }
                    },
                    TranslateTransition {
                        id: hideEgg
                        toX: animationRecipe.preferredWidth;       // The egg slides out in the opposite direction of the show animation.
                        duration: 600
                        onEnded: {
                            // At the end of the animation the the super eggs are scaled back to their original size.
                            superEggs.scaleX = 1.0;
                            superEggs.scaleY = 1.0;
                        }
                        onStopped: {
                            playStoppedAnimations ();
                        }
                    }
                ]
            }
        }

        // The control Container, contains a toggle button that switches between the two modes.
        Container {
            layout: DockLayout {
                leftPadding: 30
            }
            
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            preferredHeight: 360
            background: Color.create (0.84, 0.84, 0.84);

            // A recipe text.
            Container {

                layout: StackLayout {
                    topPadding: 42;                    
                }
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Top
                    horizontalAlignment: HorizontalAlignment.Left
                }
                Label {
                    bottomMargin: 32
                    text: "Scrambled eggs"
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        color: Color.Black
                    }
                }
                Label {
                    text: "1. Take two eggs."
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }                
                }
                Label {
                    text: "2. Scramble them."
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }
                }
                Label {
                    text: "3. Done."
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }
                }
            }

            // A Label and a toggle button is stacked together and aligned in the
            // bottom right corner
            Container {
                layout: StackLayout {
                    bottomPadding: 45
                    rightPadding: 30
                }
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Bottom
                    horizontalAlignment: HorizontalAlignment.Right
                }

                ToggleButton {
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                    onCheckedChanged: {

                        // Triggering animations is done here!
                        if (checked == true) {
                            // Scale the eggs down to create a zoom effect. The animations
                            // are done using the built in implicit animations.
                            superEggs.scaleX = 0.7;
                            superEggs.scaleY = 0.7;

                            // Stop all ongoing animations to avoid conflicts. If the functions return
                            // true this means the an animation was stopped and we have to wait til the
                            // onStopped function in the animation to start the show animation.
                            showAnimStopped = checkPlayingAnimations ();
                            if (showAnimStopped == false) {
                                // Show the eggs and call the tilt function in the Egg.qml file to wiggle the egg.
                                showEgg.play ()
                                moreEgg.tilt ();
                            }
                        } else {
                            // Stop all ongoing animations to avoid conflicts
                            hideAnimStopped = checkPlayingAnimations ();
                            if (hideAnimStopped == false) {
                                // Hide the extra egg, when this animation ends the super eggs are scaled back to their original size.
                                hideEgg.play ();
                            }
                        }
                    }
                }
                Label {                    
                    text: "Super size"
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }

                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
            }
        }
    }

    // This function checks if any of the animations are running. If that is the
    // case the animation is stopped and true is returned to notify the caller.
    function checkPlayingAnimations () {
        var animationWasStopped = false;
        if (showEgg.isPlaying ()) {
            showEgg.stop ();
            animationWasStopped = true;
        }
        if (hideEgg.isPlaying ()) {
            hideEgg.stop ();
            animationWasStopped = true;
        }
        return animationWasStopped;
    }

    // This function is called from the animations onStopped function.
    function playStoppedAnimations () {
        if (showAnimStopped) {
            showEgg.play ()
            moreEgg.tilt ();
        } else if (hideAnimStopped) {
            hideEgg.play ();
        }

        hideAnimStopped = false;
        showAnimStopped = false;
    }
}
