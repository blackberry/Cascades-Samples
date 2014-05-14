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

import bb.cascades 1.3
import bb.multimedia 1.0
import QtMobility.sensors 1.2

Container {
    leftPadding: ui.du(2.2)
    rightPadding:  ui.du(2.2)
    bottomPadding:  ui.du(2.2)

    //! [0]
    attachedObjects: [
        SystemSound {
            id: sound
            sound: SystemSound.InputKeypress
        },

        Accelerometer {
            id: alarm

            // Create variables to hold movement state and reading values
            property double x: 0
            property double y: 0
            property double z: 0
            property bool movement: false

            // Turn on the sensor
            active: true

            // Don't change sensor axis on screen rotation
            axesOrientationMode: Accelerometer.FixedOrientation

            // Remove gravity, only interested in user movement
            accelerationMode: Accelerometer.User

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            // If the device isn't moving (x&y&z==0), don't send updates, saves power
            skipDuplicates: true

            onReadingChanged: { // Called when a new user accel reading is available
                x = reading.x;
                y = reading.y;
                z = reading.z;
                movement = Math.sqrt(reading.x * reading.x + reading.y * reading.y + reading.z * reading.z) > .2;
                if (movement) {
                    sound.play(); // Movement detected, play a sound
                }
            }
        }
    ]
    //! [0]

    layout: DockLayout {}

    //! [1]
    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        text: alarm.movement ? qsTr("ALARM: Movement Detected!") : qsTr("No Movement Detected")
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            color: alarm.movement ? Color.Red : Color.White
        }
    }
    //! [1]

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Bottom

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        //! [2]
        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            text: qsTr("X: %1").arg((alarm.x).toPrecision(5))

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
        //! [2]

        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            text: qsTr("Y: %1").arg((alarm.y).toPrecision(5))

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }

        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            text: qsTr("Z: %1").arg((alarm.z).toPrecision(5))

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
    }
}
