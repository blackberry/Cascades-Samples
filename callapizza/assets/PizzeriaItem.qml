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
    property alias title: titleLabel.text
    property alias description: descriptionLabel.text

    leftPadding: ui.du(3.3)
    topPadding: ui.du(3.3)

    Label {
        id: titleLabel

        textStyle {
            base: SystemDefaults.TextStyles.TitleText
            color: Color.DarkGray
        }
    }

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        rightPadding: ui.du(8.9)

        Label {
            id: descriptionLabel

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.DarkGray
                textAlign: TextAlign.Right
            }
        }

        ImageView {
            imageSource: "asset:///images/phone_icon.png"
        }
    }

    ImageView {
        imageSource: "asset:///images/separator_line.png"
    }
}