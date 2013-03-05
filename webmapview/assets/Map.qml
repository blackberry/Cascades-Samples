/* Copyright (c) 2013 Research In Motion Limited.
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

Container {
    id: root

    layout: DockLayout {
    }

    //! [0]
    property variant center: [ "-1", "-1" ]
    property variant centerPixel: [ "-1", "-1" ]
    //! [0]

    //! [1]
    function setZoom(zoom) {
        var script = "setZoom(" + zoom + ")"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function zoomIn() {
        var script = "zoomIn()"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function zoomOut() {
        var script = "zoomOut()"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function setCenter(lat, lon) {
        var script = "setCenter(" + lat + "," + lon + ")"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function setMapType(mapType) {
        var script = "setMapTypeId(" + mapType + ")"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function createPushPin(lat, lon, title) {
        var script = "createPushPin(" + lat + "," + lon + "," + "\"" + title + "\"" + "," + "\"local:///assets/images/pin.png\"" + ")"
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    function removeAllPins() {
        var script = "removeAllPins()";
        qwvMapView.evaluateJavaScript(script, JavaScriptWorld.Normal);
    }
    //! [1]

    ScrollView {
        id: scrollView
        scrollViewProperties {
            scrollMode: ScrollMode.None
            pinchToZoomEnabled: true
        }
        //! [2]
        WebView {
            id: qwvMapView

            html: _webMaps.pageContent
        //! [2]

            onMinContentScaleChanged: {
                scrollView.scrollViewProperties.minContentScale = minContentScale;
            }
            onMaxContentScaleChanged: {
                scrollView.scrollViewProperties.maxContentScale = maxContentScale;
            }

            //! [3]
            onMessageReceived: {
                if (message.data.indexOf("centerChanged") >= 0) {
                    var data = message.data.substring(message.data.indexOf(":") + 1);
                    var values = data.split(",");
                    center = [
                        values[0],
                        values[1]
                    ];
                    centerPixel = [
                        values[2],
                        values[3]
                    ];
                    label.text = qsTr("centerChanged:\nWorld: %1, %2\nPixel: %3, %4")
                                     .arg(center[0].substr(0, 9)).arg(center[1].substr(0, 9))
                                     .arg(Math.round(x)).arg(Math.round(y));
                    labelPosition.positionX = centerPixel[0];
                    labelPosition.positionY = centerPixel[1];
                } else if (message.data.indexOf("clicked") >= 0) {
                    var data = message.data.substring(message.data.indexOf(":") + 1);
                    var values = data.split(",");
                    var lat = values[0];
                    var lon = values[1];
                    var x = values[2];
                    var y = values[3];
                    label.text = qsTr("clicked:\nWorld: %1, %2\nPixel: %3, %4")
                                     .arg(lat.substr(0, 9)).arg(lon.substr(0, 9))
                                     .arg(Math.round(x)).arg(Math.round(y));
                    labelPosition.positionX = x;
                    labelPosition.positionY = y;
                } else if (message.data.indexOf("markerClicked") >= 0) {
                    var data = message.data.substring(message.data.indexOf(":") + 1);
                    var values = data.split(",");
                    var lat = values[0];
                    var lon = values[1];
                    var x = values[2];
                    var y = values[3];
                    label.text = qsTr("markerClicked:\nWorld: %1, %2\nPixel: %3, %4")
                                     .arg(lat.substr(0, 9)).arg(lon.substr(0, 9))
                                     .arg(Math.round(x)).arg(Math.round(y));
                    labelPosition.positionX = x;
                    labelPosition.positionY = y;
                }
            }
            //! [3]

            //! [4]
            gestureHandlers: [
                PinchHandler {
                    property variant initial
                    onPinchUpdated: {
                        if (! initial) {
                            initial = event.distance;
                        }
                        if ((event.distance - initial) / 50 > 1) {
                            initial = event.distance;
                            root.zoomIn();
                        } else if ((event.distance - initial) / 50 < -1) {
                            initial = event.distance;
                            root.zoomOut();
                        }
                    }
                }
            ]
            //! [4]
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        layout: AbsoluteLayout {
        }

        touchPropagationMode: TouchPropagationMode.None
        overlapTouchPolicy: OverlapTouchPolicy.Allow

        //! [5]
        Label {
            id: label

            layoutProperties: AbsoluteLayoutProperties {
                id: labelPosition
            }

            multiline: true
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                color: Color.Blue
                fontWeight: FontWeight.Bold
                fontFamily: "courier"
            }
        }
        //! [5]
    }
}
