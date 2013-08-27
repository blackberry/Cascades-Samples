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
import Network.PostHttp 1.0
import "controls"
import my.library 1.0

/**
*  This is a simple Page that allows you to enter HTML form values and POST them to a newtwork resource
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
            leftPadding: 30

            Label {
                id: postResponseLabel
                visible: false

                text: qsTr("Response:")
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText;
                    color: Color.White
                }
            }

            TextArea {
                id: postResponseBody
                visible: false
                editable: false
                backgroundVisible: false

                textStyle {
                    base: SystemDefaults.TextStyles.BodyText;
                    color: Color.White
                }
            }
        }

        NetworkActivity {
            id: progressIndicator

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            title: qsTr("Posting...")
        }

        attachedObjects: [
            QTimer {
                id: timer
                interval: 1000
                onTimeout: {
                    // One second after page loads make the http post request
                    netpost.post(_httpsample.postBody);
                }
            },
            PostHttp {
                id : netpost
                onComplete :{
                    progressIndicator.active = false;
                    progressIndicator.visible = false;

                    postResponseBody.text = info;
                    postResponseBody.visible = true;
                    postResponseLabel.visible = true;

                    timer.stop();
                }
            }
        ]

        onCreationCompleted: {
            progressIndicator.active = true;
            timer.start();
        }
        //! [0]
    }
}
