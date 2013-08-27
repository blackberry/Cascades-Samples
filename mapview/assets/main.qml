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
import bb.cascades 1.0
import QtMobility.sensors 1.2
import bb.cascades.maps 1.0
import QtMobilitySubset.location 1.1

Page {
    actions: [
        //! [0]
        ActionItem {
            title: qsTr("Drop Pin")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _mapViewTest.addPinAtCurrentMapCenter();
            }
        },
        ActionItem {
            title: qsTr("Remove Pins")
            imageSource: "asset:///images/clearpin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _mapViewTest.clearPins();
            }
        },
        ActionItem {
            title: qsTr("Center URL")
            imageSource: "asset:///images/url.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                status.setText(mapview.url());
            }
        },
        ActionItem {
            title: qsTr("Waterloo")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                mapview.latitude = 43.468245;
                mapview.longitude = -80.519603;
            }
        },
        ActionItem {
            title: qsTr("Manhattan")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                mapview.latitude = 40.791556;
                mapview.longitude = -73.967394;
            }
        }
    //! [0]
    ]
    Container {
        id: root
        layout: DockLayout {
        }
        //! [1]
        MapView {
            id: mapview
            objectName: "mapViewObj"
            altitude: 3000
            latitude: 43.449488
            longitude: -80.406777
            preferredWidth: 768
            preferredHeight: 1280
            onAltitudeChanged: {
                alt.setText(qsTr("Alt: %1").arg(newAlt));
            }
            onHeadingChanged: {
                heading.setText(qsTr("Heading: %1\u00B0").arg(newHeading));
            }
            onLatitudeChanged: {
                lat.setText(qsTr("Lat: %1").arg(newLat));
            }
            onLongitudeChanged: {
                lon.setText(qsTr("Lon: %1").arg(newLon));
            }
            onTiltChanged: {
                tilt.setText(qsTr("Tilt: %1\u00B0").arg(newTilt));
            }
            onMapLongPressed: {
                status.setText(qsTr("map long pressed"));
            }

            onFollowedIdChanged: {
                status.setText(qsTr("followed id changed to %1").arg(idOfFollowed));
            }
            onFocusedIdChanged: {
                status.setText(qsTr("focused id changed to %1").arg(idWithFocus));
            }
            onCaptionButtonClicked: {
                status.setText(qsTr("button clicked %1").arg(focusedId));
            }
            onCaptionLabelTapped: {
                status.setText(qsTr("label clicked %1").arg(focusedId));
            }
            onLocationTapped: {
                status.setText(qsTr("location tapped %1").arg(id));
            }
            onLocationLongPressed: {
                status.setText(qsTr("location long pressed %1").arg(id));
            }
        }
        //! [1]
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            topPadding: 5
            leftPadding: 5
            bottomPadding: 5
            background: Color.create("#ddffffff")

            //! [2]
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                horizontalAlignment: HorizontalAlignment.Center
                //! [2]
                Label {

                    id: lat
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: lon
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: alt
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: heading
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                    }
                }
                Label {
                    id: tilt
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                    }
                }
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                Label {
                    id: status
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Gray
                        fontWeight: FontWeight.Bold
                    }
                }
            }
            //! [2]
        }
        //! [3]
        Container {
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            topPadding: 20
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Bottom
            overlapTouchPolicy: OverlapTouchPolicy.Allow

            ImageView {
                id: compassImage
                imageSource: "asset:///images/compass.png"
                horizontalAlignment: HorizontalAlignment.Center
                attachedObjects: [
                    ImplicitAnimationController {
                        // Disable animations to avoid jumps between 0 and 360 degree
                        enabled: false
                    }
                ]
            }
            ToggleButton {
                id: sensorToggle
                horizontalAlignment: HorizontalAlignment.Center
                checked: true
                onCheckedChanged: {
                    if (checked) {
                        mapview.setFollowedId("device-location-id");
                    } else {
                        mapview.setFollowedId("");
                    }
                }
                onCreationCompleted: {
                    mapview.setFollowedId("device-location-id");
                }
            }
        }
        //! [3]
    }
    attachedObjects: [
        //! [5]
        RotationSensor {
            id: rotation
            property real x: 0
            active: sensorToggle.checked
            alwaysOn: false
            skipDuplicates: true
            onReadingChanged: {
                x = reading.x - 30
                if (x <= 40 && x > 0) {
                    mapview.setTilt(x);
                }
            }
        },
        //! [5]
        //! [4]
        Compass {
            property double azimuth: 0
            active: sensorToggle.checked
            axesOrientationMode: Compass.UserOrientation
            alwaysOn: false
            onReadingChanged: { // Called when a new compass reading is available
                mapview.setHeading(reading.azimuth);
                compassImage.rotationZ = 360 - reading.azimuth;
            }
        },
    //! [4]
    //! [6]
        PositionSource {
            id: positionSource
            updateInterval: 1000
            active: sensorToggle.checked
            onPositionChanged: {
                _mapViewTest.updateDeviceLocation(positionSource.position.coordinate.latitude, positionSource.position.coordinate.longitude);
            }
        }
        //! [6]
    ]
}
