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
    bottomPadding: ui.du(2)

    //! [0]
    attachedObjects: [
        SystemSound {
            id: sound
            sound: SystemSound.CameraBurstEvent
        },

        IRProximitySensor {
            id: irProximity

            // Create a variable to hold movement state
            property real reflectance: 0

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            onReadingChanged: { // Called when a new IR proximity reading is available
                reflectance = reading.reflectance
            }
        },

        ProximitySensor {
            id: proximity

            // Create a variable to hold movement state
            property bool close: false

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            onReadingChanged: { // Called when a new proximity reading is available
                close = reading.close
                if (close) {
                    sound.play();
                    anim.play();
                }
            }
        }
    ]
    //! [0]

    layout: DockLayout {}

    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom

        text: qsTr("Reflectance: %1%").arg((irProximity.reflectance * 100).toPrecision(4))
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
        }
    }

    //! [1]
    Label {
        id: alert

        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        text: proximity.close ? qsTr("!! Mayday, Mayday !!") : qsTr("Cruising")
        textStyle {
            base: SystemDefaults.TextStyles.BigText
            color: proximity.close ? Color.Red : Color.White
        }

        animations: [
            FadeTransition {
                id: anim
                fromOpacity: 0.5
                toOpacity: 1
                duration: 300
                easingCurve: StockCurve.BounceIn
            }
        ]
    }
    //! [1]
}
