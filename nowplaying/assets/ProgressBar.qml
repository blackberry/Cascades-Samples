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

//! [0]
Container {
    id: root

    property int duration: 1
    property int position: 1

    bottomPadding: ui.du(2.2)

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    ProgressIndicator {
        verticalAlignment: VerticalAlignment.Center
        preferredWidth: ui.du(66.7)

        fromValue: 0
        toValue: root.duration
        value: root.position
    }

    Label {
        verticalAlignment: VerticalAlignment.Center

        property int minutes: Math.floor(root.position/1000/60)
        property int seconds: Math.floor(root.position/1000%60)

        text: qsTr("%1:%2").arg(minutes < 10 ? "0" + minutes : "" + minutes)
                            .arg(seconds < 10 ? "0" + seconds : "" + seconds)
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
            color: Color.Gray
        }
    }
}
//! [0]
