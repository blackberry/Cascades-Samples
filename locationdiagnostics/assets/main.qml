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
 */import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    onPopTransitionEnded: {
        page.session.stopUpdates();        
        page.destroy();
    }
    Page {
        //! [0]
        actions: [
            ActionItem {
                title: qsTr("Start Updates")
                imageSource: "asset:///images/track.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    var page = locationPage.createObject()
                    var session = _locationDiagnostics.createLocationSession(true, page)                    
                    page.session = session
                    navigationPane.push(page)
                }
            },
            ActionItem {
                title: qsTr("Request Update")
                imageSource: "asset:///images/pin.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    var page = locationPage.createObject()
                    var session = _locationDiagnostics.createLocationSession(false, page)                    
                    page.session = session;
                    navigationPane.push(page)
                }
            }
        ]
        //! [0]

        //! [1]
        attachedObjects: [
            ComponentDefinition {
                id: locationPage
                source: "LocationPage.qml"
            }
        ]
        //! [1]
        Container {
            layout: DockLayout {
            }

            // The background image
            ImageView {
                imageSource: "asset:///images/background.png"
            }
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 50
                    leftPadding: 50
                    rightPadding: 50                   
                    DropDown {
                        title: qsTr("Positioning Method")
                        Option {
                            text: qsTr("All")
                            description: qsTr("GPS, MS-Based, MS-Assisted, Cell or Wifi")
                            value: "all"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("Non-Satellite")
                            description: qsTr("Cell or WiFi")
                            value: "nonSatellite"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("Satellite")
                            description: qsTr("GPS, MS-Based, MS-Assisted")
                            value: "satellite"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("GPS")
                            description: qsTr("GPS Only")
                            value: "gps"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("Cellular")
                            description: qsTr("Cellular only")
                            value: "cellular"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("WiFi")
                            description: qsTr("WiFi only")
                            value: "wifi"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("MS-Based")
                            description: qsTr("MS-Based only")
                            value: "msb"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        Option {
                            text: qsTr("MS-Assisted")
                            description: qsTr("MS-Assisted only")
                            value: "msa"
                            selected: (_locationDiagnostics.positionMethod == value)
                        }
                        onSelectedValueChanged: {
                            _locationDiagnostics.positionMethod = selectedValue
                        }
                    }

                    //! [2]
                    DropDown {
                        id: assistanceModeSelector
                        title: qsTr("Assistance")
                        Option {
                            text: qsTr("None")
                            description: qsTr("No carrier assistance")
                            value: "none"
                            selected: (_locationDiagnostics.assistanceMode == value)
                        }
                        Option {
                            text: qsTr("PDE")
                            description: qsTr("PDE Server")
                            value: "pde"
                            selected: (_locationDiagnostics.assistanceMode == value)
                        }
                        Option {
                            text: qsTr("SLP")
                            description: qsTr("SLP Server")
                            value: "slp"
                            selected: (_locationDiagnostics.assistanceMode == value)
                        }
                        Option {
                            text: qsTr("App ID/Password")
                            description: qsTr("Assistance requiring ID/Password")
                            value: "password"
                            selected: (_locationDiagnostics.assistanceMode == value)
                        }
                        onSelectedValueChanged: {
                            _locationDiagnostics.assistanceMode = selectedValue
                        }
                    }
                    //! [2]

                    //! [3]
                    TextField {
                        hintText: qsTr("PDE URL")
                        visible: (assistanceModeSelector.selectedValue == "pde")
                        inputMode: TextFieldInputMode.Url
                        onTextChanging: _locationDiagnostics.pdeUrl = text
                    }
                    //! [3]
                    TextField {
                        hintText: qsTr("SUPL URL")
                        visible: (assistanceModeSelector.selectedValue == "slp")
                        inputMode: TextFieldInputMode.Url
                        onTextChanging: _locationDiagnostics.slpUrl = text
                    }
                    TextField {
                        hintText: qsTr("App ID")
                        visible: (assistanceModeSelector.selectedValue == "password")
                        onTextChanging: _locationDiagnostics.appId = text
                    }
                    TextField {
                        hintText: qsTr("Password")
                        visible: (assistanceModeSelector.selectedValue == "password")
                        inputMode: TextFieldInputMode.Password
                        onTextChanging: _locationDiagnostics.password = text
                    }
                    Divider {
                    }
                    WarpDrive {
                        frequency: _locationDiagnostics.frequency
                        onFrequencyChanged: _locationDiagnostics.frequency = frequency
                    }
                    ToggleLabelButton {
                        horizontalAlignment: HorizontalAlignment.Fill
                        topMargin: 10
                        text: qsTr("Sound")
                        checked: _locationDiagnostics.useSound
                        onCheckedChanged: _locationDiagnostics.useSound = checked
                    }
                    ToggleLabelButton {
                        horizontalAlignment: HorizontalAlignment.Fill
                        topMargin: 10
                        text: qsTr("Background Mode")
                        checked: _locationDiagnostics.backgroundMode
                        onCheckedChanged: _locationDiagnostics.backgroundMode = checked
                    }
                }
            }
        }
    }
}
