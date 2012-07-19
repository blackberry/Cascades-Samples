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

// A Sheet used for modifying the Sheet recipe Page. This is the
// first Sheet in the drill-down navigation from the Sheet recipe.

Page {
    id: sheetModify

    // Signals for canceling, saving and showing a second Sheet for fruit selection.
    signal cancel ()
    signal save (string newFruit, string newBasketText)
    signal selectFruit ();

    // Making an alias to the fruit image source so the sheet can be set up with current fruit selection.
    property alias fruitImagePath: fruitImage.imageSource
    
    titleBar: TitleBar {
        id: addBar
        title: "Modify"
        visibility: ChromeVisibility.Visible
        
        dismissAction: ActionItem {
            title: "Cancel"
            onTriggered: {
                // Cancel has been pressed so we emit cancel (will hide the sheet).
                sheetModify.cancel();
            }
        }
        
        acceptAction: ActionItem {
            title: "Save"
            onTriggered: {
                // Emit save signal with the new imageSource and greetings text.
                sheetModify.save(fruitImage.imageSource, editBasketText.text);
            }
        }
    }
    
    RecipeContainer {
        Container {
            layout: StackLayout {
                topPadding: 25
                leftPadding: 25
                rightPadding: 25
            }           

            // A text field for changing the greetings text.
            TextField {
                id: editBasketText
                bottomMargin: 150
                hintText: "Enter greetings text"
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                }
            }

            // The Image that will trigger drill down to another Sheet if tapped. To get
            // the user to understand that the image can be tapped some decoration is needed,
            // a background image with a nice drop shadow, an overlayed text and the image of course.
            Container {
                layout: DockLayout {
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
                ImageView {
                    imageSource: "asset:///images/title_gui_buffet_empty_box"
                    preferredWidth: 170
                    layoutProperties: DockLayoutProperties {
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                    }
                }

                Container {
                    layout: StackLayout {
                        leftPadding: 3
                        rightPadding: 3
                        bottomPadding: 6
                    }

                    ImageView {
                        id: fruitImage
                        imageSource: "images/sheet/fruit5.png"
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }

                    Container {
                        background: Color.create ("#aa272727")
                        
                        layout: StackLayout {
                            leftPadding: 20
                            rightPadding: leftPadding
                            bottomPadding: 10
                            topPadding: bottomPadding
                        }
                        
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                        }
                        
                        Label {
                            bottomMargin: 50
                            text: "Tap to change"
                            textStyle {
                                base: SystemDefaults.TextStyles.TitleText
                                color: Color.LightGray
                            }
                            layoutProperties: StackLayoutProperties {
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                    }
                }

                gestureHandlers: [
                    TapHandler {
                        onTap: {
                            sheetModify.selectFruit ();
                        }
                    }
                ]

            }
        }
    }
}
