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


// Custom container for the color cube and its selection overlay
Container {
    id: root
    
    // property for indicating the color png
    property string colorName: "red"
    // property for activating the selection png
    property bool selected: false

    preferredWidth: 115
    preferredHeight: 115

    layout: DockLayout {}
//! [0]
    // A standard ImageView used for the selection
    ImageView {
        layoutProperties: DockLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }
        visible: root.selected
        imageSource: "asset:///images/cube_selection.png"
    }

    // A standard ImageView used for the color cube
    ImageView {
        layoutProperties: DockLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }

        imageSource: "asset:///images/cube_" + root.colorName + ".png"
    }
//! [0]
}
