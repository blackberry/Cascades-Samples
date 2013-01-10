// Default empty project template
import bb.cascades 1.0
import bb.cascades.maps 1.0

// creates one page with a label

Page {
    actions: [
        //! [0]
        ActionItem {
            title: qsTr("Drop Pin")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                var marker = pin.createObject();
                marker.lat = mapview.latitude;
                marker.lon = mapview.longitude;
                var xy = mapViewTest.worldToPixel(mapview, marker.lat, marker.lon);
                // 36, 65 is the position of the pixel in pin.png that should point to the location.
                marker.x = xy.split(" ")[0] - 36;
                marker.y = xy.split(" ")[1] - 65;
                pinContainer.add(marker);
                marker.anim.play();
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
    //! [0]
    ]
    Container {
        id: root
        //! [1]
        function updateMarkers() {
            for (var i = 0; i < pinContainer.count(); i ++) {
                var xy = mapViewTest.worldToPixel(mapview, pinContainer.at(i).lat, pinContainer.at(i).lon);
                // 36, 65 is the position of the pixel in pin.png that should point to the location.
                pinContainer.at(i).x = xy.split(" ")[0] - 36;
                pinContainer.at(i).y = xy.split(" ")[1] - 65;
            }
        }
        layout: DockLayout {
        }
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
                root.updateMarkers();
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
        Container {
            id: pinContainer
            // Must match the mapview width and height and position
            preferredHeight: 1280
            preferredWidth: 768
            layout: AbsoluteLayout {
            }
            overlapTouchPolicy: OverlapTouchPolicy.Allow
        }
        //! [3]
        Container {
            leftPadding: 10
            rightPadding: 10
            bottomPadding: 10
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            layout: StackLayout {
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Blue
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }
                    id: altitudeLabel
                    text: "Altitude"
                }
                Slider {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }
                    id: altitudeSlider
                    horizontalAlignment: HorizontalAlignment.Center
                    value: 2
                    fromValue: 500
                    toValue: 5000
                    onImmediateValueChanged: {
                        mapview.setAltitude(Math.round(immediateValue))
                        altitudeLabel.setText("Altitude: " + Math.round(mapview.altitude))
                    }
                    onValueChanged: {
                    }
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Blue
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }
                    id: tiltLabel
                    text: "Tilt"
                }
                Slider {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }
                    id: tiltSlider
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: 0
                    toValue: 45
                    onImmediateValueChanged: {
                        mapview.setTilt(Math.round(immediateValue))
                        tiltLabel.setText("Tilt: " + mapview.tilt + "\u00B0")
                    }
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.Blue
                        fontWeight: FontWeight.Bold
                        fontFamily: "courier"
                    }
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .3
                    }
                    id: headingLabel
                    text: "Heading"
                }
                Slider {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: .7
                    }
                    id: headingSlider
                    horizontalAlignment: HorizontalAlignment.Center
                    fromValue: -180
                    toValue: 180
                    onImmediateValueChanged: {
                        mapview.setHeading(Math.round(immediateValue))
                        headingLabel.setText("Heading: " + mapview.heading + "\u00B0")
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
