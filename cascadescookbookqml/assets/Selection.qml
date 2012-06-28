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

// In this recipe we show how the RadioGroup and CheckBox Controls are
// used to compose a delicious mix of olives.
RecipePage {
    RecipeContainer {

        Container {
            layout: StackLayout {
                leftPadding: 80
                rightPadding: leftPadding
            }

            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            // Container with a header label and three checkboxes.
            Container {
                layout: StackLayout {

                }

                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }

                Label {
                    text: "Olive type"
                    bottomMargin: 0
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        fontWeight: FontWeight.Bold
                    }
                }

                // The CheckBox is put in a separate QML component together
                // with a name and colored box (illustrating the olive color)
                // see SelectionCheckBox.qml.
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
                //topMargin: 10
                //bottomMargin: 10
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
            Container {
                layout: StackLayout {

                }
                Label {
                    text: "Filling"
                    bottomMargin: 0
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        fontWeight: FontWeight.Bold
                    }
                }

                // The RadioGroup is a Control that groups several of RadioOptions together.
                // It ensures that one and only one Option is selected
                // and by listening for the onSelectedOptionChanged signal its possible
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
                    Option {
                        text: "Garlic"
                        value: text
                    }

                    // When a new option is selected we print the selection to the console.
                    onSelectedIndexChanged: {
                        console.debug ("New filling set: " + radioGroup.selectedValue());
                    }
                }
            }
        }
    }
}