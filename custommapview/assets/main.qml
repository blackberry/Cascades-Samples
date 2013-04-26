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
                status.setText(qsTr("altitude changed: %1").arg(newAlt));
            }
            onHeadingChanged: {
                status.setText(qsTr("heading changed: %1").arg(newHeading));
            }
            onLatitudeChanged: {
                status.setText(qsTr("latitude changed: %1").arg(newLat));
            }
            onLongitudeChanged: {
                status.setText(qsTr("longitude changed: %1").arg(newLon));
            }
            onTiltChanged: {
                status.setText(qsTr("tilt changed: %1").arg(newTilt));
            }
            onMapLongPressed: {
                status.setText(qsTr("map long pressed"));
            }
            onCreationCompleted: {
                setCaptionGoButtonVisible(false);
            }
            onFollowedIdChanged: {
                status.setText(qsTr("followed id changed to %1").arg(idOfFollowed));
            }
            onCaptionButtonClicked: {
                status.setText(qsTr("%1 button clicked!").arg(focusedId));
            }
            onCaptionLabelTapped: {
                status.setText(qsTr("%1 label clicked!").arg(focusedId));
            }
        }
        //! [1]
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            topPadding: 20
            leftPadding: 20
            bottomPadding: 20
            background: Color.create("#aaffffff");

            //! [2]
            Label {
                id: status
                multiline: true
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    color: Color.Black
                    fontWeight: FontWeight.Bold
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
            ImageView {
                id: compassImage
                overlapTouchPolicy: OverlapTouchPolicy.Allow
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
