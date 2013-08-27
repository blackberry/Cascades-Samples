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
import Network.ExternalIP 1.0
import "controls"
import my.library 1.0

/**
*  This is a Page that displays your external IP address
*
*  You will learn how to:
*  -- Import C++ objects
*  -- Use attachedObjects
*  -- Use a QTimer for timing operations
**/
Page {
    Container {

        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }
        //! [0]
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Label {
                id: ipaddress
                visible: false

                text: qsTr("Retrieving IP address")

                textStyle {
                    base: SystemDefaults.TextStyles.BigText;
                    color: Color.White
                    fontWeight: FontWeight.Bold
                }
            }
        }

        NetworkActivity {
            id: progressIndicator

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            title: qsTr("Retrieving IP address")
        }

        attachedObjects: [
            QTimer {
                id: timer
                interval: 1000
                onTimeout: {
                    // One second after page loads make the http get request
                    netip.getIP();
                }
            },
            ExternalIP {
                id : netip
                onComplete :{
                    // Stop progress indicator and hide it
                    progressIndicator.active = false;
                    progressIndicator.visible = false;

                    // Display external IP address
                    ipaddress.text = info;
                    ipaddress.visible = true;
                    timer.stop();
                }
            }
        ]

        onCreationCompleted: {
            // Start progress indicator and timer
            progressIndicator.active = true;
            timer.start();
        }
        //! [0]
    }
}
