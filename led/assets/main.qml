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
import bb.device 1.0

// Page which organizes the various visual components
Page {
    content: Container {

        background: Color.create("#AFC7C7")
        
        // A standard Label used for the application title
        Label {
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                fontWeight: FontWeight.Bold
                color: Color.Black
            }
            preferredHeight: 150
            text: "LED tester"
        }
        
        // Container used to organize the layout of the color cubes
        Container {
            layout: DockLayout { }

            preferredHeight: 250
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    rightPadding: 20
                    leftPadding: 20
                }
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Top
                }
//! [0]
                // Custom Cube container
                Cube {
                    id: redCube
                    // indicating the cubes color
                    colorName: "red"
                    // whether cube selection is enabled
                    selected: true
                    rightMargin: 10
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("red is down")
                            // reset all the selections
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Red
                        }
                    }
                }
                // Custom cube container
                Cube {
                    id: greenCube
                    // indicate the cubes color
                    colorName: "green"
                    rightMargin: 10
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("green is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Green
                        }
                    }
                }
                // Custom cube container
                Cube {
                    id: blueCube
                    // indicate the cubes color
                    colorName: "blue"
                    rightMargin: 10
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("blue is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Blue
                        }
                    }
                }
                // Custom cube container
                Cube {
                    id: yellowCube
                    // indicate the cubes color
                    colorName: "yellow"
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("yellow is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Yellow
                        }
                    }
                }
//! [0]
            }
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    rightPadding: 20
                    leftPadding: 20
                }
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Bottom
                }
                // Custom cube container
                Cube {
                    id: cyanCube
                    // indicate the cubes color
                    colorName: "cyan"
                    rightMargin: 10
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("cyan is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Cyan
                        }
                    }
                }
                // Custom cube container
                Cube {
                    id: magentaCube
                    // indicate the cubes color
                    colorName: "magenta"
                    rightMargin: 10
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("magenta is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.Magenta
                        }
                    }
                }
                // Custom cube container
                Cube {
                    id: whiteCube
                    // indicate the cubes color
                    colorName: "white"
                    onTouch: {
                        if (event.isDown ()) {
                            console.debug("white is down")
                            // reset previous selecitons
                            resetSelection ()
                            selected = true
                            // set the led color
                            ledID.color = LedColor.White
                        }
                    }
                }
            }
        }
        // Container grouping the Slider/Label element
        Container {
            layout: StackLayout {
                leftPadding: 20
                rightPadding: 20
            }
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
//! [1]
            // Slider which allows you to choose flash counts
            Slider {
                id: slider
                fromValue: 0
                toValue: 10
                value: 0
            }
//! [1]
            // A standard Label displaying the slider value
            Label {
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Top
                }
                text: {
                    var result = sliderCount( slider.toValue, slider.value);
                    if ( result == -1 ) {
                        return "∞";
                    } else {
                        return result;
                    }
                }
                textStyle.size: 52
                textStyle.color: Color.Black
            }
        }
        // Container grouping the Labels for displaying status and the
        // application control buttons
        Container {
            // A standard Label
            Label {
                id: active
                text: "Active: false"
            }
            // A standard Label
            Label {
                id: countDown
                text: "Remaining flash count: n/a"
            }
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
//! [2]
                // A standard Button to activate LED flash
                Button {
                    text: "Flash"
                    onClicked: {
                        if (ledID.count <= 0) {
                            ledID.flash ()
                        } else {
                            ledID.flash (ledID.count)
                        }
                    }
                }
                // A standard Button to cancel LED flash
                Button {
                    text: "Cancel"
                    onClicked: {
                        ledID.cancel ();
                    }
                }
//! [2]
            }
        }
        attachedObjects: [
//! [3]
            // Led component
            Led {
                id: ledID
                property int count: sliderCount( slider.toValue, slider.value)
                // change Label text on active state
                onActiveChanged: {
                    if (count == -1)
                        countDown.text = "Remaining flash count: ∞";
                    else
                        countDown.text = "Remaining flash count: " + count
                    active.text = "Active: " + value;
                }
                onRemainingFlashCountChanged: {
                    countDown.text = "Remaining flash count: " + value
                }
            }
//! [3]
        ]

    }
//! [4]
    // custom function to return -1 if max value is reached, and actual value otherwise
    function sliderCount (k, val) {
        var tmp = Math.floor (val);
        if (tmp == k) {
            return -1;
        } else {
            return tmp;
        }
    }
//! [4]
    // custom function to remove all cube selections
    function resetSelection() {
        redCube.selected = false
        greenCube.selected = false
        blueCube.selected = false
        yellowCube.selected = false
        cyanCube.selected = false
        magentaCube.selected = false
        whiteCube.selected = false
    }
}
