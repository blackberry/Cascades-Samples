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

Container {
    id: container
    property alias label: labelPart.text
    property alias text: textPart.text
    property alias textStyle: textPart.textStyle
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Label {
        id: labelPart
        text: ""
        textStyle {
            base: SystemDefaults.TextStyles.SubtitleText
            fontWeight: FontWeight.Bold
            color: Color.DarkGray
        }
    }
    Label {
        id: textPart
        text: ""
        textStyle {
            base: SystemDefaults.TextStyles.SubtitleText
            color: Color.Gray
        }
    }
}
