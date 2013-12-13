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

// Simple ProgressIndicator recipe that shows how a ProgressIndicator control
// can be added and the different states the ProgressIndicator can have.
RecipePage {

    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            Container {
                bottomPadding: 50
                horizontalAlignment: HorizontalAlignment.Center
                ImageView {
                    id: potLid
                    property int cookingProgress: 0
                    property int cookingTime: 10

                    translationY: 30
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
                            // Update cooking progress
                            potLid.cookingProgress = potLid.cookingProgress + 1;
                            
                            if (potLid.cookingProgress == potLid.cookingTime) {
                                // Cooking is done turn off the stove and put the lid to rest
                                pausebutton.checked = false
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
            }

            Container {
                rightPadding: 20
                leftPadding: 20
                bottomPadding: 20
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                ProgressIndicator {
                    id: progressbar
                    fromValue: 0
                    toValue: 10
                    value: potLid.cookingProgress
                    verticalAlignment: VerticalAlignment.Center
                    
                    onValueChanged: {
                        if (value == 0) {
                            // Value 0 means that the progress is undecided
                            progressbar.state = ProgressIndicatorState.Indeterminate
                        }
                    }
                }

                ToggleButton {
                    id: pausebutton

                    onCheckedChanged: {
                        if (checked) {
                            if (potLid.cookingProgress == potLid.cookingTime) {
                                potLid.cookingProgress = 0;
                            }
                            cooking.play();
                            
                            // As soon as the cooking animation is turned on turn on progress
                            progressbar.state = ProgressIndicatorState.Progress
                        } else {
                            if (potLid.cookingProgress == potLid.cookingTime) {
                                // If the cooking time has reached its limit set state to complete
                                progressbar.state = ProgressIndicatorState.Complete
                            } else {
                                // Otherwise pause the progress
                                progressbar.state = ProgressIndicatorState.Pause
                                cooking.stop();
                            }
                        }
                    }
                } // ToggleButton
            }
        } // Container
    } // RecipeContainer
}// RecipePage
