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

// A Sheet used for modifying the Sheet recipe Page. This is the
// first Sheet in the drill-down navigation from the Sheet recipe.
Page {
    id: sheetModify

    // Signals for canceling, saving, and showing a second Sheet for fruit selection.
    signal cancel ()
    signal save (string newFruit, string newBasketText)
    signal selectFruit ();

    // Create an alias to the fruit imageSource so the sheet can be setup with current fruit selection.
    property alias fruitImagePath: fruitImage.imageSource
    
    titleBar: TitleBar {
        id: addBar
        title: "Modify"
        visibility: ChromeVisibility.Visible
        
        dismissAction: ActionItem {
            title: "Cancel"
            onTriggered: {
                // Cancel has been pressed so we emit the cancel signal (will hide the sheet).
                sheetModify.cancel();
            }
        }
        
        acceptAction: ActionItem {
            title: "Save"
            onTriggered: {
                // Emit save signal with the new imageSource and greetings text as parameters.
                sheetModify.save(fruitImage.imageSource, editBasketText.text);
            }
        }
    }
    
    RecipeContainer {
        Container {
            topPadding: ui.du(2)
            leftPadding: ui.du(2)
            rightPadding: ui.du(2)

            // A text field for changing the greetings text
            TextField {
                id: editBasketText
                hintText: "Enter greetings text"
            }

            // The Image that will trigger a drill down to another Sheet if tapped. To get
            // the user to understand that the image can be tapped, we add some decorations.
            // So we'll setup a background image with a nice drop shadow, an overlayed text, and the image, of course.
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(3)
                background: Color.create("#f8f8f8")
                layout: DockLayout {
                }

                ImageView {
                    imageSource: "asset:///images/empty_box.amd"
                    preferredWidth:  ui.du(17)
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }

                Container {
                    leftPadding: ui.du(0.5) 
                    rightPadding: leftPadding
                    bottomPadding: ui.du(0.5) 
                    background: Color.create("#f0f0f0")

                    ImageView {
                        id: fruitImage
                        imageSource: "images/sheet/fruit5.png"
                        horizontalAlignment: HorizontalAlignment.Center
                    }

                    Container {
                        background: Color.create ("#aa272727")
                        leftPadding: ui.du(2)
                        rightPadding: leftPadding
                        bottomPadding: ui.du(1) 
                        topPadding: bottomPadding                                                
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        
                        Label {
                            horizontalAlignment: HorizontalAlignment.Center
                            text: "Tap to change"
                            textStyle.base: textStyleLightTitle.style
                        }
                    }
                }

                gestureHandlers: [
                    TapHandler {
                        onTapped: {
                            sheetModify.selectFruit ();
                        }
                    }
                ]
            }
        }
    }
    
    attachedObjects: [
        TextStyleDefinition {
            id: textStyleLightTitle
            base: SystemDefaults.TextStyles.TitleText
            color: Color.LightGray
        }
    ]
}
