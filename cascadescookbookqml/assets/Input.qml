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

// This recipe shows how to set up different type of text input. The simple
// one line TextField and an editable TextArea are shown. How to listen for textChanged
// signals is also illustrated in this recipe.
RecipePage {
    RecipeScrollView {
        RecipeContainer {
            topPadding: ui.du(3)
            leftPadding: ui.du(2)
            rightPadding: leftPadding
            
            layout: StackLayout {
            }

            // A Label that the text fields will be updating as text is entered
            Label {
                id: inputLabel
                horizontalAlignment: HorizontalAlignment.Fill
                multiline: true
            }

            // A one line TextField that has built in clear-field functionality
            TextField {
                hintText: "Enter text into a single line TextField"

                // On text change the label text is updated.
                onTextChanging: {
                    inputLabel.text = text;
                }
                input
                {
                    keyLayout: KeyLayout.Text
                }
            }

            // A disabled one-line TextField
            TextField {
                enabled: false
                hintText: "This is a disabled TextField"
            }

            // A multi-line TextArea, the background can be disabled via the backgroundVisible property
            TextArea {
                hintText: "Enter text into multi-line TextArea"
                preferredHeight: ui.du(14)
                
                // On the textChanging signal, the label text is updated.
                onTextChanging: {
                    inputLabel.text = text;
                }
            }
        }
    }
}
