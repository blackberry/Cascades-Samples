/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2
import "Common"

// This recipe shows how to create your own animations and transitions
// on visual items.
RecipePage {
    RecipeContainer {
        layout: StackLayout {
        }

        // The top Container contains the eggs that will be animated.
        Container {
            id: eggContainer
            background: backgroundPaint.imagePaint
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            layout: DockLayout {
            }

            // The two "super" eggs are two big eggs stacked side by side.
            // This entire Container is scaled by an implicit animation when the toggle is switched.
            Container {
                id: superEggs
                property real eggScale: 1.0
                property int eggWidth: eggLayoutHandler.layoutFrame.width
                
                leftPadding: 20
                topPadding: 80
                verticalAlignment: VerticalAlignment.Bottom
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                
                // Two eggs; the eggs are set up in the AnimationEgg.qml file.
                AnimationEgg {
                    pivotX: - superEggs.eggWidth * 0.5
                    scaleX: superEggs.eggScale
                    scaleY: superEggs.eggScale
                    clipContentToBounds: false

                    // LayoutUpdateHandler is used to get the width of the egg image.
                    attachedObjects: LayoutUpdateHandler {
                        id: eggLayoutHandler
                    }
                }
                
                AnimationEgg {
                    pivotX: - superEggs.eggWidth * 1.5
                    scaleX: superEggs.eggScale
                    scaleY: superEggs.eggScale
                    clipContentToBounds: false
                }
                
                // This egg will be animated into the scene when the toggle is switched.
                AnimationEgg {
                    id: moreEgg
                    leftMargin: 20
                    clipContentToBounds: false

                    // The egg starts outside the screen in the x direction and is already scaled down.
                    scaleX: 0.7
                    scaleY: 0.7;

                    layoutProperties: StackLayoutProperties {
                        // A positive space quota will in this case force the third egg outside the 
                        // screen. It will later be animated to a negative translation so that it is visible.
                        spaceQuota: 1
                    }

                    // Show and hide animations of the egg.
                    animations: [
                        TranslateTransition {
                            id: showEgg
                            toX: -230; // The egg slides in from outside the screen (see comment on spaceQuota above)
                            duration: 600
                            onStarted: {
                                // Scale the eggs down to create a zoom effect. The animations
                                // are done using the built-in implicit animations.
                                superEggs.eggScale = 0.7;
                            }
                        },
                        TranslateTransition {
                            id: hideEgg
                            toX: 0; // The egg slides out in the opposite direction of the show animation.
                            duration: 600
                            onEnded: {
                                // At the end of the animation the super eggs are scaled back to their original size.
                                superEggs.eggScale = 1.0;
                            }
                        } // TranslateTransition
                    ] // animations
                } // AnimationEgg
            }
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/animation/dark_background.png"
                }
            ]
                        
        } // Container

        // The control Container contains a toggle button that switches between the two modes.
        Container {
            background: controllerPaint.imagePaint
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            leftPadding: 20
            rightPadding: leftPadding
            bottomPadding: leftPadding
            topPadding: leftPadding

            layout: DockLayout {
            }

            Label {
                text: "More eggs"
                textStyle.base: SystemDefaults.TextStyles.TitleText
            }

            ToggleButton {
                horizontalAlignment: HorizontalAlignment.Right
                onCheckedChanged: {
                    // Trigger the animations
                    if (checked == true) {
                        // Show the eggs and call the tilt function in the AnimationEgg.qml file to wiggle the egg.
                        showEgg.play()
                        moreEgg.tilt();
                    } else {
                        // Hide the extra egg when this animation ends. The super eggs are scaled back to their original size.
                        hideEgg.play();
                    }
                }

            }

            attachedObjects: [
                ImagePaintDefinition {
                    id: controllerPaint
                    imageSource: "asset:///images/background.png"
                    repeatPattern: RepeatPattern.XY
                }
            ]

        } // Container
    }// RecipeContainer (see common folder)
}// RecipePage (see common folder)
