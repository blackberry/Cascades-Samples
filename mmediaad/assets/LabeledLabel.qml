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

// this is a custom control allowing for labeled Labels
Container {
    property alias tag: tag.text
    property alias text: label.text
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    bottomMargin: ui.du(2.2)
    Label {
        minWidth: ui.du(31.1)
        id: tag
        textStyle {
            base: tsd.style
        }
    }
    Label {
        id: label
    }
    attachedObjects: [
        TextStyleDefinition {
            id: tsd
            base: SystemDefaults.TextStyles.BodyText
            fontWeight: FontWeight.Bold
        }
    ]
}
