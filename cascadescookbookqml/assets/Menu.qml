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

/* WHAT

 NO CODE for menu's?
 
 Don't worry, there is code. but a system menu needs to be on the 
 uppermost top of the tree, so we put all the menu stuff in main.qml.
 
 Just go there and look for the magic words "Add Salt"
 
 Sincerely
 Le Chef
 
 */
RecipePage {
  
    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                multiline:true
                text: "Pull down from bezel to reveal application menu."
            }
        } // main Container
    } // RecipeContainer
}// RecipePage
