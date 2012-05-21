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
    content: Container {
        layout: DockLayout {
        }
        ImageView {
            imageSource: "asset:///images/Background.png"
        }
        Container {
            layout: StackLayout {
                topPadding: 180
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            // Top Container with a RadioButtonGroup and title.
            Container {
                preferredWidth: 545
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Label {
                    bottomMargin: 0
                    text: "DIVERT ALL POWER TO:"
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        fontWeight: FontWeight.Bold
                        color: Color.create ("#ffFDfDfD")
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Left
                    }
                }
                RadioGroup {
                    id: radioGroup1
                    objectName: "radioGroup1"
                    Option {
                        id: radioGroupOption0
                        objectName: "radioGroupOption0"
                        text: "HYPERDRIVE"

                        // Here we call the getValue for objectname,
                        // this calls the C++ functions that connects to the QSettings.
                        selected: _starshipApp.getValueFor (objectName, "false")
                        onSelectedChanged: {
                            _starshipApp.saveValueFor (radioGroupOption0.objectName, radioGroupOption0.selected)
                        }
                    }
                    Option {
                        id: radioGroupOption2
                        objectName: "radioGroupOption2"
                        text: "SAUNA"

                        // Here we call the getValue for objectname,
                        // this calls the C++ functions that connects to the QSettings.
                        selected: _starshipApp.getValueFor (objectName, "true")
                        onSelectedChanged: {
                            _starshipApp.saveValueFor (radioGroupOption2.objectName, radioGroupOption2.selected)
                        }
                    }
                }
            }
            
            // Component with warp core image and slider with title and tooltip.
            WarpDrive {
                preferredWidth: 786
                layout: StackLayout {
                    leftPadding: 110
                    rightPadding: leftPadding
                }
            }
            
            // Bottom Container with custom CheckBox and ToggleButton.
            Container {
                preferredWidth: 545
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
                CheckBoxWithText {
                    objName: "uranusScanner"
                    text: "URANUS SCANNER"
                }
                Label {
                    topMargin: 15
                    text: "Gravity"
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        fontWeight: FontWeight.Bold
                        color: Color.create ("#ffFDfDfD")
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                
                ToggleButton {
                    id: gravity
                    checked: _starshipApp.getValueFor (objectName, "false")
                    objectName: "gravity"
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    
                    onCheckedChanged: {
                        _starshipApp.saveValueFor (gravity.objectName, checked)
                    }
                }
            }
        }
    }
}
