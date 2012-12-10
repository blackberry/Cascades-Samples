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
    // Need this to prevent the layout to change when the virtual keybaord is shown.
    resizeBehavior: PageResizeBehavior.None 
    
    Container {
        layout: DockLayout {
        }

        // background image
        ImageView {
            id: backgroundImage
            imageSource: "asset:///images/background.png"
        }

        // Stacklayout holding gauge, text and input controls.
        // When in landscape mode, the layout orientation will change
        Container {
            id: stackContainer
            
            layout: StackLayout {
                //Changes to "RightToLeft when in landscape
                orientation: LayoutOrientation.TopToBottom 
            }

            // GaugeContainer is placeholder for the custom control speedGauge
            Container {
                id: gaugeContainer
                topPadding: 40
                bottomPadding: 40
                horizontalAlignment: HorizontalAlignment.Fill
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: -1
                }
                
                SpeedGauge {
                    id: speedGauge
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            // The Container with the speed text and the text input control.
            Container {
                id: textContainer
                
                Container {
                    id: speedTextContainer
                    maxHeight: 165
                    preferredWidth: 768
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    layout: DockLayout {
                    }

                    // Text background image.
                    ImageView {
                        id: bgImage
                        imageSource: "asset:///images/border_image_text_field_source.png.amd"
                        preferredWidth: 728
                        preferredHeight: speedTextContainer.maxHeight
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    
                    // The speed text is put inside a ScrollView so that the entire text is 
                    // layouted even as the translationY would move it outside. If it where to 
                    // be put inside a Container instead the text would be cut. 
                    ScrollView {
                        horizontalAlignment: HorizontalAlignment.Center
                        // Since the nine-sliced image has transparent boarders we have to adjust the positon and size of the scrollview.
                        preferredHeight: speedTextContainer.maxHeight - 12
                        translationY: 6
                        
                        // Need to add a scrollmode to prevent touch interaction with the scrollview control. Will just show the text.
                        scrollViewProperties {
                            scrollMode: ScrollMode.None
                        }
                        
                        Label {
                            id: speedTextLabel
                            multiline: true
                            preferredWidth: 688
                            text: "<html><span style='color:#1f1f1f;'>" + wordChecker.enteredLines + wordChecker.currentCorrectLine + "</span>" + "<span style='color:#e0e0e0;'>" + wordChecker.remainingText + "</span></html>"
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                            // The position of the Label is changed as new lines are entered
                            // resulting in a line feed animation of this Label.
                            translationY: - wordChecker.line * 56
                        }
                    }
                } // speedTextContainer
                
                Container {
                    id: textInputContainer
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 30
                    
                    layout: DockLayout {
                    }

                    // A multi-line text area used for text input
                    TextArea {
                        id: textInput
                        preferredWidth: bgImage.preferredWidth - 10
                        preferredHeight: 99
                        hintText: "Type here to see how fast you are."
                        textStyle.base: SystemDefaults.TextStyles.TitleText
                        input.flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.AutoCorrectionOff | TextInputFlag.PredictionOff | TextInputFlag.SpellCheckOff | TextInputFlag.WordSubstitutionOff
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        onTextChanging: {
                            // Check if the entered text is correct or not by using the wordChecker object.
                            wordChecker.checkWord(text);
                        }
                    } // textInput

                    // Text background image.
                    ImageView {
                        imageSource: "asset:///images/border_image_text_field_input.png.amd"
                        preferredWidth: bgImage.preferredWidth
                        translationY: -4
                        preferredHeight: textInput.preferredHeight + 10
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        // Need overlapTouchPolicy te be able to place the image above the text and still have touchinteraction for the textarea.
                        overlapTouchPolicy: OverlapTouchPolicy.Allow
                        
                    } // inputbgImage
                } // textInputContainer

            } // textContainer
        } // stackContainer
    } // rootContainer
    
    attachedObjects: [

        // Non-visual objects are added to QML as attached objects.
        // The WordChecker is the object that contains the logics for checking
        // the correctness of the entered text.
        WordChecker {
            id: wordChecker
            speedText: "Mary had a little lamb, its fleece \nwas white as snow. Sea shells, \nsea shells, by the sea shore. The \nflirtatious flamingo relentlessly \nargued with the aerodynamic \nalbatross. Admire the \nmiscellaneous velociraptors \nbasking in the sun. Egotistic \naardvarks enthusiastically \neating lollipops. Precisely, \npronounced the presidential \nparrot presiding over the \npurple pachyderms. "
            
            onLineChanged: {
                // When one line has been entered correctly the textinput is cleared
                // to make room for entering the next line.
                textInput.text = "";
                textInput.hintText = "";
            }
            
            onEnded: {
                // The game is over, set up a text for displaying the final result in the text label and text area.
                speedTextLabel.text = "Your speed was " + speedGauge.averageSpeed + " words/min.\nWell done!";

                // Position the resulting text in the middle of the window and clear the other texts.
                speedTextLabel.translationY = 0;
                textInput.text = "";
                textInput.enabled = false;
            }
            
            onNbrOfCharactersChanged: {
                // A new correct character(s) has been entered so the speed is updated.
                speedGauge.calculateSpeed(nbrOfCharacters);
            }
        },

        // The orientation handler takes care of orientation change events. What we do here
        // is simply to change values for properties so that the app will look great in portrait
        // as well as in landscape orientation.
        OrientationHandler {
            id: handler
            onOrientationAboutToChange: {
                if (orientation == UIOrientation.Landscape) {
                    // Change the background image and the orientation of the StackLayout.
                    backgroundImage.imageSource = "asset:///images/landscape_background.png"
                    stackContainer.layout.orientation = "RightToLeft"
                    
                    textContainer.topPadding = 30
                    textInputContainer.topPadding = 20
                    gaugeContainer.topPadding = 30
                    gaugeContainer.bottomPadding = 30
                    gaugeContainer.layoutProperties.spaceQuota = "1"
                    
                    // The Speed gauge is scaled downed to fit in landscape mode.
                    speedGauge.scaleY = 0.725
                    speedGauge.scaleX = 0.725
                    speedGauge.translationY = -50
                } else {
                    // Change to portrait background image and arrange the Controls from top to bottom.
                    backgroundImage.imageSource = "asset:///images/background.png"
                    stackContainer.layout.orientation = "TopToBottom"
                    
                    textContainer.topPadding = 0
                    textInputContainer.topPadding = 30
                    gaugeContainer.topPadding = 40
                    gaugeContainer.bottomPadding = 40
                    gaugeContainer.layoutProperties.spaceQuota = "-1"
                    
                    // Reset the scale of the speed guage for portrait mode
                    speedGauge.scaleY = 1
                    speedGauge.scaleX = 1
                    speedGauge.translationY = 0
                }
            } // onOrientationAboutToChange
        } // OrientationHandler
    ] // attachedObjects


    // Enable support for portrait and lanscape orientation.
    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
    
}// Page
