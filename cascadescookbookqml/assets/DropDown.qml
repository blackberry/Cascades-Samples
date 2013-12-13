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

// A recipe showing how the DropDown Control is used to make a selection.
// In this case, different selections result in a recipe with different amounts of delicious beer.
RecipePage {
    RecipeContainer {
        ScrollView {
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }
            Container {
                topPadding: 20
                rightPadding: topPadding
                leftPadding: rightPadding
                
                Label {
                    text: "Beer recipe"
                }

                // In the DropDown Control, we add a number of options for selecting different values.
                // We use them later to check how many pints the recipe should make
                DropDown {
                    id: dropdown

                    title: "Number of pints"
                    Option {
                        value: 1
                        text: value
                    }
                    Option {
                        value: 2
                        text: value
                        selected: true
                    }
                    Option {
                        value: 3
                        text: value
                    }
                    Option {
                        value: 4
                        text: value
                    }
                                        
                    onSelectedValueChanged: {
                        // Illustrating usage of DropDown signal handler
                        console.debug("Number of pints changed to " + selectedValue)
                    }
                } // DropDown

                // The recipe text is built up from three texts: A text with the amount that
                // contain the different measures added to create the mix, A text with the ingredients
                // that is aligned to the amounts, and a text of the formula describing the process.
                Container {
                    topMargin: 20
                    TextArea {
                        editable: false
                        input.flags: TextInputFlag.SpellCheckOff
                        text: dropdown.selectedValue / 10 + " kg\t\tPale Ale Malt\n" + dropdown.selectedValue * 1.0 + " g     \t\tCascade Hops\n" + dropdown.selectedValue * 3 / 5 + " g\t\tYeast\n" + dropdown.selectedValue * 1.5 + " l     \t\tWater"
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                    }
                    ImageView {
                        imageSource: "asset:///images/dropdown/beer" + dropdown.selectedValue + ".png"
                        scalingMethod: ScalingMethod.AspectFit
                    }
                    
                    TextArea {
                        editable: false
                        input.flags: TextInputFlag.SpellCheckOff
                        horizontalAlignment: HorizontalAlignment.Center
                        text: "1. Mash at 67°C for 60 min.\n2. Sparge\n3. Boil the wort for 90 min.\n4. Add hops after 30 min.\n5. Add yeast, ferment 1-2 weeks.\n6. Add sugar and ferment in bottles for 1 week.\n7. Serve."
                    }// TextArea
                }// Container
            }// Container
        }// ScrollView
    }// RecipeContainer
}// RecipePage
