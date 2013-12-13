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

// This recipe shows how to create a variety of different buttons, both with and without images.
RecipePage {
    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            layout: StackLayout {
            }

            // A Button with icon
            Button {
                id: fruitButton
                property int state: 0
                topMargin: 40
                text: "Ripen"
                imageSource: "asset:///images/button/icon_orange_green.png"

                // Alternate the Button icon on click signals depending on the existing state.
                onClicked: {
                    switch (state) {
                        case 0:
                            {
                                state = 1;
                                fruitButton.imageSource = "asset:///images/button/icon_orange_ripe.png"
                                break;
                            }
                        case 1:
                            {
                                state = 2;
                                fruitButton.imageSource = "asset:///images/button/icon_orange_mouldy.png"
                                newButton.enabled = true;
                                break;
                            }
                        case 2:
                            {
                                fruitButton.imageSource = "asset:///images/button/icon_orange_dust.png"
                                fruitButton.enabled = false;
                                break;
                            }
                    } // switch
                } // onClicked
            } // Button

            // A standard Button
            Button {
                id: eatButton
                text: "Eat"
                topMargin: 40

                // Change the button text on click and hide the fruit button.
                onClicked: {
                    if (eatButton.text == "Eat") {
                        eatButton.text = "Burp";
                        eatButton.enabled = false;
                        fruitButton.opacity = 0;
                        newButton.enabled = true;
                    }
                }
            }

            // A disabled ImageButton
            ImageButton {
                id: newButton
                defaultImageSource: "asset:///images/button/image_button_enabled.png"
                pressedImageSource: "asset:///images/button/image_button_selected.png"
                disabledImageSource: "asset:///images/button/image_button_disabled.png"
                topMargin: 40
                enabled: false
                horizontalAlignment: HorizontalAlignment.Center
                
                onClicked: {
                    // Reset all the buttons to their original state.
                    fruitButton.imageSource = "asset:///images/button/icon_orange_green.png"
                    fruitButton.enabled = true;
                    fruitButton.opacity = 1.0;
                    fruitButton.state = 0;
                    eatButton.text = "Eat";
                    eatButton.enabled = true;
                    enabled = false;
                }
            }// ImageButton
        }// Container
    }// RecipeContainer
}// RecipePage
