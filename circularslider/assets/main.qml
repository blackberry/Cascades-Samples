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
import custom.lib 1.0

Page {
    // The root container
    Container {
        topMargin: ui.du(14.4)
        layout: DockLayout {}
        background: Color.create ("#404040")

        // Display the current rotation of the slider handle.
        Label {
            horizontalAlignment: HorizontalAlignment.Center

            text: slider.value
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                fontWeight: FontWeight.Bold
            }
        }

        // Create the CircularSlider and lay it out just like any other control.
        CircularSlider {
            id: slider

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // Initialize the slider with 180
            value: 180

            // Capture the valueChanged signal and show it as debug output.
            onValueChanged: {
                console.debug(value);
            }
        }
    }
}
