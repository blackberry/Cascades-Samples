/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
import bb.multimedia 1.0

/**
*    added support for orientation Landscape / Portrait
*    Ekkehard Gentz (ekke) @ekkescorner http://ekkes-corner.org
*/

NavigationPane {
    id: navigationPane

    onPopTransitionEnded: page.destroy()

    Page {
        id: mainPage
        DictaphoneScrollView {

            Container {
                layout: DockLayout {
                }

                // The background image
                ImageView {
                    id: imageBackground
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    imageSource: "asset:///images/background.png"
                }

                //! [0]
                // The led object
                Led {
                    id: led
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Top
                    translationX: 40
                    translationY: 500
                    state: (recorder.mediaState == MediaState.Started ? "rec" : recorder.mediaState == MediaState.Paused ? "pause" : "off")
                }
                //! [0]

                //! [1]
                // The tape object
                Tape {
                    id: tape
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top

                    translationY: 570

                    running: (recorder.mediaState == MediaState.Started)
                }
                //! [1]

                // The 'dictaphone' label
                ImageView {
                    id: label
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top

                    translationY: -205

                    imageSource: "asset:///images/dictaphone_label.png"
                }

                // The background image of the button container
                ImageView {
                    id: buttonBackground
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom

                    imageSource: "asset:///images/buttons_background.png"
                }

                // The button container
                Container {
                    id: buttonContainer
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom

                    preferredWidth: 670
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    //! [2]
                    // The 'Record' button
                    ImageToggleButton {
                        rightMargin: 2
                        imageSourceDefault: "asset:///images/rec_button.png"
                        imageSourcePressedUnchecked: "asset:///images/rec_button.png"
                        imageSourceChecked: "asset:///images/rec_button_pressed.png"
                        imageSourcePressedChecked: "asset:///images/rec_button_pressed.png"
                        imageSourceDisabledChecked: "asset:///images/rec_button_pressed.png"
                        enabled: (recorder.mediaState != MediaState.Paused)
                        onCheckedChanged: {
                            if (recorder.mediaState == MediaState.Started) {
                                // Stop the recorder
                                recorder.reset()

                                // Update the internal track list
                                _trackManager.update()

                                // Play the finished sound
                                recordStopSound.play()
                            } else {
                                // Update the internal track list
                                _trackManager.update()

                                // Configure the recorder to use a new URL
                                recorder.outputUrl = "file://" + _trackManager.nextTrackUrl()

                                // Play the start sound
                                recordStartSound.play()

                                // Start the recorder
                                recorder.record()
                            }
                        }
                    }
                    //! [2]

                    //! [3]
                    // The 'Pause' button
                    ImageToggleButton {
                        leftMargin: 2
                        rightMargin: 2
                        imageSourceDefault: "asset:///images/pause_button.png"
                        imageSourceDisabledUnchecked: "asset:///images/pause_button.png"
                        imageSourcePressedUnchecked: "asset:///images/pause_button.png"
                        imageSourceChecked: "asset:///images/pause_button_pressed.png"
                        imageSourcePressedChecked: "asset:///images/pause_button_pressed.png"
                        enabled: (recorder.mediaState == MediaState.Started || recorder.mediaState == MediaState.Paused)
                        onCheckedChanged: {
                            if (recorder.mediaState == MediaState.Started) recorder.pause(); else recorder.record()
                        }
                    }
                    //! [3]

                    //! [4]
                    // The 'Play' button
                    ImageButton {
                        leftMargin: 2
                        defaultImageSource: "asset:///images/play_button.png"
                        pressedImageSource: "asset:///images/play_button_pressed.png"
                        disabledImageSource: "asset:///images/play_button.png"
                        enabled: (_trackManager.hasRecordedTracks && recorder.mediaState != MediaState.Started && recorder.mediaState != MediaState.Paused)
                        onClicked: navigationPane.push(playerPage.createObject())
                    }
                    //! [4]
                }
            }
        }

        //! [5]
        attachedObjects: [
            AudioRecorder {
                id: recorder
            },
            ComponentDefinition {
                id: playerPage
                source: "PlayerPage.qml"
            },
            SystemSound {
                id: recordStartSound
                sound: SystemSound.RecordingStartEvent
            },
            SystemSound {
                id: recordStopSound
                sound: SystemSound.RecordingStopEvent
            },
            // application supports changing the Orientation
            OrientationHandler {
                // onOrientationChanged: { should be this from docs, but onOrientationAboutToChange runs smoother
                onOrientationAboutToChange: {
                    mainPage.reLayout(orientation);
                }
            }
        ]
        //! [5]

        function reLayout(orientation) {
            if (orientation == UIOrientation.Landscape) {
                imageBackground.imageSource = "asset:///images/background_landscape.png"
                led.translationY = 40
                tape.translationY = 120
                tape.translationX = -30
                tape.horizontalAlignment = HorizontalAlignment.Right
                label.verticalAlignment = VerticalAlignment.Top
                label.translationY = 50
                label.translationX = 260
                buttonBackground.horizontalAlignment = HorizontalAlignment.Right
                buttonBackground.translationX = -40
                buttonContainer.horizontalAlignment = HorizontalAlignment.Right
                buttonContainer.translationX = -40
            } else {
                imageBackground.imageSource = "asset:///images/background.png"
                led.translationY = 500
                tape.translationY = 570
                tape.translationX = 0
                tape.horizontalAlignment = HorizontalAlignment.Center
                label.verticalAlignment = VerticalAlignment.Bottom
                label.translationY = -205
                label.translationX = 0
                buttonBackground.horizontalAlignment = HorizontalAlignment.Center
                buttonBackground.translationX = 0
                buttonContainer.horizontalAlignment = HorizontalAlignment.Center
                buttonContainer.translationX = 0
            }
        }

        onCreationCompleted: {
            // support portrait and landscape
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
            reLayout(OrientationSupport.orientation);
        }
    }
}
