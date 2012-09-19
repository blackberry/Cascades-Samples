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

RecipePage {
    RecipeContainer {        
        layout: StackLayout {                    
        }
        
        // The top Container contains the eggs that will be animated.
        Container {
            id: eggContainer
            property int eggOffset: 300
            property int eggDistance: 768
            
            horizontalAlignment: HorizontalAlignment.Fill
            background: backgroundPaint.imagePaint
            preferredHeight: 650
            
            layout: AbsoluteLayout {
            }

            // The two "super" eggs are two big eggs stacked side by side.
            // This entire Container is scaled by an implicit animation when the toggle is switched.
            Container {
                id: superEggs
                leftPadding: 30
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionY: eggContainer.eggOffset
                }

                // When scaling the entire Container down, it should be done on a point corresponding
                // to left edge. This point is half of the preferred width of the parent Container.
                pivotX: - eggContainer.eggDistance / 2

                // Two eggs; the eggs are set up in the AnimationEgg.qml file.
                AnimationEgg {
                }
                
                AnimationEgg {
                }
            }

            // This egg will be animated into the scene when the toggle is switched.
            AnimationEgg {
                id: moreEgg
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionY: eggContainer.eggOffset
                }

                // The egg starts outside the screen in the x direction and is already scaled down.
                translationX: eggContainer.eggDistance
                scaleX: 0.7
                scaleY: 0.7;

                // Show and hide animations of the egg.
                animations: [
                    TranslateTransition {
                        id: showEgg
                        toX: 460; // The egg slides in from outside the screen.
                        duration: 600
                        onStarted: {
                            // Scale the eggs down to create a zoom effect. The animations
                            // are done using the built-in implicit animations.
                            superEggs.scaleX = 0.7;
                            superEggs.scaleY = 0.7;
                        }
                    },
                    TranslateTransition {
                        id: hideEgg
                        toX: eggContainer.eggDistance; // The egg slides out in the opposite direction of the show animation.
                        duration: 600
                        onEnded: {
                            // At the end of the animation the super eggs are scaled back to their original size.
                            superEggs.scaleX = 1.0;
                            superEggs.scaleY = 1.0;
                        }
                    } // TranslateTransition
                ] // animations
            } // AnimationEgg
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/dark_background.png"
                }
            ]
                        
        } // Container

        // The control Container contains a toggle button that switches between the two modes.
        Container {
            background: controllerPaint.imagePaint
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            leftPadding: 35
            rightPadding: leftPadding
            bottomPadding: leftPadding    
            topPadding: leftPadding
            
            layout: DockLayout {
            }
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            Label {
                text: "Scrambled eggs"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }

            // A recipe text
            Container {
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                Label {
                    verticalAlignment: VerticalAlignment.Bottom
                    multiline: true
                    text: "1. Take two eggs.\n2. Scramble them.\n3. Done."
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }

                // A Label and a ToggleButton are stacked together and aligned in the
                // bottom-right corner.
                Container {
                    verticalAlignment: VerticalAlignment.Bottom
                    
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
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
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Right
                        
                        text: "Super size"
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                    }// Label
                }// Container
            }
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: controllerPaint
                    imageSource: "asset:///images/background.png"
                    repeatPattern: RepeatPattern.XY
                }
            ]
            
        }// Container        
    }// RecipeContainer (see common folder)
}// RecipePage (see common folder)
