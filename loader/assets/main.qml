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
        title: qsTr ("External")
        Page {
            Container {
                layout: DockLayout {}

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background.png"
                }

                //! [0]
                Container {
                    id: pageContainer

                    property variant pageNames: ['', 'Page1.qml', 'Page2.qml', 'Page3.qml', 'Page4.qml', 'Page5.qml']

                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    Loader {
                        horizontalAlignment: HorizontalAlignment.Center

                        // Update the source of the loader depending on the position of the slider
                        source: pageContainer.pageNames[Math.round(slider.immediateValue)]
                    }

                    Slider {
                        id: slider

                        topMargin: ui.du(5.6)

                        fromValue: 0
                        toValue: 5
                        value: 0
                    }
                }
                //! [0]
            }
        }
    }

    Tab {
        title: qsTr ("Internal")
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
                        property string colorName: qsTr ("red")
                        color: Color.Red
                        title: qsTr ("Red")
                    }
                },
                QtBase.Component {
                    id: greenSquare
                    ColoredRectangle {
                        property string colorName: qsTr ("green")
                        color: Color.Green
                        title: qsTr ("Green")
                    }
                },
            //! [1]
                QtBase.Component {
                    id: blueSquare
                    ColoredRectangle {
                        property string colorName: qsTr ("blue")
                        color: Color.Blue
                        title: qsTr ("Blue")
                    }
                },
                QtBase.Component {
                    id: yellowSquare
                    ColoredRectangle {
                        property string colorName: qsTr ("yellow")
                        color: Color.Yellow
                        title: qsTr ("Yellow")
                    }
                }
            ]

            Container {
                layout: DockLayout {}

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background.png"
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    Label {
                        horizontalAlignment: HorizontalAlignment.Center

                        text: qsTr ("Touch the rectangle below")
                        textStyle.color: Color.White
                    }

                    //! [2]
                    Loader {
                        id: rectLoader

                        property int counter: 0

                        horizontalAlignment: HorizontalAlignment.Center
                        topMargin: ui.du(1.1)

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
                    }
                    //! [2]

                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        topMargin: ui.du(1.1)

                        // We can select properties of the currently loaded element via the 'control' property of the Loader
                        text: qsTr ("Selected color: %1").arg(rectLoader.control ? rectLoader.control.colorName : qsTr("n/a"))
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            color: Color.White
                        }
                    }
                }
            }
        }
    }
}
