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

                topPadding: 30
                leftPadding: 30
                rightPadding: 30
                //! [0]
                ImageView {
                    horizontalAlignment: HorizontalAlignment.Center

                    preferredHeight: 500
                    preferredWidth: 700
                    minWidth: 700
                    minHeight: 500

                    image: _scratchpad.image
                }
                //! [0]
                //! [1]
                // Container with images to select
                Container {
                    topMargin: 50

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
                    topMargin: 50

                    hintText: qsTr("Enter # rows (1 or more) of replicants")
                    inputMode: TextFieldInputMode.NumbersAndPunctuation

                    onTextChanged: {
                        _scratchpad.rows = text
                    }
                }

                TextField {
                    hintText: qsTr("Enter # columns (1 or more) of replicants")
                    inputMode: TextFieldInputMode.NumbersAndPunctuation

                    onTextChanged: {
                        _scratchpad.columns = text
                    }
                }
                //! [2]
            }
        }
    }
}
