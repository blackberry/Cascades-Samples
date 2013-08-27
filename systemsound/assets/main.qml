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

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            topPadding: 300
            leftPadding: 30
            rightPadding: 30

            //! [0]
            // The selector for system sounds
            DropDown {
                id: soundChooser

                horizontalAlignment: HorizontalAlignment.Center

                title: qsTr ("Sound")

                Option {
                    text: qsTr ("Battery Alarm")
                    value: SystemSound.BatteryAlarm
                    selected: true
                }

                Option {
                    text: qsTr ("Browser Start Event")
                    value: SystemSound.BrowserStartEvent
                }

                Option {
                    text: qsTr ("Camera Shutter Event")
                    value: SystemSound.CameraShutterEvent
                }

                Option {
                    text: qsTr ("Device Lock Event")
                    value: SystemSound.DeviceLockEvent
                }

                Option {
                    text: qsTr ("Device Unlock Event")
                    value: SystemSound.DeviceUnlockEvent
                }

                Option {
                    text: qsTr ("Device Tether Event")
                    value: SystemSound.DeviceTetherEvent
                }

                Option {
                    text: qsTr ("Device Untether Event")
                    value: SystemSound.DeviceUntetherEvent
                }

                Option {
                    text: qsTr ("General Notification")
                    value: SystemSound.GeneralNotification
                }

                Option {
                    text: qsTr ("Input Keypress")
                    value: SystemSound.InputKeypress
                }

                Option {
                    text: qsTr ("Recording Start Event")
                    value: SystemSound.RecordingStartEvent
                }

                Option {
                    text: qsTr ("Recording Stop Event")
                    value: SystemSound.RecordingStopEvent
                }

                Option {
                    text: qsTr ("Sapphire Notification")
                    value: SystemSound.SapphireNotification
                }

                Option {
                    text: qsTr ("System Master Volume Reference")
                    value: SystemSound.SystemMasterVolumeReference
                }

                Option {
                    text: qsTr ("Video Call Event")
                    value: SystemSound.VideoCallEvent
                }

                Option {
                    text: qsTr ("Video Call Outgoing Event")
                    value: SystemSound.VideoCallOutgoingEvent
                }

                Option {
                    text: qsTr ("Camera Burst Event")
                    value: SystemSound.CameraBurstEvent
                }
            }
            //! [0]

            //! [1]
            // The button to play the selected sound
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 100

                text: qsTr ("Play System Sound")
                onClicked: systemSound.play()
            }
            //! [1]
        }
    }

    //! [2]
    attachedObjects: [
        SystemSound {
            id: systemSound
            sound: soundChooser.selectedValue
        }
    ]
    //! [2]
}
