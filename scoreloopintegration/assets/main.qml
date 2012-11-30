/*
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

Page {
    Container {
        topPadding: 20
        bottomPadding: 10
        rightPadding: 10

        layout: DockLayout {}

        background: Color.DarkGray

        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            text: qsTr("Scoreloop Sample Cascades")
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.White
            }
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Label {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 50

                text: qsTr("Your username is")
                textStyle {
                    color: Color.White
                }
            }

            //! [0]
            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: _scoreloop.userName
                textStyle {
                    color: Color.White
                }
            }
            //! [0]

            //! [1]
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 100

                text: qsTr("Reload")

                onClicked: {
                    console.log("scoreloop: " + _scoreloop);
                    console.log("scoreloop.userName: " + _scoreloop.userName);
                    _scoreloop.load();
                }
            }
            //! [1]
        }

        ImageView {
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Bottom

            imageSource: "asset:///images/logo.png"
        }
    }
}
