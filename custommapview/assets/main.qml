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

// creates one page with a label

Page {
    actions: [
        //! [0]
        ActionItem {
            title: qsTr("Drop Pin")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                pinContainer.addPin(mapview.latitude, mapview.longitude);
            }
        },
        ActionItem {
            title: qsTr("Ramove Pins")
            imageSource: "asset:///images/clearpin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                pinContainer.removeAll();
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
            altitude: 3000
            latitude: 43.449488
            longitude: -80.406777
            preferredWidth: 768
            preferredHeight: 1280
            onAltitudeChanged: {
                status.setText("altitudeChanged: " + newAlt);
            }
            onHeadingChanged: {
                status.setText("headingChanged: " + newHeading);
            }
            onLatitudeChanged: {
                status.setText("latitudeChanged: " + newLat);
            }
            onLongitudeChanged: {
                status.setText("longitudeChanged: " + newLon);
            }
            onTiltChanged: {
                status.setText("tiltChanged: " + newTilt);
            }
            onMapLongPressed: {
                status.setText("mapLongPress: ");
            }
            onRequestRender: {
                pinContainer.updateMarkers();
            }
        }
        //! [1]
        //! [2]
        Label {
            id: status
            multiline: true
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                color: Color.Red
                fontWeight: FontWeight.Bold
            }
        }
        //! [2]
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
                        pinContainer.showMe();
                        pinContainer.me.visible = false;
                    } else {
                        pinContainer.me.visible = false;
                    }
                }
                onCreationCompleted: {
                    pinContainer.showMe();
                    pinContainer.me.visible = false;
                }
            }
        }
        //! [3]
        //! [7]
        Container {
            id: pinContainer
            // Must match the mapview width and height and position
            preferredHeight: 1280
            preferredWidth: 768
            //touchPropagationMode: TouchPropagationMode.PassThrough
            overlapTouchPolicy: OverlapTouchPolicy.Allow
            property variant currentBubble
            property variant me
            layout: AbsoluteLayout {
            }
            function addPin(lat, lon) {
                var marker = pin.createObject();
                marker.lat = lat;
                marker.lon = lon;
                var xy = mapViewTest.worldToPixelInvokable(mapview, marker.lat, marker.lon);
                marker.x = xy.split(" ")[0];
                marker.y = xy.split(" ")[1];
                pinContainer.add(marker);
                marker.animDrop.play();
            }
            function showBubble(pin) {
                pinContainer.remove(currentBubble);
                var details = bubble.createObject();
                details.lat = pin.lat;
                details.lon = pin.lon;
                var xy = mapViewTest.worldToPixelInvokable(mapview, details.lat, details.lon);
                details.x = xy.split(" ")[0];
                details.y = xy.split(" ")[1];
                pinContainer.add(details);
                details.play();
                currentBubble = details;
            }
            function showMe() {
                var marker = pin.createObject();
                marker.pinImageSource = "asset:///images/me.png"
                marker.pointerOffsetX = 30
                marker.pointerOffsetY = 30
                pinContainer.insert(-1, marker);
                marker.visible = false;
                me = marker;
            }
            function updateMarkers() {
                mapViewTest.updateMarkers(mapview, pinContainer);
            }
            function removeBubble() {
                pinContainer.remove(currentBubble);
            }
            onTouch: {
                if (event.isDown()) {
                    if ((event.localX <= currentBubble.actualX) || (event.localX >= currentBubble.actualX + currentBubble.contentWidth) || (event.localY <= currentBubble.actualY) || (event.localY >= currentBubble.actualY + currentBubble.contentHeight)) {
                        removeBubble();
                    }
                }
            }
        }
        //! [7]
    }
    attachedObjects: [
        //! [5]
        ComponentDefinition {
            id: pin
            source: "pin.qml"
        },
        ComponentDefinition {
            id: bubble
            source: "bubble.qml"
        },
        //! [5]
        //! [6]
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
        //! [6]
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
        PositionSource {
            id: positionSource
            updateInterval: 1000
            active: sensorToggle.checked
            onPositionChanged: {
                mapview.latitude = positionSource.position.coordinate.latitude;
                mapview.longitude = positionSource.position.coordinate.longitude;
                pinContainer.me.lat = positionSource.position.coordinate.latitude;
                pinContainer.me.lon = positionSource.position.coordinate.longitude;
                var xy = mapViewTest.worldToPixelInvokable(mapview, pinContainer.me.lat, pinContainer.me.lon);
                pinContainer.me.x = xy.split(" ")[0];
                pinContainer.me.y = xy.split(" ")[1];
                pinContainer.me.visible = true;
            }
        }
    //! [4]
    ]
}
