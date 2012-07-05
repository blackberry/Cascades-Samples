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
import Utils 1.0

/**
 * The following import statement is needed for declaring internal Component elements.
 * Unfortunately the Component element is exported into the QtQuick namespace even though
 * it doesn't depend on QtQuick in any way.
 */
import QtQuick 1.0 as QtBase

TabbedPane {

    /**
     * The page that shows how to use the Loader element to load/unload external QML files at runtime.
     */
    Tab {
        title: "External"
        Page {
            content: Container {
                preferredWidth: 768

                layout: DockLayout {}

                ImageView {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    imageSource: "asset:///images/background.png"
                }

//! [0]
                Container {
                    id: pageContainer

                    property variant pageNames: ['', 'Page1.qml', 'Page2.qml', 'Page3.qml', 'Page4.qml', 'Page5.qml']

                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }

                    Loader {
                        id: pageLoader

                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }

                    Slider {
                        topMargin: 50

                        id: slider
                        fromValue: 0
                        toValue: 5
                        value: 0

                        // Update the source of the loader depending on the position of the slider
                        onValueChanging: pageLoader.source = pageContainer.pageNames[Math.round(value)]
                    }
                }
//! [0]
            }
        }
    }

    Tab {
        title: "Internal"
        /**
         * The page that shows how to use the Loader element to load/unload Component elements that are defined inline.
         */
        Page {
            // Declare some internal Components as attached objects
            // Note: The graphical representation of these components is not loaded yet!
//! [1]
            attachedObjects: [
                QtBase.Component {
                    id: redSquare
                    ColoredRectangle {
                        property string colorName: "red"
                        color: Color.Red
                        title: "Red"
                    }
                },
                QtBase.Component {
                    id: greenSquare
                    ColoredRectangle {
                        property string colorName: "green"
                        color: Color.Green
                        title: "Green"
                    }
                },
//! [1]
                QtBase.Component {
                    id: blueSquare
                    ColoredRectangle {
                        property string colorName: "blue"
                        color: Color.Blue
                        title: "Blue"
                    }
                },
                QtBase.Component {
                    id: yellowSquare
                    ColoredRectangle {
                        property string colorName: "yellow"
                        color: Color.Yellow
                        title: "Yellow"
                    }
                }
            ]

            content: Container {
                preferredWidth: 768

                layout: DockLayout {}

                ImageView {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                    }

                    imageSource: "asset:///images/background.png"
                }

                Container {
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }

                    Label {
                        text: "Touch the rectangle below"

                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }

//! [2]
                    Loader {
                        id: rectLoader

                        property int counter: 0

                        // Bind one of the internal components to 'sourceComponent' property depeding on the counter
                        sourceComponent:   counter == 0 ? redSquare
                                         : counter == 1 ? greenSquare
                                         : counter == 2 ? blueSquare
                                         : yellowSquare

                        // The Loader provides all the functionality of a standard Control e.g. the touch signal
                        onTouch: {
                            if (event.isDown()) {
                                // Increment counter and wrap if > 3
                                counter = ((counter + 1) % 4)
                            }
                        }

                        topMargin: 10
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
//! [2]

                    Label {
                        topMargin: 10

                        // We can select properties of the currently loaded element via the 'control' property of the Loader
                        text: "Selected color: " + (rectLoader.control ? rectLoader.control.colorName : "n/a")

                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                }
            }
        }
    }
}
