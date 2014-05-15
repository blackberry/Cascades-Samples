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

            imageSource: "asset:///images/background.png"
        }
        ScratchpadScrollView {

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                topPadding: ui.du(3.3)
                leftPadding: ui.du(3.3)
                rightPadding: ui.du(3.3)
                //! [0]
                ImageView {
                    horizontalAlignment: HorizontalAlignment.Center

                    preferredHeight: ui.du(55.6)
                    preferredWidth: ui.du(77.8)
                    minWidth: ui.du(77.8)
                    minHeight: ui.du(55.6)

                    image: _scratchpad.image
                }
                //! [0]
                //! [1]
                // Container with images to select
                Container {
                    topMargin: ui.du(5.6)

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    Button {
                        imageSource: "asset:///images/Blue_20Nose_20Thumb.png"

                        onClicked: {
                            _scratchpad.object = "images/Blue_20Nose_20Thumb.png"
                        }
                    }

                    Button {
                        imageSource: "asset:///images/Zeppelin_Thumb.png"

                        onClicked: {
                            _scratchpad.object = "images/Zeppelin_Thumb.png"
                        }
                    }

                    Button {
                        imageSource: "asset:///images/warning.png"

                        onClicked: {
                            _scratchpad.object = "images/warning.png"
                        }
                    }

                    Button {
                        text: "\u25CB" // unicode char for white circle

                        onClicked: {
                            _scratchpad.object = "circle"
                        }
                    }

                    Button {
                        text: "\u25A1" // unicode char for white square

                        onClicked: {
                            _scratchpad.object = "square"
                        }
                    }
                }
                //! [1]
                //! [2]
                TextField {
                    id: rowText
                    topMargin: ui.du(5.6)

                    hintText: qsTr("Enter # rows (1 or more) of replicants")
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    
                    input {
                        submitKey: SubmitKey.EnterKey
                        onSubmitted: {
                            _scratchpad.rows = rowText.text
                        }
                    }
                }

                TextField {
                    id: columnText
                    hintText: qsTr("Enter # columns (1 or more) of replicants")
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    input {
                        submitKey: SubmitKey.EnterKey
                        onSubmitted: {
                            _scratchpad.columns = columnText.text
                        }
                    }
                }
                //! [2]
            }
        }
    }
}
