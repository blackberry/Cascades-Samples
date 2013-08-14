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
import bb.cascades 1.2

Page {
    // Content Container
    Container {
        layout: AbsoluteLayout {
        }

        // ImageView containing the background image
        ImageView {
            imageSource: "asset:///images/background.png"
        }

        // The beard image
        ImageView {
            id: beard

            // Beard dragging properties
            property real dy: 0
            property real currentY: 0
            property real dragThreshold: 40
            property bool playedAhh: false
            imageSource: "asset:///images/beard.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 249
                positionY: 591
            }

            // Define an animation that will be played when the beard is released.
            animations: [
                SequentialAnimation {
                    id: resetBeard
                    animations: [
                        TranslateTransition {
                            toY: 11
                            duration: 400 * 0.7
                        },
                        TranslateTransition {
                            toY: 13
                            duration: 400 * 0.3
                        },
                        TranslateTransition {
                            toY: 0
                            duration: 400 * 0.1
                        }
                    ]
                    onStarted: {
                        // Play the "thank you" sound when the animation starts.
                        pullMyBeardApp.playSound("thank_you.wav");
                    }
                }
            ]
            onTouchExit: {
                // Release the beard when exiting the image as well.
                releaseBeard();
            }
            onTouch: {
                // Disable implicit animations on the beard; this will make the small movements more snappy.
                translationControllerY.enabled = false;

                // Check for a press, if so, set the state variables we need
                if (event.isDown()) {
                    resetBeard.stop();
                    dy = event.windowY;
                    playedAhh = false;

                    // Check for a move.
                } else if (event.isMove()) {
                    currentY = event.windowY - dy;

                    // Check the drag limits.
                    if (currentY > dragThreshold) {
                        translationY = dragThreshold;

                        // If we haven't already played the "aaaah" sound, its time to do so!
                        if (! playedAhh) {
                            pullMyBeardApp.playSound("aaaa.wav");
                            playedAhh = true;
                        }
                    } else if (currentY < 0) {
                        // Minimum drag threshold of the beard is zero position.
                        translationY = 0;
                        playedAhh = false;
                    } else {
                        // Within bounds, update position.
                        translationY = currentY;
                    }
                } else if (event.isUp()) {
                    // Beard was released, we call the release function.
                    releaseBeard();
                }

                // Re-enabling the implicit animations.
                translationControllerY.enabled = true;
            }
            
            attachedObjects: [
                ImplicitAnimationController {
                    id: translationControllerY
                    propertyName: "translationY"
                }
            ]
        }

        // Top of head "mask" image
        ImageView {
            imageSource: "asset:///images/top_head_mask.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 282
                positionY: 494
            }
        }
    }// Content Container

    function releaseBeard () {

        // Check if the beard is not at its original position.
        if (beard.translationY != 0) {

            // If it is not, play the reset animation to close the man's mouth.
            resetBeard.play ();
        }
    }
}
