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

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: ui.du(77.8)

            //! [0]
            // The input field for the NDEF message payload
            TextField {

                inputMode: TextFieldInputMode.Text
                hintText: qsTr("Enter some text")
                onTextChanging: {
                    _nfcSender.payload = text
                }
            }

            // Guide message to display to the user
            Label {
                text: qsTr("Type some text into the text field and then tap an NFC tag or device to share content")

                multiline: true
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                    textAlign: TextAlign.Center
                }
            }
            //! [0]
        }
    }
}
