/* Copyright (c) 2013 BlackBerry Limited.
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
        //Todo: fill me with QML
        layout: DockLayout {
        }
        id: root
        // Container presenting the service status
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            // Screen title
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Headless UI"
                textStyle.fontSize: FontSize.Large
                textStyle.fontWeight: FontWeight.Bold
            }
            //! [0]
            // Service status representation
            ColoredRectangle {
                topMargin: ui.du(11.1)
                id: service
                title: _app.isServiceRunning() ? "Headless Process Running" : "N/A"
                color: _app.isServiceRunning() ? Color.Green : Color.Red
            }
            //! [0]
        }
        // Container for communicating what the flash count was set to
        // by the headless service
        //! [1]
        Container {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            Container {

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "FlashCount: "
                }
                Label {
                    id: flashCount
                    text: _app.flashCount()
                    textStyle.fontWeight: FontWeight.Bold
                }
            }
            // Container for cummunicating the remaining flash count
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Label {
                    text: "RemainingCount: "
                }
                Label {
                    id: remainingFlashCount
                    textStyle.fontWeight: FontWeight.Bold
                    text: _app.remainingFlashCount
                }
            }
        }
        //! [1]
		//! [2]
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            Button {
                text: "Restart"
                onClicked: {
                    _app.resetLED();
                }
            }
        }
		//! [2]
    }
}
