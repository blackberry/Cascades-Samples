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
import QtMobility.sensors 1.2
import bb.vibrationController 1.0

Container {
    leftPadding:  ui.du(2.2)
    rightPadding:  ui.du(2.2)
    bottomPadding:  ui.du(2.2)

    //! [0]
    attachedObjects: [
        Magnetometer {
            id: metalfinder

            // Create various variables to hold values from magnetometer reading
            property double baseline: 0
            property double magnitude: 0
            property double intensity: 0
            property double x: 0
            property double y: 0
            property double z: 0

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            onReadingChanged: { // Called when a magnetometer reading is available
                magnitude = Math.sqrt(reading.x * reading.x + reading.y * reading.y + reading.z * reading.z);
                if (0 == baseline) {
                    baseline = magnitude;
                }
                intensity = ((magnitude - baseline) / magnitude) * 100;
                if (intensity > 0) {
                    vib.start(intensity, 100);
                }
                x = reading.x;
                y = reading.y;
                z = reading.z;
            }
        },

        VibrationController {
            id: vib
        }
    ]
    //! [0]

    layout: DockLayout {}

    //! [1]
    Button {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        text: qsTr("Calibrate Metal Finder")

        onClicked: {
            metalfinder.baseline = metalfinder.magnitude
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

            text: qsTr("X: %1").arg((metalfinder.x * 1000000).toPrecision(5))
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

            text: qsTr("Y: %1").arg((metalfinder.y * 1000000).toPrecision(5))
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }

        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            text: qsTr("Z: %1").arg((metalfinder.z * 1000000).toPrecision(5))

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
    }
}
