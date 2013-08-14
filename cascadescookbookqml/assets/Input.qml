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
import bb.cascades 1.2
import "Common"

// This recipe shows how to set up different type of text input. The simple
// one line TextField and an editable TextArea are shown. How to listen for textChanged
// signals is also illustrated in this recipe.
RecipePage {
    RecipeContainer {
        ScrollView {
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }

            Container {
                leftPadding: 80
                rightPadding: leftPadding
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                // A Label that the text fields will be updating as text is entered
                Label {
                    id: inputLabel
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Fill
                    multiline: true
                }

                // A one line TextField that has built in clear-field functionality
                TextField {
                    hintText: "Enter text into a single line TextField"
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Fill

                    // On text change the label text is updated.
                    onTextChanging: {
                        inputLabel.text = text;
                    }
                }

                // A disabled one-line TextField
                TextField {
                    enabled: false
                    hintText: "This is a disabled TextField"
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Fill
                }

                // A multi-line TextArea, the background can be disabled via the backgroundVisible property
                TextArea {
                    hintText: "Enter text into multi-line TextArea"
                    preferredHeight: 140
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: ""

                    // On the textChanging signal, the label text is updated.
                    onTextChanging: {
                        inputLabel.text = text;
                    }
                } // TextArea
            } // Container
        } // ScrollView
    } // RecipeContainer
}// RecipePage
