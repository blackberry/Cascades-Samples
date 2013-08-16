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

import bb.cascades 1.2

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        // The description label
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            text: qsTr("Send a tag via NFC to this device to see the senders MAC address")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
                textAlign: TextAlign.Center
            }

            multiline: true
        }

        //! [0]
        // The label that shows the MAC address
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            text: qsTr("MAC Address\n%1").arg(_macAddressHandler.macAddress)
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.White
                textAlign: TextAlign.Center
            }
            multiline: true
        }
        //! [0]
    }
}
