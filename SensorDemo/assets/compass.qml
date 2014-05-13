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
    //! [0]
    attachedObjects: [
        OrientationHandler {
            onOrientationAboutToChange: { // Callend whenever the screen orientation changes

                // Adapt the compass to current screen orientation
                if (displayDirection == DisplayDirection.North) {
                    compass.userOrientation = 0;
                } else if (displayDirection == DisplayDirection.South) {
                    compass.userOrientation = 180;
                } else if (displayDirection == DisplayDirection.East) {
                    compass.userOrientation = 270;
                } else if (displayDirection == DisplayDirection.West) {
                    compass.userOrientation = 90;
                }
            }
        },

        Compass {
            id: compass

            // Create a variable to hold azimuth
            property double azimuth: 0

            // Turn on the sensor
            active: true

            // Change sensor axis depending on 'userOrientation' property
            axesOrientationMode: Compass.UserOrientation

            onReadingChanged: { // Called when a new compass reading is available
                compass.azimuth = reading.azimuth;
            }
        },

        OrientationSensor {
            id: orientation

            // Create a variable to hold orientation
            property bool face_down: false

            // Turn on the sensor
            active: true

            onReadingChanged: { // Called when a new orientation reading is available
                face_down = (reading.orientation == OrientationReading.FaceDown);
            }
        }
    ]
    //! [0]

    layout: DockLayout {}

    // The compass in face-up device orientation
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        opacity: orientation.face_down ? 0 : 1

        //! [1]
        Label {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("%1\u00B0").arg(compass.azimuth.toFixed())
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
        //! [1]

        //! [2]
        ImageView {
            imageSource: "images/MID-Dial.png"
            rotationZ: - compass.azimuth
            maxWidth: ui.du(55.5)
            maxHeight: ui.du(55.5)
            attachedObjects: [
                ImplicitAnimationController {
                    // Disable animations to avoid jumps between 0 and 360 degree
                    enabled: false
                }
            ]
        }
        //! [2]

        attachedObjects: [
            ImplicitAnimationController {
                enabled: false
            }
        ]
    }

    // The compass in face-down device orientation
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        opacity: orientation.face_down ? 1 : 0

        Label {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("%1\u00B0").arg(compass.azimuth.toFixed())
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.Yellow
                fontWeight: FontWeight.Bold
            }
        }

        ImageView {
            imageSource: "images/MID-Dial-reverse.png"
            rotationZ: 360 + compass.azimuth
            maxWidth: 500.0
            maxHeight: 500.0

            attachedObjects: [
                ImplicitAnimationController {
                    // Disable animations to avoid jumps between 0 and 360 degree
                    enabled: false
                }
            ]
        }

        attachedObjects: [
            ImplicitAnimationController {
                enabled: false
            }
        ]
    }
}
