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
import com.sample.payment 1.0

// Container creating a message box type of display
Container {
    property alias text: text_input.text

    layout: DockLayout {}

    // Container organizing the visual components
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        leftPadding: 10
        rightPadding: 10
        topPadding: 10
        bottomPadding: 10

        background: Color.Gray

        Container {
            leftPadding: 15
            rightPadding: 15
            topPadding: 15
            bottomPadding: 15

            background: Color.Black

            // A standard TextArea for displaying messages
            TextArea {
                id: text_input

                horizontalAlignment: HorizontalAlignment.Fill

                backgroundVisible: true
                editable: false
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.Red
                    textAlign: TextAlign.Center
                }
            }
        }
    }
}