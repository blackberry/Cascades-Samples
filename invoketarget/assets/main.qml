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

NavigationPane {

    peekEnabled: true

    backButtonsVisible: _app.backButtonVisible

    Page {
        titleBar: TitleBar {
            title: _app.title
        }

        Container {
            layout: DockLayout {}

            // The background image
            ImageView {
                imageSource: "asset:///images/background.png"
            }

            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                scrollViewProperties.scrollMode: ScrollMode.Vertical

                Container {

                    topPadding: 50
                    leftPadding: 50
                    rightPadding: 50

                    //! [0]
                    LabelLabel {
                        label: qsTr ("startup mode")
                        text: _app.startupMode
                    }

                    LabelLabel {
                        label: qsTr ("source")
                        text: _app.source
                    }
                    //! [0]

                    LabelLabel {
                        label: qsTr ("target")
                        text: _app.target
                    }

                    LabelLabel {
                        label: qsTr ("action")
                        text: _app.action
                    }

                    LabelLabel {
                        label: qsTr ("mime type")
                        text: _app.mimeType
                    }

                    LabelLabel {
                        label: qsTr ("uri")
                        text: _app.uri
                    }

                    LabelLabel {
                        label: qsTr ("data")
                        text: _app.data
                    }

                    Divider {}

                    LabelLabel {
                        topMargin: 100

                        label: qsTr ("status")
                        text: _app.status
                    }

                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        topMargin: 50

                        text: qsTr("Done")
                        onClicked: _app.cardDone()
                    }
                }
            }
        }
    }
}
