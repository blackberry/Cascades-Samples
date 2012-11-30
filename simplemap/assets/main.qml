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
import bb.cascades.maps 1.0

Page {
    Container {
        layout: DockLayout {}

        //! [0]
        // The map view
        MapView {
            id: mapView

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            // Use a decent height as initial value
            altitude: 2000

            // Update the latitude/longitude automatically when user selects a different city
            latitude: cityDropDown.selectedValue.latitude
            longitude: cityDropDown.selectedValue.longitude

            tilt: 2
        }
        //! [0]

        // The control bar at the top
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            background: Color.create("#aaffffff")

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            //! [1]
            // The 'Zoom in' button
            Button {
                text: qsTr("+")

                // Disable button if altitude is less than or equal to 500 meter
                enabled: mapView.altitude > 500

                // Decrease the altitude on every click
                onClicked: mapView.altitude -= 500
            }
            //! [1]

            //! [2]
            // The city selection drop down
            DropDown {
                id: cityDropDown

                title: qsTr("City")

                Option {
                    text: qsTr("Ottawa")

                    // Encode the coordinates as JSON object, which will be mapped to QVariantMap internally
                    value: { "latitude": 45.42354, "longitude": -75.69797 }
                    selected: true
                }

                Option {
                    text: qsTr("Los Angeles")
                    value: { "latitude": 34.05, "longitude": -118.25 }
                }

                Option {
                    text: qsTr("Waterloo")
                    value: { "latitude": 43.4652, "longitude": -80.5224 }
                }
            }
            //! [2]

            //! [3]
            // The 'Zoom out' button
            Button {
                text: qsTr("-")

                // Disable button if altitude is larger than or equal to 5000 meter
                enabled: mapView.altitude < 5000

                // Increase the altitude on every click
                onClicked: mapView.altitude += 500
            }
            //! [3]
        }

        //! [4]
        // The altitude slider at the bottom
        Slider {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            opacity: 0.4

            // Use the same range like for the zoom-in/zoom-out buttons
            fromValue: 500
            toValue: 5000

            // Keep slider in sync with current altitude value
            value: mapView.altitude

            onValueChanged: {
                // Change the altitude of the map view
                mapView.altitude = Math.floor(value)
            }
        }
        //! [4]
    }
}
