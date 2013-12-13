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

// This is the Sheet recipe. A sheet is primarily used to present UI concerned with
// creating or selection of content. They are also typically used
// when changing context to a temporary sub-flow that contains more complex entry
// or has more than one step (more than one screen).
// In this recipe, this use is illustrated by a bowl of fruit. The content of the bowl
// can be modified and a drill-down flow consisting of two Sheets is shown.
RecipePage {
    id: sheetRecipe
    property string fruitImagePath: "images/sheet/fruit5.png"
    property string basketText: "My basket of fruit"

    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // A greetings text that can be changed in the SheetModify.
            Label {
                text: basketText
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.base: SystemDefaults.TextStyles.BigText
            }

            // The bowl of fruit contents can be altered via SheetModify->SheetFruit.
            Container {
                topMargin: 20
                layout: DockLayout {
                }

                Container {
                    bottomPadding: 160
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    ImageView {
                        imageSource: fruitImagePath
                    }
                }

                ImageView {
                    imageSource: "asset:///images/sheet/basket.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom
                }
            }
        }
    }

    // The two Sheets are set up as attached objects so they are readily available when
    // entering the modify flow.
    attachedObjects: [
        Sheet {
            id: modifySheet
            SheetModify {
                id: modify

                onCancel: {
                    // Cancel modification so just hide the Sheet.
                    modifySheet.close();
                }

                onSave: {
                    // Save modification sp hide the Sheet, update the fruit, and the 
					// greetings text as long as the text has length greater than 0.
                    sheetRecipe.fruitImagePath = newFruit;
                    if(newBasketText.length > 0)
                        sheetRecipe.basketText = newBasketText;

                    // Hide the sheet.
                    modifySheet.close();
                }
                onSelectFruit: {
                    // The user has pressed the fruit and wants to select a new one.
                    // Show a second Sheet on top of this one.
                    fruitSheet.open();
                }
            }
        },
        Sheet {
            id: fruitSheet
            SheetFruit {
                onCancel: {
                    // Cancel fruit selection so hide this Sheet.
                    fruitSheet.close();
                }
                onSave: {
                    // A new fruit has been selected so update the currently selected
                    // fruit on the modification sheet (the user might still cancel
                    // modification). We do not update the fruitImagePath until
                    // the save signal from that Sheet has been received.
                    modify.fruitImagePath = newFruit;
                    fruitSheet.close();
                }
            }
        }
    ]
    actions: [
        ActionItem {            
            title: "Modify"
            ActionBar.placement: ActionBarPlacement.OnBar            
            onTriggered: {
                // Show the first sheet by setting its visibility to true.
                modify.fruitImagePath = fruitImagePath;
                modifySheet.open();
            }
        }// ActionItem
    ]
}// RecipePage
































