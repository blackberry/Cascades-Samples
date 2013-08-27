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
import bb.platform 1.0
import bb.system 1.0

Page {
    Container {
        layout: DockLayout{}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            //! [0]
            Button {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr("Notification")
                onClicked: {
                    notification.notify();
                }
            }
            //! [0]

            //! [1]
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 50

                text: qsTr("Notification Dialog")
                onClicked: {
                    notificationDialog.show();
                }
            }
            //! [1]

            //! [2]
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 50

                text: qsTr("Clear All Notifications")
                onClicked: {
                    notification.clearEffectsForAll();
                    notification.deleteFromInbox();
                }
            }
            //! [2]
        }

        //! [3]
        attachedObjects: [
            Notification {
                id: notification
                title: qsTr ("TITLE")
                body: qsTr ("BODY")
                soundUrl: _publicDir + "system_battery_low.wav"
            },
            NotificationDialog {
                id: notificationDialog
                title: qsTr ("TITLE")
                body: qsTr ("BODY")
                soundUrl: _publicDir + "system_battery_low.wav"
                buttons : [
                    SystemUiButton {
                        label: qsTr ("Okay")
                    }
                ]
                onFinished: {
                    console.log("Result: " + result);
                    console.log("Error: " + error);
                }
            }
        ]
        //! [3]
    }
}
