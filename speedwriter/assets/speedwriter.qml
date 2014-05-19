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
import com.speedwriter 1.0
import "SpeedTextBox"

Page {
    // Need this to prevent the layout to change when the virtual keyboard is shown.
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
                topPadding: ui.du(3)
                minHeight: ui.du(44)
                horizontalAlignment: HorizontalAlignment.Fill
                
                // This Container is scaled on orientation switch, we set the scaling point 
                // close to the top instead of the middle to maintain its padded position. 
                pivotY: -180

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                // This is a CustomControl see speedgauge.cpp/h
                SpeedGauge {
                    id: speedGauge
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }

			// The text box, the input field and its update logics is kept in one Component in the SpeedTextBox folder.
			SpeedTextBox {
			    id: speedTextBox
                currentLine: wordChecker.line
                remainingText: wordChecker.speedText
                isSpeedTextValid: wordChecker.valid
                averageSpeed: speedGauge.averageSpeed
                topPadding: ui.du(3)
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2
                }
                
                onInputTextChanging: {
                    // Check if the entered text is correct or not by using the wordChecker object.
                    wordChecker.checkWord(inputText);

                    // Update the speedText in the scrolling area of the Component.
                    if (wordChecker.remainingText.length > 0) {
                        speedText = "<html><span style='color:#1f1f1f;'>" + wordChecker.enteredLines + wordChecker.currentCorrectLine + "</span>" + "<span style='color:#e0e0e0;'>" + wordChecker.remainingText + "</span></html>"
                    }
                }
			}
        }
    }

    attachedObjects: [
        // Non-visual objects are added to QML as attached objects.
        // The WordChecker is the object that contains the logics for checking
        // the correctness of the entered text.
        WordChecker {
            id: wordChecker
            speedText: "Mary had a little lamb, its fleece \nwas white as snow. Sea shells, \nsea shells, by the sea shore. The \nflirtatious flamingo relentlessly \nargued with the aerodynamic \nalbatross. Admire the \nmiscellaneous velociraptors \nbasking in the sun. Egotistic \naardvarks enthusiastically \neating lollipops. Precisely, \npronounced the presidential \nparrot presiding over the \npurple pachyderms. \n"
            onNbrOfCharactersChanged: {
                // A new correct character(s) has been entered so the speed is updated.
                speedGauge.calculateSpeed(nbrOfCharacters);
            }
            
            onEnded: {
                speedTextBox.ended = true;
                speedGauge.stop();
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
                    stackContainer.layout.orientation = LayoutOrientation.RightToLeft

                    // Scale down the gauge in landscape mode.
                    gaugeContainer.scaleY = 0.725
                    gaugeContainer.scaleX = gaugeContainer.scaleY
                } else {
                    // Change to portrait background image and arrange the Controls from top to bottom.
                    backgroundImage.imageSource = "asset:///images/background.png"
                    stackContainer.layout.orientation = LayoutOrientation.TopToBottom

                    // Reset the scale of the speed gauge for portrait mode
                    gaugeContainer.scaleY = 1
                    gaugeContainer.scaleX = gaugeContainer.scaleY
                }
            }
        }
    ]
}
