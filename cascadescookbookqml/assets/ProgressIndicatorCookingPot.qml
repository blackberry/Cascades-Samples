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

// The Cooking pot used to signal updates in the ProgressIndicator recipe.
Container {
    signal cookingDone();
    signal cookingProgressChanged(int cookingProgress);

    bottomPadding: ui.du(3)
    horizontalAlignment: HorizontalAlignment.Center

    ImageView {
        id: potLid
        property int cookingProgressValue: 0
        property int cookingTime: 10

        translationY: ui.px(30)
        horizontalAlignment: HorizontalAlignment.Center
        imageSource: "asset:///images/progressindicator/lid.png"

        animations: SequentialAnimation {
            id: cooking
            animations: [
                RotateTransition {
                    toAngleZ: 2
                    duration: 100
                },
                RotateTransition {
                    toAngleZ: -2
                    duration: 100
                }
            ]
            onEnded: {
                // Update cooking progress and emit signal for watchers.
                potLid.cookingProgressValue = potLid.cookingProgressValue + 1;
                cookingProgressChanged(potLid.cookingProgressValue);
                
                if (potLid.cookingProgressValue == potLid.cookingTime) {
                    // Cooking is done turn off the stove and put the lid to rest
                    cookingDone();
                    potLid.rotationZ = 0;
                } else {
                    // Continue cooking
                    cooking.play();
                }
            }
        }
    }

    ImageView {
        horizontalAlignment: HorizontalAlignment.Center
        imageSource: "asset:///images/progressindicator/pot.png"
    }
    
    function startCooking() {
        if (potLid.cookingProgressValue == potLid.cookingTime) {
            potLid.cookingProgressValue = 0;
        }

        cooking.play();
    }
    
    function stopCooking() {
        cooking.stop();
    }
}
