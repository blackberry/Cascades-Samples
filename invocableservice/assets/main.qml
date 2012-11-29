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

//! [0]
Page {
    titleBar: TitleBar {
        title: qsTr("Invoke Service Sample")
    }

    Container {
        topPadding: 20

        Label {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("\"Tap buttons to invoke service\"")
            textStyle {
                fontStyle: FontStyle.Italic
                fontSize: FontSize.XLarge
            }
        }

        CheckBox {
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 700
            topMargin: 50

            text: qsTr("5 sec launch delay")

            onCheckedChanged: _app.handleCheckedChange(checked)
        }

        ImageButton {
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 100

            defaultImageSource: "asset:///images/sound.png"

            onClicked: _app.sendSoundInvokeRequest()
        }

        ImageButton {
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 60

            defaultImageSource: "asset:///images/led.png"

            onClicked: _app.sendLEDInvokeRequest()
        }

        ImageButton {
            horizontalAlignment: HorizontalAlignment.Center
            topMargin: 60

            defaultImageSource: "asset:///images/vibrate.png"

            onClicked: _app.sendVibrateInvokeRequest()
        }
        Button {
            horizontalAlignment: HorizontalAlignment.Center
            text: "kill Service"
            onClicked: _app.killServiceRequest();
        }
    }
}
//! [0]
