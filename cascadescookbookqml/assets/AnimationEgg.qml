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

// As the egg is close to finished, two rotation
// animations are triggered making the egg tilt a little bit.
Container {
    layout: AbsoluteLayout{}

    // The shadow is put below the egg.
    ImageView {
        layoutProperties: AbsoluteLayoutProperties { positionY: 150 }
        imageSource: "asset:///images/animation/egg_isolated_shadow.png"

        // The shadow rotates around half its height,
        // the pivot point is in the middle of the image to start.
        pivotY: -297/2;

        animations: [
            // A sequential animation is used to trigger consecutive rotations of
            // the image to make it wiggle.
            SequentialAnimation {
                id: tiltInShadow
                animations: [
                    RotateTransition { toAngleZ: 15; duration: 700 * 0.4; delay: 350},
                    RotateTransition { toAngleZ: -5; duration: 700 * 0.3},
                    RotateTransition { toAngleZ: 0;  duration: 700 * 0.3}
                ]
            }// SequentialAnimation
        ]// animations
    }// ImageView

    // The egg image
    ImageView {
        layoutProperties: AbsoluteLayoutProperties { positionX: 40 } 
        imageSource: "asset:///images/animation/egg_isolated.png"

        // The egg rotates around half the full height (the bottom of the egg),
        // the pivot point is in the middle of the image to start.
        pivotY: 203/2;

        // As the egg comes to a stop, it wiggles a little bit.
        animations: [
            SequentialAnimation {
                id: tiltIn
                animations: [
                    RotateTransition { toAngleZ: -15; duration: 700 * 0.4; delay: 350},
                    RotateTransition { toAngleZ: 5;   duration: 700 * 0.3},
                    RotateTransition { toAngleZ: 0;   duration: 700 * 0.3}
                ]
            }
        ]
    }

    // This function triggers all the show animations for the egg.
    function tilt()
    {
        tiltIn.stop();
        tiltInShadow.stop();
        tiltIn.play();
        tiltInShadow.play();
    }
}
