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
import Components 1.0

TabbedPane {
    Tab {
        title: "Simple"
        Page {
            content: CustomPage {
//! [0]
                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    layout: StackLayout {
                        leftPadding: 30
                        topPadding: 30
                        rightPadding: 30
                    }

                    Repeater {
                        // Use a simple number (N) as model -> the delegate will be repeated N times
                        model: 5

                        Label {
                            text: "Hello World"
                            textStyle.color: Color.White
                        }
                    }
                }
//! [0]
            }
        }
    }

    Tab {
        title: "Index"
        Page {
            content: CustomPage {
//! [1]
                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    layout: StackLayout {
                        leftPadding: 30
                        topPadding: 30
                        rightPadding: 30
                    }

                    Repeater {
                        // Use a simple number (N) as model -> the delegate will be repeated N times
                        model: 5

                        Label {
                            // The current index can be accessed via the 'index' context property
                            text: "Hello World (" + index + ")"
                            textStyle.color: Color.White
                        }
                    }
                }
//! [1]
            }
        }
    }

    Tab {
        title: "Dynamic"
        Page {

            content: CustomPage {
//! [2]
                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    layout: StackLayout {
                        leftPadding: 30
                        topPadding: 30
                        rightPadding: 30
                    }

                    Slider {
                        id: slider
                        fromValue: 1
                        toValue: 5
                        value: 1
                    }

                    Repeater {
                        // The number, that is used as model, can be a property binding as well
                        model: slider.value

                        Label {
                            text: "Hello World (" + index + ")"
                            textStyle.color: Color.White
                        }
                    }
                }
//! [2]
            }
        }
    }

    Tab {
        title: "Array"
        Page {
            content: CustomPage {
//! [3]
                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    layout: StackLayout {
                        leftPadding: 30
                        topPadding: 30
                        rightPadding: 30
                    }

                    Repeater {
                        // Use an array of arbitrary values as model
                        model: [Color.Red, Color.Yellow, Color.Green, Color.Blue, Color.White]

                        Label {
                            text: "Hello World (" + index + ")"

                            // The current value of the model can be accessed via the 'modelData' context property
                            textStyle.color: modelData
                        }
                    }
                }
//! [3]
            }
        }
    }

    Tab {
        title: "SQL Model"
        Page {
            content: CustomPage {
//! [4]
                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    layout: StackLayout {
                        leftPadding: 30
                        topPadding: 30
                        rightPadding: 30
                    }

                    preferredWidth: 768

                    ScrollView {
                        scrollViewProperties {
                            scrollMode: ScrollMode.Both
                        }
                        Container {
                            Repeater {
                                // Use a DataModel object as model
                                model: _sqlModel
                                Container {
                                    layout: StackLayout {
                                        topPadding: 30
                                    }

                                    // The values of the current model entry can be accessed by their key names (here: title, firstname, surname)
                                    Label {
                                        text: title
                                        textStyle.base: SystemDefaults.TextStyles.TitleText
                                        textStyle.color: Color.White
                                    }
                                    Label {
                                        text: "[" + firstname + " " + surname + "]"
                                        textStyle.color: Color.White
                                    }
                                    Divider { }
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