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
*/import bb.cascades 1.0

import bb.cascades 1.0

Dialog {
    id: root

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center

            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr ("An Error Occurred")
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                }
            }

            //! [0]
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 100

                text: _app.errorMessage
                multiline: true
            }
            //! [0]

            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 100

                text: qsTr ("OK")

                onClicked: {
                    _app.clearError()
                    root.close()
                }
            }
        }
    }

    //! [1]
    function handleErrorMessage()
    {
        if (_app.errorMessage != "")
            root.open()
    }

    onCreationCompleted: _app.errorMessageChanged.connect(handleErrorMessage)
    //! [1]
}
