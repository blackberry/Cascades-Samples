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

// In this recipe we show how the RadioGroup and CheckBox controls are
// used to compose a delicious mix of olives.
RecipePage {
    // This screen was designed for a specific resolution in some cases a ScrollView is necessary 
    // and in some not (see Common/RecipeScrollView.qml and 720x720/Common/RecipeScrollView.qml)  
    RecipeScrollView {
        Container {
            leftPadding: 80
            rightPadding: leftPadding
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // Container with a header label and three checkboxes
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                layout: StackLayout {
                }

                Label {
                    text: "Olive type"
                    bottomMargin: 0
                    textStyle.base: textStyleBoldTitle.style
                }

                // The CheckBox is put in a separate QML component along
                // with a name and a colored box (illustrating the olive color).
                // See SelectionCheckBox.qml for more detail on this component.
                SelectionCheckBox {
                    oliveName: "Limoncello"
                    oliveColor: Color.create ("#808000")
                }
                SelectionCheckBox {
                    oliveName: "Greek"
                    oliveColor: Color.create ("#698B22")
                }
                SelectionCheckBox {
                    oliveName: "Kalamata"
                    oliveColor: Color.create ("#733D1A")
                }
            }

            Divider {
                bottomMargin: 60
                horizontalAlignment: HorizontalAlignment.Fill
            }

            Container {
                layout: StackLayout {

                }
                Label {
                    text: "Filling"
                    bottomMargin: 9
                    textStyle.base: textStyleBoldTitle.style
                }

                // The RadioGroup is a Control that groups several of RadioOptions together.
                // It ensures that one and only one Option is selected at once.
                // By listening for the onSelectedOptionChanged signal, it's possible
                // to keep track of what the current selection is.
                RadioGroup {
                    id: radioGroup
                    dividersVisible: false
                    Option {
                        text: "Stone"
                        value: text
                    }
                    Option {
                        text: "Pimento"
                        value: text
                    }

                    // When a new option is selected, we print the selection to the console.
                    onSelectedValueChanged: {
                        console.debug ("New filling set: " + selectedValue);
                    }
                }// RadioGroup
            }// Container
        }// top Container
    }// RecipeContainer

    attachedObjects: [
        TextStyleDefinition {
            id: textStyleBoldTitle
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
        }
    ]
}// RecipePage
