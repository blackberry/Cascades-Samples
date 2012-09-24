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

// Simple ProgressIndicator recipe that shows how a ProgressIndicator control
// can be added and the different states the ProgressIndicator can have.
RecipePage {
    RecipeContainer {
        Container {
            leftPadding: 20
            rightPadding: leftPadding
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            TextArea {
                bottomMargin:100
                text: "Drag the slider to change the ProgressIndicator"
                editable: false
                textStyle.base: SystemDefaults.TextStyles.TitleText
            }

            ProgressIndicator {
                id: progressbar
                fromValue: 0
                toValue: 100
                value: slider1.value

                onValueChanged: {
                    pausebutton.text = "Pause"
                    if(value== 0){
                        progressbar.state = ProgressIndicatorState.Indeterminate
                    } else if(value== 100){
                        progressbar.state = ProgressIndicatorState.Complete
                    } else {
                        progressbar.state = ProgressIndicatorState.Progress
                    }
                }
            }

            Slider {
                id: slider1
                topMargin:100
                fromValue: 0
                toValue: 100
            }

            Button {
                id: pausebutton
                text: "Pause"
                onClicked: {
                    if (text == "Pause") {
                        text = "unPause"
                        progressbar.state = ProgressIndicatorState.Pause
                    } else {
                        text = "Pause"
                        progressbar.state = ProgressIndicatorState.Progress
                    }
                }
            }// Button
        }// Container
    }// RecipeContainer
}// RecipePage
