/*!
 * Copyright (c) 2012, 2013  BlackBerry Limited.
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

Dialog {
    property alias title: activityTitle.text
    property alias message: activityMessage.text

    Container {
        id: activityDialogContainer

        // The dialog does not automatically fill the entire screen like a Page does, so in order
        // for it to be possible to center the dialog on screen, the width and height has to be set.
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        preferredWidth: ui.du(85.3)
        preferredHeight: ui.du(142.2)

        layout: DockLayout {}

        // The background is set to semi transparent to indicate that it is not possible to interact
        // with the screen behind the dialog.
        background: Color.create(0.0, 0.0, 0.0, 0.8)

        Container {
            preferredWidth: activityDialogContainer.preferredWidth - 60
            preferredHeight: activityDialogContainer.preferredHeight - 200

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                background: Color.White

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: ui.du(2.2)
                    bottomPadding: ui.du(2.2)
                    leftPadding: ui.du(2.2)

                    background: Color.create("#087CC1")

                    Label {
                        id: activityTitle
                        textStyle{
                            color: Color.White
                            fontSize: FontSize.Large
                        }
                    }
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    topPadding: ui.du(1.1)
                    bottomPadding: ui.du(2.2)

                    ActivityIndicator {
                        id: registerIndicator

                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center

                        preferredWidth: ui.du(22.2)
                        preferredHeight: ui.du(22.2)
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
