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
        id: animationRecipe
        property int eggOffset: 380
        preferredWidth: 768

        // The top Container contains the eggs that will be animated.
        Container {
            preferredWidth: 768
            preferredHeight: 674
		    background: backgroundPaint.imagePaint
		    
		    attachedObjects: [
		        ImagePaintDefinition {
		            id: backgroundPaint
		            imageSource: "asset:///images/dark_background.png"
		        }
		    ]
		    
            layout: AbsoluteLayout {
            }

            // The two "super" eggs, two big eggs stacked side by side.
            // This entire Container i scaled by an implicit animation when the toggle is switched.
            Container {
                id: superEggs

                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                }
                layoutProperties: AbsoluteLayoutProperties {
                    positionY: animationRecipe.eggOffset
                }

                // When scaling the entire Container down it should be done on a point corresponding
                // to left edge. That is half the preferred width of the parent Container.
                pivotX: -animationRecipe.preferredWidth / 2;
                preferredHeight: 450

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
                        onStarted: {
                            // Scale the eggs down to create a zoom effect. The animations
                            // are done using the built in implicit animations.
                            superEggs.scaleX = 0.7;
                            superEggs.scaleY = 0.7;
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
                    }
                ]
            }
        }

        // The control Container, contains a toggle button that switches between the two modes.
        Container {
            preferredHeight: 360
            background: controllerPaint.imagePaint
    	    
    	    attachedObjects: [
    	        ImagePaintDefinition {
    	            id: controllerPaint
    	            imageSource: "asset:///images/background.png"
    	            repeatPattern: RepeatPattern.XY
    	        }
    	    ]
    	    
            layout: DockLayout {
                leftPadding: 35
            }

            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
            }

            // A recipe text.
            Container {
                layout: StackLayout {
                    topPadding: 11;
                }
                
                TextArea {                    
                    bottomMargin: 1
                    editable: false
                    text: "Scrambled eggs"
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        color: Color.Black
                    }
                }
                
                TextArea {
                    topMargin: 0
                    editable: false
                    text: "1. Take two eggs.\n2. Scramble them.\n3. Done."
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                        lineSpacing: 1.4
                    }                    
                }
            }

            // A Label and a toggle button is stacked together and aligned in the
            // bottom right corner
            Container {
                layout: StackLayout {
                    bottomPadding: 35
                    rightPadding: 35
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
                            // Show the eggs and call the tilt function in the Egg.qml file to wiggle the egg.
                            showEgg.play ()
                            moreEgg.tilt ();
                        } else {
                            // Hide the extra egg, when this animation ends the super eggs are scaled back to their original size.
                            hideEgg.play ();
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
}
