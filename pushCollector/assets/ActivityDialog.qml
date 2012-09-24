/*!
 * Copyright (c) 2012 Research In Motion Limited.
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

Dialog {
    property alias title: activityTitle.text
    property alias message: activityMessage.text

    Container {
        id: activityDialogContainer

        // The dialog does not automatically fill the entire screen like a Page does, so in order
        // for it to be possible to center the dialog on screen, the width and height has to be set.
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        preferredWidth: 768
        preferredHeight: 1280

        layout: DockLayout {}

        // The background is set to semi transparent to indicate that it is not possible to interact
        // with the screen behind the dialog.
        background: Color.create(0.0, 0.0, 0.0, 0.5)

        Container {
            preferredWidth: activityDialogContainer.preferredWidth - 40
            preferredHeight: activityDialogContainer.preferredHeight - 400

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                background: Color.White

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 10
                    bottomPadding: 10

                    background: Color.create("#CCCCCC")

                    Label {
                        id: activityTitle
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    topPadding: 10
                    bottomPadding: 10

                    ActivityIndicator {
                        id: registerIndicator

                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center

                        preferredWidth: 200
                        preferredHeight: 200
                    }

                    Label {
                        id: activityMessage
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
            }
        }
    }

    onOpened: {
        registerIndicator.start();
    }

    onClosed: {
        registerIndicator.stop();
    }
}
