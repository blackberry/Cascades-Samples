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
import bb.cascades 1.3

// A cozy candle that triggers a fire alarm, used to illustrate how to show a CustomDialog
Container {
    signal triggerFireAlarm();
    
    layout: AbsoluteLayout {
    }

    ImageView {
        id: flameImage
        property real flametime: 400
        imageSource: "asset:///images/customdialog/flame.png"

        // The flame animation gradually scales the flame up in Y direction and
        // finally triggers the CustomDialog.
        animations: [
            SequentialAnimation {
                id: risingFlame
                ScaleTransition {
                    toY: 1.2
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.1
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.4
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.3
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.6
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.5
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.9
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 1.7
                    duration: flameImage.flametime
                }
                ScaleTransition {
                    toY: 2.0
                    duration: flameImage.flametime
                }

                onEnded: {
                    triggerFireAlarm()
                }
            }
        ]
        onCreationCompleted: risingFlame.play()

        attachedObjects: [
            LayoutUpdateHandler {
                onLayoutFrameChanged: {
                    // A layout update handler is used to find the size of the image so the
                    // pivot point can be set to the  middle-bottom of the image, which makes
                    // the image scale up from the bottom in the risingFlame animation above.
                    flameImage.pivotX = layoutFrame.width / 2
                    flameImage.pivotY = layoutFrame.height / 2
                }
            }
        ]
    }

    ImageView {
        imageSource: "asset:///images/customdialog/background.png"
    }
    
    function startFire() {
        risingFlame.play();
    }

    function stopFire() {
        risingFlame.stop();
    }

}
