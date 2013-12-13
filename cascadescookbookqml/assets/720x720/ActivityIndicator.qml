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

// This is a simple ActivityIndicator recipe.
RecipePage {
    RecipeContainer {
        Container {
            topPadding: 50
            horizontalAlignment: HorizontalAlignment.Center
         
            Label {
                bottomMargin: 50
                horizontalAlignment: HorizontalAlignment.Center
                text: "This is an egg boiling simulator"
                textStyle.base: SystemDefaults.TextStyles.TitleText
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Center
                
                layout: DockLayout { 
                }
                
                ImageView {
                    id: unbroken
                    imageSource: "asset:///images/stockcurve/egg.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center                    
                }
                ImageView {
                    id: broken
                    imageSource: "asset:///images/stockcurve/broken_egg.png"
                    opacity : 0.0
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                }
            }
            
            Button {
                topMargin:50
                horizontalAlignment: HorizontalAlignment.Center
                text: "Start cooking"
                
                onClicked: {
                    if( text == "Start cooking") {
                        text = "Look away"
                        recipeIndicator.indicatortext = "boiling, boiling, boiling!";
                        recipeIndicator.startIndicator();
                    
                    } else if(text == "Look away") {
                        text = "Clean up"
                        unbroken.opacity = 0.0 
                        broken.opacity = 1.0
                        recipeIndicator.stopIndicator();
                    } else {
                        text = "Start cooking"
                        unbroken.opacity = 1.0 
                        broken.opacity = 0.0
                    }
                }// onClicked
            }// Button
        }// Container
                                
        InlineActivityIndicator {
            id:recipeIndicator
            indicatortext: "Default text"
            
            // Setting new padding value and height for 720x720 devices.
            aiPreferredHeight: 130
            padding: 10 
            
        } // InlineActivityIndicator
    }// RecipeContainer
}// RecipePage

