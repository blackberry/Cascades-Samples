/*
 * Copyright (c) 2014 BlackBerry Limited.
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
import bb.cascades 1.4

// Custom view representing an abstraction of a classic belt button.
Container {
    id: root
    // Buttons initial text
    property string text: "button"
    
    // Buttons initial color
    property string color: "#EFEFEF"
    
    Button {
        id: button
        maxWidth: ui.du(25)
        minWidth: ui.du(25)
        text: root.text
        color: Color.create(root.color)
        
        // Shortcuts representing each button on the belt. These
        // allow us to intercept their press events.
        shortcuts: [
            // The Send button (phone call initiation)
            DeviceShortcut {
                type: DeviceShortcuts.SendTap
                onTriggered: {
                    if(text.indexOf("call") >= 0) {
                        clearButton()
                    }
                }
            },
            // The Menu button
            DeviceShortcut {
                type: DeviceShortcuts.MenuTap
                onTriggered: {
                    if(text.indexOf("menu") >= 0) {
                        clearButton()
                    }
                }
            },
            // The Back button
            DeviceShortcut {
                type: DeviceShortcuts.BackTap
                onTriggered: {
                    if(text.indexOf("back") >= 0) {
                        clearButton()
                    }
                }
            },
            // The End button (end call)
            DeviceShortcut {
                type: DeviceShortcuts.EndTap
                onTriggered: {
                    if(text.indexOf("end") >= 0) {
                        clearButton()
                    }
                }
            }
        ]

        // Envent handler for tracking user interaction with
        // the belt trackpad.
        eventHandlers: [
            TrackpadHandler {

                onTrackpad: {

                    switch (event.trackpadEventType) {
                        // User has started to trackpad interaction
                        case TrackpadEventType.Begin:
                            {
                                console.log("TrackpadEventType.Begin");
                                break;
                            }
                        // Trackpad continuing to be swiped (moving finger over trackpad)
                        case TrackpadEventType.Move:
                            {
                                console.log("TrackpadEventType.Move:  x:" + event.deltaX + " y:" + event.deltaY);
                                break;
                            }
                        // User has ended trackpad interaction
                        case TrackpadEventType.End:
                            {
                                console.log("TrackpadEventType.End");
                                break;
                            }
                        // User has pressed the trackpad
                        case TrackpadEventType.Press:
                            {
                                console.log("TrackpadEventType.Press");
                                break;
                            }
                        // User released the trackpad
                        case TrackpadEventType.Release:
                            {
                                console.log("TrackpadEventType.Release");
                                if(button.text == "trackpad") {
                                    clearButton()
                                }
                                break;
                            }
                        // Interaction has been canceled
                        case TrackpadEventType.Cancel:
                            {
                                console.log("TrackpadEventType.Cancel");
                                break;
                            }
                    }
                }
            }
        ]
    }
    
    // Clear the button of any text, and reset it to default color
    function clearButton() {
        button.text = ""
        button.color = Color.create("#EFEFEF")
        // Check if all the buttons have been cleared to declare a win
        main.checkForWin()
    }
}

