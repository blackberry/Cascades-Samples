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
import Flashlight 1.0

Container {
    topPadding: ui.du(2.2)
    leftPadding: ui.du(2.2)
    rightPadding: ui.du(2.2)

    //! [0]
    attachedObjects: [
        Flashlight {
            id: flashlight
        },

        Gyroscope {
            id: accel

            // Create a variable to count the number of sensor readings
            property int count: 0

            // Turn on the sensor
            active: true

            // Don't change sensor axis on screen rotation
            axesOrientationMode: Gyroscope.FixedOrientation

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            // If the device isn't moving (x&y&z==0), don't send updates, saves power
            skipDuplicates: true

            onReadingChanged: { // Called when a gyroscope reading is available
                // Check whether rotation on Z axis is above the configured threshold
                if (reading.z > gyroSlider.value.toFixed() * 360 / (2 * 3.14)) {
                    if (count > 5 && ambLight.val < lightDropDown.selectedIndex) {
                        // If after 5 readings the ambient light is darker than the configured threshold, toggle the flash light
                        count = 0
                        flashlight.enabled = ! flashlight.enabled;
                    }
                }
                count ++;
            }
        },

        AmbientLightSensor {
            id: ambLight

            // Create a variable to hold the ambient light value
            property variant val: AmbientLightSensor.Undefined

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            // If the device isn't moving (x&y&z==0), don't send updates, saves power
            skipDuplicates: true

            onReadingChanged: { // Called when an ambient light reading is available
                val = reading.lightLevel;
                if (ambLight.val >= lightDropDown.selectedIndex && flashlight.enabled) {
                    // Switch off the flash light if current ambient light is brigther than configured threshold
                    flashlight.enabled = false;
                }
            }
        },

        LightSensor {
            id: light

            // Create a variable to hold the ambient light value
            property real val: 0

            // Turn on the sensor
            active: true

            // Keep the sensor active when the app isn't visible or the screen is off (requires app permission in bar-descriptor)
            alwaysOn: true

            // If the device isn't moving (x&y&z==0), don't send updates, saves power
            skipDuplicates: true

            onReadingChanged: { // Called when an light reading is available
                val = reading.lux;
            }
        }
    ]
    //! [0]

    TextArea {
        text: qsTr("To enable the flash light give the device a clockwise flick with an angular velocity greater then 'Gyro Threshold'. The flashlight is only enabled in environments darker then 'Light Threshold'. The flashlight will automatically turn off in enviroments brighter then 'Light Threshold'.")
    }

    Label {
        text: {
            if (light.val == AmbientLightReading.Dark) {
                return qsTr("Light: Dark");
            } else if (light.val == AmbientLightReading.Twilight) {
                return qsTr("Light: Twilight");
            } else if (light.val == AmbientLightReading.Light) {
                return qsTr("Light: Light");
            } else if (light.val == AmbientLightReading.Bright) {
                return qsTr("Light: Bright");
            } else if (light.val == AmbientLightReading.Sunny) {
                return qsTr("Light: Sunny");
            } else {
                return qsTr("Light: Undefined");
            }
        }
    }

    DropDown {
        id: lightDropDown
        title: qsTr("Light Threshold")

        Option {
            text: qsTr("Undefined")
        }
        Option {
            text: qsTr("Dark")
        }
        Option {
            text: qsTr("Twilight")
            selected: true
        }
        Option {
            text: qsTr("Light")
        }
        Option {
            text: qsTr("Bright")
        }
        Option {
            text: qsTr("Sunny")
        }
    }

    Label {
        topMargin: ui.du(5.6)

        text: qsTr("Gyro Threshold: %1").arg(gyroSlider.value.toFixed())
    }

    Slider {
        id: gyroSlider
        value: 10
        fromValue: 0
        toValue: 50
    }

    //! [1]
    Label {
        horizontalAlignment: HorizontalAlignment.Center

        text: qsTr("Luminosity: %1 lux").arg(Math.round(light.val))
        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            color: Color.White
            lineHeight: ui.du(0.2)
        }
    }
    //! [1]
}
