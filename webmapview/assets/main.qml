/* Copyright (c) 2013 BlackBerry Limited.
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
import com.example.webmapview 1.0

Page {
    actions: [
        //! [0]
        ActionItem {
            title: _webMaps.viewModeTitle
            imageSource: "asset:///images/map.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _webMaps.nextViewMode()
                map.setMapType(_webMaps.viewMode);
            }
        },
        //! [0]
        ActionItem {
            title: qsTr("Waterloo")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                map.setCenter(43.468245, -80.519603);
            }
        },
        //! [1]
        ActionItem {
            title: qsTr("Manhattan")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                map.setCenter(40.790907, -73.965334);
            }
        },
        ActionItem {
            title: qsTr("Drop Pin")
            imageSource: "asset:///images/pin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                map.createPushPin(map.center[0], map.center[1], qsTr("I am a Pin"));
            }
        },
        ActionItem {
            title: qsTr("Clear Pins")
            imageSource: "asset:///images/clearpin.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                map.removeAllPins();
            }
        }
        //! [1]
    ]
    Container {
        layout: DockLayout {
        }
        //! [2]
        Map {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Fill
            id: map
        }
        //! [2]
        //! [3]
        DropDown {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            title: qsTr("Map Provider")

            Option {
                text: qsTr("Google Maps")
                imageSource: "asset:///images/googlemaps.png"
                value: WebMaps.GoogleMaps
                selected: true
            }
            Option {
                text: qsTr("Open Layers")
                imageSource: "asset:///images/openlayers.png"
                value: WebMaps.OpenLayers
            }
            Option {
                text: qsTr("Bing Maps")
                imageSource: "asset:///images/bingmaps.png"
                value: WebMaps.BingMaps
            }

            onSelectedValueChanged: _webMaps.currentProvider = selectedValue
        }
        //! [3]
    }
    onCreationCompleted: {
        map.setCenter(43.468245, -80.519603);
    }
}
