/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import bb.cascades 1.3
import "Common"

// This recipe shows how to create a variety of different buttons, both with and without images.
RecipePage {
    property int state: 0
    property bool buttonsEnabled: true

    RecipeScrollView {
        RecipeContainer {
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: ui.du(3)

            layout: StackLayout {
            }

            // A Button
            Button {
                id: startButton
                horizontalAlignment: HorizontalAlignment.Center
                text: buttonsEnabled?"Disable":"Enable"

                onClicked: {
                    // Disable all the other buttons.
                    buttonsEnabled = ! buttonsEnabled;
                }
            }

            // A Button with icon
            Button {
                id: fruitButton
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(3)
                text: "Ripen"
                imageSource: "asset:///images/button/icon_orange_green.png"
                enabled: buttonsEnabled

                onClicked: {
                    // The button icon is alternated via the state parameter (see onStateChanged below).
                    state = (state + 1) % 3;
                }
            }

            // A standard Button with a green color.
            Button {
                id: eatButton
                horizontalAlignment: HorizontalAlignment.Center
                text: "Eat"
                topMargin: ui.du(3)
                color: Color.create("#00B800")
                enabled: buttonsEnabled

                onClicked: {
                    // Hide the button when its eaten.
                    eatButton.visible = false;
                }
            }

            // Standard button using the Primary color appearance.
            Button {
                id: newButton
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(3)
                text: "New"
                appearance: ControlAppearance.Primary
                enabled: buttonsEnabled

                onClicked: {
                    // Show the eat button (no effect if its already shown)
                    eatButton.visible = true;
                }
            }

            // An ImageButton
            ImageButton {
                id: orangeButton
                horizontalAlignment: HorizontalAlignment.Center
                defaultImageSource: "asset:///images/button/image_button_enabled.png"
                pressedImageSource: "asset:///images/button/image_button_disabled.png"
                disabledImageSource: "asset:///images/button/image_button_disabled.png"
                topMargin: ui.du(3)
                enabled: buttonsEnabled

                onClicked: {
                    // Show the eat button (no effect if its already shown)
                    eatButton.visible = true;
                }
            }
        }
    }

    onStateChanged: {
        // The state controls which icon should be shown on the fruit Button.
        switch (state) {
            case 0:
                {
                    fruitButton.imageSource = "asset:///images/button/icon_orange_green.png"
                    break;
                }
            case 1:
                {
                    fruitButton.imageSource = "asset:///images/button/icon_orange_ripe.png"
                    break;
                }
            case 2:
                {
                    fruitButton.imageSource = "asset:///images/button/icon_orange_mouldy.png"
                    break;
                }
            case 3:
                {
                    fruitButton.imageSource = "asset:///images/button/icon_orange_dust.png"
                    break;
                }
        }
    }
}
