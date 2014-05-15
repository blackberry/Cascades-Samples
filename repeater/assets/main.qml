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
import Components 1.0

TabbedPane {
    Tab {
        title: qsTr ("Simple")
        Page {
            CustomPage {
                //! [0]
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    leftPadding: ui.du(3.3)
                    topPadding: ui.du(3.3)
                    rightPadding: ui.du(3.3)

                    Repeater {
                        // Use a simple number (N) as model -> the delegate will be repeated N times
                        model: 5

                        Label {
                            text: qsTr ("Hello World")
                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: Color.White
                            }
                        }
                    }
                }
                //! [0]
            }
        }
    }

    Tab {
        title: qsTr ("Index")
        Page {
            CustomPage {
                //! [1]
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    leftPadding: ui.du(3.3)
                    topPadding: ui.du(3.3)
                    rightPadding: ui.du(3.3)

                    Repeater {
                        // Use a simple number (N) as model -> the delegate will be repeated N times
                        model: 5

                        Label {
                            // The current index can be accessed via the 'index' context property
                            text: qsTr ("Hello World (%1)").arg(index)
                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: Color.White
                            }
                        }
                    }
                }
                //! [1]
            }
        }
    }

    Tab {
        title: qsTr ("Dynamic")
        Page {

            CustomPage {
                //! [2]
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    leftPadding: ui.du(3.3)
                    topPadding: ui.du(3.3)
                    rightPadding: ui.du(3.3)

                    Slider {
                        id: slider
                        fromValue: 1
                        toValue: 5
                        value: 1
                    }

                    Repeater {
                        // The number, that is used as model, can be a property binding as well
                        model: slider.immediateValue

                        Label {
                            text: qsTr ("Hello World (%1)").arg(index)
                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: Color.White
                            }
                        }
                    }
                }
                //! [2]
            }
        }
    }

    Tab {
        title: qsTr ("Array")
        Page {
            CustomPage {
                //! [3]
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    leftPadding: ui.du(3.3)
                    topPadding: ui.du(3.3)
                    rightPadding: ui.du(3.3)

                    Repeater {
                        // Use an array of arbitrary values as model
                        model: [Color.Red, Color.Yellow, Color.Green, Color.Blue, Color.White]

                        Label {
                            text: qsTr ("Hello World (%1)").arg(index)

                            // The current value of the model can be accessed via the 'modelData' context property
                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: modelData
                            }
                        }
                    }
                }
                //! [3]
            }
        }
    }

    Tab {
        title: qsTr ("SQL Model")
        Page {
            CustomPage {
                //! [4]
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    leftPadding: ui.du(3.3)
                    topPadding: ui.du(3.3)
                    rightPadding: ui.du(3.3)

                    ScrollView {
                        scrollViewProperties {
                            scrollMode: ScrollMode.Vertical
                        }

                        Container {
                            Repeater {
                                // Use a DataModel object as model
                                model: _sqlModel
                                Container {
                                    topPadding: 30

                                    // The values of the current model entry can be accessed by their key names (here: title, firstname, surname)
                                    Label {
                                        text: title
                                        textStyle.base: SystemDefaults.TextStyles.TitleText
                                        textStyle.color: Color.White
                                    }

                                    Label {
                                        text: qsTr ("[%1 %2]").arg(firstname).arg(surname)
                                        textStyle.base: SystemDefaults.TextStyles.BodyText
                                        textStyle.color: Color.White
                                    }

                                    Divider {}
                                }
                            }
                        }
                    }
                }
                //! [4]
            }
        }
    }
}
