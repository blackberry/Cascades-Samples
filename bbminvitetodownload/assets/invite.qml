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

Page {
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Press the button to invite your BlackBerry Messenger contacts to download this application from App World.")
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                    textAlign: TextAlign.Center
                }
                multiline: true
            }

            //! [0]
            Button {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Invite to Download")
                onClicked: {
                    _inviteToDownload.sendInvite();
                }
            }
            //! [0]

            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Note that this sample is not in App World so the final invite button will be disabled.")
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                    textAlign: TextAlign.Center
                }
                multiline: true
            }
        }
    }
}
