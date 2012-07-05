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

// Page laying out the visual components
Page {
    content:
        Container {
            // Container for the application header title
            Container {
                background: Color.Black
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    spaceQuota: 1
                }
                // A standard Label for the title
                Label {
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    text: qsTr("Battery Charging State ")
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.White
                    }
                }
            }
            Container {
                topMargin: 10
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 8
                }
                Container {
                    layout: StackLayout {
                        leftPadding: 5
                        rightPadding: 5    
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        spaceQuota: 4
                    }
                    background: Color.create("#0A5A40")
                    // A standard RadioGroup holding the different battery states
                    // each option has the correspongin image url for display
                    ScrollView {
                        scrollViewProperties {
                            scrollMode: ScrollMode.Vertical
                        }
//! [0]
                        RadioGroup {
                            id: group1
                            selectedIndex: battery.chargingState
                            Option {
                                id: option0
                                text: qsTr("Unknown")
                                description: qsTr("Somethings up with Battery")
                                value: "asset:///images/batt_status_unknown.png"
                            }
                            Option {
                                id: option1
                                text: qsTr("Not Charging")
                                description: qsTr("Plugged in, just no charge")
                                value: "asset:///images/batt_status_full.png"
                            }
                            Option {
                                id: option2
                                text: qsTr("Charging")
                                description: qsTr("Plugged in")
                                value: "asset:///images/batt_status_charge.png"
                            }
                            Option {
                                id: option3
                                text: qsTr("Discharging")
                                description: qsTr("Unplugged and discharging")
                                value: "asset:///images/batt_status_discharge.png"
                            }
                            Option {
                                id: option4
                                text: qsTr("Full")
                                description: qsTr("Plugged in enough to be not discharging")
                                value: "asset:///images/batt_status_full.png"
                            }
                        }
//! [0]
                    }
                }
                Container {
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center 
                        spaceQuota: 1  
                    }
                    // A standard ImageView for displaying the current battery state image
//! [1]
                    ImageView {
                        id: batt_image
                        imageSource: group1.selectedIndex == RadioGroup.SelectedIndexNone ? "" : group1.selectedValue()
                        layoutProperties: DockLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                        }
                    }
//! [1]   
                }
            }
            // A standard ProgressIndicator for displaying the battery charge level
//! [2]
            ProgressIndicator {
                id: batteryLevel
                preferredWidth: 1024
                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Bottom
                    horizontalAlignment: HorizontalAlignment.Center;
                }
                fromValue: 0
                toValue: 100
                value: battery.level
            }
//! [2]
        }
}