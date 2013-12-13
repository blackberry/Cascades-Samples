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

// Simple slider recipe that shows how a Slider control can be added
// and how to connect to the valueChanging signal of the Slider.
RecipePage {
    RecipeContainer {

        Container {
            leftPadding: 20
            rightPadding: leftPadding
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // A label that presents the current Slider values (with one digit precision)
            Label {
                id: labeltext
                text: "Value: 0"
                textStyle.base: SystemDefaults.TextStyles.TitleText
            }

            // A simple Slider with changing values from 0 to 1 (one-digit precision)
            Slider {
                fromValue: 0
                toValue: 1
                onImmediateValueChanged: {
                    // Update the Label with current value (with one-digit precision).
                    labeltext.text = "Value: " + immediateValue.toFixed (1);
                }
            }
        }// Container
    }// RecipeContainer
}// RecipePage
