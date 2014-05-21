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
import "Common"

// Simple ProgressIndicator recipe that shows how a ProgressIndicator control
// can be added and the different states the ProgressIndicator can have.
RecipePage {
    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            // A component with a cooking pot that emits the state of the cooking progress.
            ProgressIndicatorCookingPot {
                id: cookingPot
                
                onCookingDone: {
                    // If the cooking time has reached its limit set state to complete
                    pausebutton.checked = false;
                    progressbar.state = ProgressIndicatorState.Complete
                }
                
                onCookingProgressChanged: {
                    progressbar.value = cookingProgress;
                }
            }            

            Container {
                rightPadding: ui.du(2)
                leftPadding: ui.du(2)
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                // A ProgressIndicator 
                ProgressIndicator {
                    id: progressbar
                    fromValue: 0
                    toValue: 10
                    state: ProgressIndicatorState.Indeterminate
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
                            // As soon as we start cooking, turn on progress.
                            progressbar.state = ProgressIndicatorState.Progress
                            cookingPot.startCooking();
                        } else {
                            // Pause the cooking is ongoing.
                            progressbar.state = ProgressIndicatorState.Pause
                            cookingPot.stopCooking();
                        }
                    }
                }
            }
        }
    }
}
