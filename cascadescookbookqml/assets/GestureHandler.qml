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

// Sometimes in a UI, special gestures are used to trigger particular actions.
// In this recipe it is shown how to handle: tap, double-tap, pinch, and long-press
// by the use of gestureHandlers.
RecipePage {
    RecipeContainer {
        id: gestureRecipe
        property string gestureState: "tap"
        
        // Title Label used for giving instructions
        Label {
            id: instruction
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            text: "Tap to start."
            textStyle.base: SystemDefaults.TextStyles.TitleText
        }
        
        // Create the gesture Container containing an image that will be altered as 
        // different gestures are performed.
        Container {
            id: gestureContainer
            opacity: 0
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
                        
            ImageView {
                id: gestureImage
                imageSource: "asset:///images/gesturehandler/whole.png"
            }
                        
            animations: [
                ScaleTransition {
                    id: longpressOut
                    toX: 0.0
                    toY: 0.0
                    duration: 400
                    easingCurve: StockCurve.Linear
                    onStarted: {
                        instruction.opacity = 0;
                    }
                    onEnded: {
                        // Reset gesture object to start over.
                        instruction.opacity = 1;
                        instruction.text = "Well done, tap to start over."
                        gestureContainer.scaleX = 1;
                        gestureContainer.scaleY = 1;
                        gestureContainer.rotationZ = 0;
                        gestureContainer.opacity = 0;
                        gestureImage.imageSource = "asset:///images/gesturehandler/whole.png";
                        gestureRecipe.gestureState = "tap"
                    }
                }
            ]
        }
        
        // All gestures in this recipe are handled by these gesture handlers. The gesture
        // handlers belong to the UI element they are in, in this case the RecipeContainer.
        // So events occurring inside that element can be captured by them.
        gestureHandlers: [
            TapHandler {
                onTapped: {
                    // The gesture state is a local property used to try one 
                    // gesture at the time, the first one is tap.
                    if(gestureRecipe.gestureState == "tap") {
                        // Tap was made and double-tap is the next gesture to try out
                        gestureContainer.opacity = 1;
                        instruction.text = "Excellent! Try to double-tap."
                        gestureRecipe.gestureState = "doubletap"
                    }
                }
            },
            DoubleTapHandler {
                onDoubleTapped: {
                    if(gestureRecipe.gestureState == "doubletap") {
                        // Double-tap was performed, let's move on to pinching
                        instruction.text = "Now pinch to enlarge."
                        gestureImage.imageSource = "asset:///images/gesturehandler/broken.png"
                        gestureRecipe.gestureState = "pinch"
                    }
                }
            },
            PinchHandler {
                property bool pinchInProgress: false
                property real previousPinchRatio: 0
                
                onPinchStarted: {
                    if(gestureRecipe.gestureState == "pinch") {        
                        pinchInProgress = true;
                    }
                }
                onPinchUpdated: {
                    if (pinchInProgress) {
                        
                        // The total scale factor is the sum of the current pinch ratio
                        // and the accumulated pinching performed previously.
                        var totalScaleFactor = event.pinchRatio + previousPinchRatio;
                        gestureContainer.scaleX = totalScaleFactor;
                        gestureContainer.scaleY = totalScaleFactor;
                        gestureContainer.rotationZ = event.rotation;
                        
                        if (totalScaleFactor > 2.0) {
                            // When the pinching is larger then 2.0, it's time to try out the 
                            // long-press gesture.
                            pinchInProgress = false;
                            instruction.text = "Great! Now long-press.";
                        } else if(totalScaleFactor < 1.0) {
                            instruction.text = "I said ENLARGE, change direction.";
                        } else {
                            instruction.text = "Continue pinching!";
                        }
                    }
                }
                onPinchEnded: {
                    if(pinchInProgress) {
                        // Save the current scaling of the image, so that the 
                        // zoom factor can be adjusted for this during the next pinch session.
                        previousPinchRatio = gestureContainer.scaleX - 1.0;
                    } else {
                        previousPinchRatio = 0;
                        gestureRecipe.gestureState = "longpress";
                    }
                }
            },
            LongPressHandler {
                onLongPressed: {
                    if(gestureRecipe.gestureState == "longpress") {
                        // Run the animation for removing the gesture object.
                        longpressOut.play();
                        instruction.opacity = 0;
                    }
                }
            }// LongPressHandler
        ]// gestureHandler
    }// RecipeContainer
}// RecipePage
