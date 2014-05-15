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

import bb.cascades 1.3

Page {
    Container {

        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background_blurred.png"
        }
        BbmProfileBoxScrollView {

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                topPadding: ui.du(2.2)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                Label {
                    horizontalAlignment: HorizontalAlignment.Center

                    text: qsTr("New Profile Box")
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        color: Color.White
                        fontWeight: FontWeight.Bold
                    }
                }

                Divider {
                }

                Label {
                    text: qsTr("Profile Box Text")
                    textStyle.color: Color.White
                }

                //! [0]
                TextField {
                    id: profileBoxText
                    hintText: qsTr("Type profile box text here")
                }
                //! [0]

                Divider {
                }

                Label {
                    text: qsTr("Pick an icon from below")
                    textStyle.color: Color.White
                }

                //! [1]
                RadioGroup {
                    id: profileBoxIcon

                    Option {
                        imageSource: "images/apple.png"
                        value: 1
                        selected: true
                    }

                    Option {
                        imageSource: "images/pear.png"
                        value: 2
                    }

                    Option {
                        imageSource: "images/orange.png"
                        value: 3
                    }
                }
                //! [1]

                //! [2]
                Button {
                    horizontalAlignment: HorizontalAlignment.Right

                    text: qsTr("Save Profile Box")

                    onClicked: {
                        _profileBoxManager.addProfileBoxItem(profileBoxText.text, profileBoxIcon.selectedValue)
                        navigationPane.pop()
                    }
                }
                //! [2]
            }
        }
    }
}
