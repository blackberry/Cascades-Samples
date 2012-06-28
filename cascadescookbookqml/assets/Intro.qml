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

// The application introduction recipe. Sets up a view with some text
// explaining the purpose of the application and a small example UI
// showing a couple of Cascades 4 Core Controls.
RecipePage {
    RecipeContainer {
        Container {
            layout: StackLayout {
                topPadding: 50
                leftPadding: 30
                rightPadding: 30
            }

            // Head line in intro recipe.
            TextArea {
                text: "Cascades Cookbook"
                editable: false
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    fontWeight: FontWeight.Bold
                }
            }

            // A small example UI is set up in a separate QML document (IntroExampleUI.qml).
            IntroExampleUI {
                topMargin: 70
                bottomMargin: 50;
            }

            // The introduction text.
            TextArea {
                editable: false
                text: "In this cookbook, I've put a list of ingredients for making user interfaces. Just like sushi, the end result will depend on your skill of aligning and balancing the components until they look just right.\n\nGood luck!\n/The Chef"

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    lineSpacing: 1.1
                }
            }
        }
    }

}