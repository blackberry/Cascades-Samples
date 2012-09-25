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
import bb.multimedia 1.0

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

//! [0]
        // The led object
        Led {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Top

            translationX: 40
            translationY: 500

            state: (recorder.mediaState == MediaState.Started ? "rec" :
                    recorder.mediaState == MediaState.Paused ? "pause" : "off")
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
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            translationY: -205

            imageSource: "asset:///images/dictaphone_label.png"
        }

        // The background image of the button container
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            imageSource: "asset:///images/buttons_background.png"
        }

        // The button container
        Container {
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
                        recorder.outputUrl = _trackManager.nextTrackUrl()

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

                enabled: (recorder.mediaState == MediaState.Started ||
                          recorder.mediaState == MediaState.Paused)

                onCheckedChanged: {
                    if (recorder.mediaState == MediaState.Started)
                        recorder.pause()
                    else
                        recorder.record()
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

                enabled: (_trackManager.hasRecordedTracks &&
                          recorder.mediaState != MediaState.Started &&
                          recorder.mediaState != MediaState.Paused)

                onClicked: playerSheet.open()
            }
//! [4]
        }
    }

//! [5]
    attachedObjects: [
        AudioRecorder {
            id: recorder
        },
        PlayerSheet {
            id: playerSheet
        },
        SystemSound {
            id: recordStartSound
            sound: SystemSound.RecordingStartEvent
        },
        SystemSound {
            id: recordStopSound
            sound: SystemSound.RecordingStopEvent
        }
    ]
//! [5]
}
