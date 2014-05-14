/*!
 * Copyright (c) 2012, 2013  BlackBerry Limited.
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

// A custom checkbox that reacts on touch events on the label
Container {
    property alias text: label.text
    property alias checked: checkbox.checked

    horizontalAlignment: HorizontalAlignment.Fill
    leftPadding: ui.du(2.2)
    rightPadding: leftPadding

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    Label {
        id: label

        horizontalAlignment: HorizontalAlignment.Left

        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        onTouch: {
            if (event.touchType == TouchType.Up) {
                checkbox.setChecked(! checkbox.checked);
            }
        }
    }

    CheckBox {
        id: checkbox

        verticalAlignment: VerticalAlignment.Center
    }
}
