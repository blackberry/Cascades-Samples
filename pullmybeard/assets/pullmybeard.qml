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

Page {
    content: Container {
        layout: AbsoluteLayout {
        }
    
        // Imageview containing the background image.
        ImageView {
            imageSource: "asset:///images/background.png"
            preferredWidth: 768
            preferredHeight: 1280
            
            // If we touch the image that means were outside of the pullable part of the UI.
            onTouch: {
                if (event.isUp ()) {
                    // An up event on the background also mean that the beard has been released.
                    releaseBeard();
                }
            }
        }
    
        // The beard image.
        ImageView {
            id: beard
        
            // Beard dragging properties
            property real _dy: 0
            property real _currentY: 0
            property real _dragThreshold: 40
            property bool playedAhh: false
            
            imageSource: "asset:///images/beard.png"
            preferredWidth: 285
            preferredHeight: 273
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 249
                positionY: 591
            }
        
            // Defining an animation that will be played when the beard is relased.
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
                        // Play the thank you sound when the animaiton starts.
                        pullMyBeardApp.playSound ("thank_you.wav");
                    }
                }
            ]
            onTouch: {
                // Disabling the implicit animations on the beard, this will make the small movements more snappy.
                translationControllerY.enabled = false;
            
                // Checking for a press, then set the statevariables we need.
                if (event.isDown()) {
                    resetBeard.stop ();
                    _dy = event.screenY;
                    playedAhh = false;
                } else if (event.isMove ()) {
                    _currentY = event.screenY - _dy;
                
                    // Check the drag limits.
                    if (_currentY > _dragThreshold) {
                        translationY = _dragThreshold;
                    
                        // If we havent already played the aaaah sound, it's now high time to do so!
                        if (!playedAhh) {
                            pullMyBeardApp.playSound ("aaaa.wav");
                            playedAhh = true;
                        }
                    }                    
                    else if (_currentY < 0) {
                        // Minimum drag threshold of the beard is zero position. 
                        translationY = 0;
                        playedAhh = false;
                    } else {
                        // Within bounds update position.
                        translationY = _currentY;
                    }
                } else if(event.isUp()) {
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
    
        // Masking image.
        ImageView {
            imageSource: "asset:///images/top_head_mask.png"
            preferredWidth: 297
            preferredHeight: 231
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 282
                positionY: 494
            }
        }
    }
    
    function releaseBeard() {
        
        // Check if the beard is not at its original postion.
        if (beard.translationY != 0) {
                    
            // If it is not play the reset animation to close the old mans mouth.
            resetBeard.stop ();
            resetBeard.play ();
        }
    }
    
}
