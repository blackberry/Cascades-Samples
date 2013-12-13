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

// The application introduction recipe that sets up a view with some text
// explaining the purpose of the application and a small example UI
// showing a couple of Cascades Core Controls.
RecipePage {
    resizeBehavior: PageResizeBehavior.None

    // This screen was designed for a specific resolution in some cases a ScrollView is necessary
    // and in some not (see Common/RecipeScrollView.qml and 720x720/Common/RecipeScrollView.qml)
    RecipeScrollView {
        Container {
            topPadding: 30
            bottomPadding: topPadding
            leftPadding: 50
            rightPadding: leftPadding

            // Title Label for the intro recipe
            Label {
                text: "Cascades Cookbook"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }

            // A small example UI is set up in a separate QML document (IntroExampleUI.qml).
            IntroExampleUI {
                topMargin: 45;
                bottomMargin: 40;
            }

            // The introduction text
            Label {
                multiline: true
                text: "In this cookbook, I've put a list of ingredients for making user interfaces. Just like sushi, the end result will depend on your skill of aligning and balancing the components until they look just right.\n\nGood luck!\n/The Chef"
            }// Label
        }// Container
    }// RecipeContainer
}// RecipePage
