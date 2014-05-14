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
import bb.cascades.places 1.0

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }
        PlacepickerScrollView {
            horizontalAlignment: HorizontalAlignment.Fill

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                leftPadding: ui.du(3.3)
                topPadding: ui.du(5.6)
                rightPadding: ui.du(3.3)

                //! [0]
                // The 'Select Place' button
                Button {
                    horizontalAlignment: HorizontalAlignment.Center

                    text: qsTr("Select Place")

                    onClicked: placePicker.selectPlace()
                }
                //! [0]

                // The container with all the place fields
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    topMargin: ui.du(5.6)

                    //! [1]
                    FieldLabel {
                        title: qsTr("name")
                        field: "name"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("alternative name")
                        field: "alternativeName"
                        selectedPlace: placePicker.selectedPlace
                    }
                    //! [1]

                    FieldLabel {
                        title: qsTr("description")
                        field: "description"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("street")
                        field: "street"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("city")
                        field: "city"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("region")
                        field: "region"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("country")
                        field: "country"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("postal code")
                        field: "postal"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("address label")
                        field: "addressLabel"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("phone")
                        field: "phone"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("secondary phone")
                        field: "secondaryPhone"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("fax")
                        field: "fax"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("email")
                        field: "email"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("web site")
                        field: "url"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldNumber {
                        title: qsTr("rating")
                        field: "rating"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldLabel {
                        title: qsTr("map URL")
                        field: "mapURL"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldNumber {
                        title: qsTr("latitude")
                        field: "latitude"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldNumber {
                        title: qsTr("longitude")
                        field: "longitude"
                        selectedPlace: placePicker.selectedPlace
                    }

                    FieldNumber {
                        title: qsTr("altitude")
                        field: "altitude"
                        selectedPlace: placePicker.selectedPlace
                    }
                }
            }
        }
    }

    //! [2]
    attachedObjects: [
        PlacePicker {
            id: placePicker

            property variant selectedPlace

            function selectPlace()
            {
                selectedPlace = show()
            }
        }
    ]
    //! [2]
}
