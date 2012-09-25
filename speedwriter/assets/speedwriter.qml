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
import com.speedwriter 1.0

Page {
    Container {
        layout: AbsoluteLayout {
        }

        // This is the base-background image that covers the entire screen.
        // The background can be disabled via the backgroundOpacity property.
        ImageView {
            imageSource: "asset:///images/deep_background.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionY: 65
            }
        }

        // A multi-line label
        Label {
            id: speedTextLabel
            multiline: true
            preferredWidth: 730
            text: wordChecker.speedText;
            textStyle.base: speedTextStyle.style

            // The position of the Label is changed as new lines are entered
            // resulting in a line feed animation of bot this Label and the overlay
            // Label beneath.
            translationY: - wordChecker.line * 62
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 40
                positionY: 460
            }
            
        }

        // This is a multi-line label that shows the progress of entered text
        // in a different color then the text that should be entered (above).
        Label {
            id: overlayLabel
            multiline: true
            preferredWidth: speedTextLabel.preferredWidth
            text: wordChecker.enteredLines + wordChecker.currentCorrectLine;
            textStyle.base: overlayTextStyle.style

            // This Label is on top of the speedTextLabel and should have the same positioning.
            translationY: speedTextLabel.translationY
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: speedTextLabel.layoutProperties.positionX
                positionY: speedTextLabel.layoutProperties.positionY
            }
            
        }

        // The decorative "background" image which is actually put on top of the two Label above,
        // so that only the current line and one more line is visible through the transparen window
        // in the bbackground image.
        ImageView {
            imageSource: "asset:///images/background.png"
        }

        // A multi-line text area used for text input
        TextArea {
            id: textInput
            objectName: "textInput"
            preferredWidth: speedTextLabel.preferredWidth
            preferredHeight: 99
            hintText: "Type here to see how fast you are."
            textStyle.base: SystemDefaults.TextStyles.TitleText
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 20
                positionY: 635
            }
            
            onTextChanging: {
                if (text != "") {
                    // Check if the entered text is correct or not by using the wordChecker object.
                    wordChecker.checkWord(text);
                }
            }
        }

        // Custom control for the speed gauge
        SpeedGauge {
            objectName: "speedGauge"
            id: speedGauge
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 202
                positionY: 42
            }
        }
    }
    attachedObjects: [
        
        // Non-viusal objects are added to QML as attached objects.
        // The WordChecker is the object that contains the logics for checking
        // the correctness of the entered text.
        WordChecker {
            objectName: "wordChecker"
            id: wordChecker
            speedText: "Mary had a little lamb, its fleece \nwas white as snow. Sea shells, \nsea shells, by the sea shore. The \nflirtatious flamingo relentlessly \nargued with the aerodynamic \nalbatross. Admire the \nmiscellaneous velociraptors \nbasking in the sun. Egotistic \naardvarks enthusiastically \neating lollipops. Precisely, \npronounced the presidential \nparrot presiding over the \npurple pachyderms."
            
            onLineChanged: {
                // When one line has been entered correctly the textinput is cleared
                // to make room for entering the next line.
                textInput.text = "";
                textInput.hintText = "";
            }
            
            onEnded: {
                // The game is over, set up a text for displaying the final result in the overlay area.
                overlayLabel.text = "Your speed was " + speedGauge.averageSpeed + " words/min.\nWell done!";

                // Position the resulting text in the middle of the window and clear the other texts.
                overlayLabel.translationY = 0;
                speedTextLabel.text = "";
                textInput.text = "";
                textInput.enabled = false;
            }
            
            onNbrOfCharactersChanged: {
                // A new correct character(s) has been entered so the speed is updated.
                speedGauge.calculateSpeed(nbrOfCharacters);
            }
        },
        TextStyleDefinition {
            id: speedTextStyle
	        base: SystemDefaults.TextStyles.TitleText
	        color: Color.create("#e0e0e0")
        },
        TextStyleDefinition {
            id: overlayTextStyle
            base: SystemDefaults.TextStyles.TitleText
            color: Color.create("#1f1f1f")
        }
    ] // attachedObjects
}// Page
