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

import bb.cascades 1.0

//! [0]
Container {
    id: root

    property alias title : titleLabel.text
    property string field
    property variant selectedPlace

    visible: (selectedPlace ? (selectedPlace[field] != "") : false)

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    // The title label
    Label {
        id: titleLabel

        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        opacity: 0.7

        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
            fontStyle: FontStyle.Italic
        }
    }

    // The field value label
    Label {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        text: selectedPlace ? selectedPlace[field] : ""

        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
        }

        multiline: true
    }
}
//! [0]
