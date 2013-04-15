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
    //! [0]
    // The actions of the page
    actions: [
        ActionItem {
            title: qsTr("Drop Pin")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var marker = pin.createObject();
                marker.lat = mapview.latitude;
                marker.lon = mapview.longitude;
                marker.x = _mapViewTest.longitudeToX(mapview, marker.lon);
                marker.y = _mapViewTest.latitudeToY(mapview, marker.lat);
                pinBubble.add(marker);
                marker.anim.play();
            }
        },
        ActionItem {
            title: qsTr("Remove Pins")
            imageSource: "asset:///images/clearpin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                pinBubble.removeAll();
            }
        },
        ActionItem {
            title: qsTr("Center URL")
            imageSource: "asset:///images/url.png"
            ActionBar.placement: ActionBarPlacement.OnBar
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
    ]
    //! [0]
    // The main content of the page
    Container {
        id: root

        layout: DockLayout {
                }
        //! [1]
        function updateMarkers(x, y) {
            for (var i = 0; i < pinBubble.count(); i ++) {
                if (x) {
                    pinBubble.at(i).x = _mapViewTest.longitudeToX(mapview, pinBubble.at(i).lon)
                }
                if (y) {
                    pinBubble.at(i).y = _mapViewTest.latitudeToY(mapview, pinBubble.at(i).lat);
                }
            }
        }

        MapView {
            id: mapview

            latitude: 43.468245
            longitude: -80.519603
            altitude: 10000

            onAltitudeChanged: {
                status.setText(qsTr("altitude changed: %1").arg(newAlt));
                root.updateMarkers(true, true);
            }
            onHeadingChanged: {
                status.setText(qsTr("heading changed: %1").arg(newHeading));
            }
            onLatitudeChanged: {
                status.setText(qsTr("latitude changed: %1").arg(newLat));
                root.updateMarkers(false, true);
            }
            onLongitudeChanged: {
                status.setText(qsTr("longitude changed: %1").arg(newLon));
                root.updateMarkers(true, false);
            }
            onTiltChanged: {
                status.setText(qsTr("tilt changed: %1").arg(newTilt));
            }
            onMapLongPressed: {
                status.setText(qsTr("map long pressed"));
            }
        }
        //! [1]
        // Overlay container to place the pins on the map
        Container {
            id: pinBubble
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredHeight: maxHeight
            preferredWidth: maxWidth
            layout: AbsoluteLayout {
            }
            overlapTouchPolicy: OverlapTouchPolicy.Allow
        }

        //! [2]
        // Container that contains the status label
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            topPadding: 20
            bottomPadding: 20

            background: Color.create("#aaffffff");

            Label {
                id: status
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    color: Color.Black
                    fontWeight: FontWeight.Bold
                    fontFamily: "courier"
                    textAlign: TextAlign.Center
                }

                multiline: true
            }
        }
        //! [2]

        //! [3]
        // Container that contains the sliders
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            topPadding: 10
            leftPadding: 20
            rightPadding: 10
            bottomPadding: 10

            layout: StackLayout {
            }

            background: Color.create("#aaffffff");

            // The altitude slider
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                Label {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }

                    text: qsTr("Altitude: %1").arg(Math.round(altitudeSlider.immediateValue) * 2000)
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                }

                Slider {
                    id: altitudeSlider

                    horizontalAlignment: HorizontalAlignment.Center

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }

                    value: 30
                    fromValue: 1
                    toValue: 500

                    onImmediateValueChanged: {
                        mapview.setAltitude(Math.round(immediateValue) * 2000)
                    }
                    onValueChanged: {
                        mapview.setAltitude(Math.round(value) * 2000)
                    }
                }
            }

            // The tilt slider
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                Label {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }

                    text: qsTr("Tilt: %1\u00B0").arg(Math.round(tiltSlider.immediateValue))
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                }

                Slider {
                    id: tiltSlider

                    horizontalAlignment: HorizontalAlignment.Center

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }

                    fromValue: 0
                    toValue: 45

                    onImmediateValueChanged: {
                        mapview.setTilt(Math.round(immediateValue))
                    }
                }
            }

            // The heading slider
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                Label {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }

                    text: qsTr("Heading: %1\u00B0").arg(Math.round(headingSlider.immediateValue))
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Black
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                }

                Slider {
                    id: headingSlider

                    horizontalAlignment: HorizontalAlignment.Center

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }

                    fromValue: -180
                    toValue: 180

                    onImmediateValueChanged: {
                        mapview.setHeading(Math.round(immediateValue))
                    }
                }
            }
        }
        //! [3]
    }
    //! [4]
    attachedObjects: [
        ComponentDefinition {
            id: pin
            source: "pin.qml"
        },
        ComponentDefinition {
            id: bubble
            source: "bubble.qml"
        }
    ]
    //! [4]
}
