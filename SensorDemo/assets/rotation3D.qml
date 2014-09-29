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
    //! [0]
    attachedObjects: [
        RotationSensor {
            id: rotation

            // Create variables to hold rotation reading values
            property real x: 0
            property real y: 0
            property real z: 0

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            // If the device isn't moving (x&y&z==0), don't send updates, saves power
            skipDuplicates: true

            onReadingChanged: { // Called when a new rotation reading is available
                x = reading.x
                y = reading.y
                z = reading.z
            }
        }
    ]
    //! [0]

    layout: DockLayout {}

    ImageView {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        imageSource: "images/device.png"
    }

    Container {
        layout: AbsoluteLayout {}

        //! [1]
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: ui.du(51)
                positionY: ui.du(70)
            }

            text: qsTr("%1\u00B0").arg(rotation.x.toPrecision(4))
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
        //! [1]

        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: ui.du(53.3)
                positionY: ui.du(51.1)
            }

            text: qsTr("%1\u00B0").arg(rotation.y.toPrecision(4))
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }

        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: ui.du(37.2)
                positionY: ui.du(40)
            }

            text: qsTr("%1\u00B0").arg(rotation.z.toPrecision(4))
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
    }
}
