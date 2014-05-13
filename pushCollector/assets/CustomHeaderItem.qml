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

// A custom header item for the list view, which groups the item by date
Container {
    property alias text: label.text

    topPadding: ui.du(1.1)
    leftPadding: ui.du(2.2)
    bottomPadding: ui.du(1.7)
    preferredWidth: ui.du(85.3)

    layout: DockLayout {}

    background: Color.create("#E6E6E6");

    Label {
        id: label

        verticalAlignment: VerticalAlignment.Center

        textStyle {
            base: SystemDefaults.TextStyles.BodyText
        }
    }
}
