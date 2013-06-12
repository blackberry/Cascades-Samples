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
    // A custom ScrollView is used here, depending on the resolution
    // the UI will have the ability to scroll or not.
    StarshipScrollView{
        Container {
            layout: DockLayout {
            }
            topPadding: 30
            bottomPadding: 30
            ImageView {
                imageSource: "asset:///images/Background.amd"
                horizontalAlignment: HorizontalAlignment.Center
            }

            // Content Container
            Container {
                verticalAlignment: VerticalAlignment.Bottom
                // Top Container with a RadioButtonGroup and title
                Container {
                    preferredWidth: 545
                    horizontalAlignment: HorizontalAlignment.Center

                    Label {
                        bottomMargin: 0
                        horizontalAlignment: HorizontalAlignment.Left
                        text: qsTr("DIVERT ALL POWER TO:") + Retranslate.onLanguageChanged

                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            fontWeight: FontWeight.Bold
                        }
                    }

                    RadioGroup {
                        id: radioGroup1
                        objectName: "radioGroup1"

                        // Button 1
                        Option {
                            id: radioGroupOption0
                            objectName: "radioGroupOption0"
                            text: qsTr("HYPERDRIVE") + Retranslate.onLanguageChanged

                            // Call our C++ getValueFor() function for objectName, which connects to the QSettings.
                            selected: _starshipApp.getValueFor(objectName, "false") == "true" ? true : false;

                            onSelectedChanged: {
                                _starshipApp.saveValueFor(radioGroupOption0.objectName, radioGroupOption0.selected)
                            }
                        }

                        // Button 2
                        Option {
                            id: radioGroupOption2
                            objectName: "radioGroupOption2"
                            text: qsTr("SAUNA") + Retranslate.onLanguageChanged

                            // Call our C++ getValueFor() function for objectName, which connects to the QSettings.
                            selected: _starshipApp.getValueFor(objectName, "true") == "true" ? true : false;

                            onSelectedChanged: {
                                _starshipApp.saveValueFor(radioGroupOption2.objectName, radioGroupOption2.selected)
                            }
                        }
                    } // RadioGroup
                } // Top Container

                // This is our custom component with warp-core image, slider with title, and tooltip
                // which is based from Container in WarpDrive.qml.
                WarpDrive {
                    leftPadding: 110
                    rightPadding: leftPadding
                }

                // Bottom Container with custom CheckBox and ToggleButton
                Container {
                    preferredWidth: 545
                    horizontalAlignment: HorizontalAlignment.Center
                    topPadding: 30
                    CheckBox {
                        id: uranuscanner
                        text: qsTr("URANUS SCANNER") + Retranslate.onLanguageChanged
                        objectName: "uranuscanner"
                        checked: _starshipApp.getValueFor(objectName, "yes") == "true" ? true : false;
                        onCheckedChanged: {
                            _starshipApp.saveValueFor(uranuscanner.objectName, checked)
                        }
                    }

                    Label {

                        horizontalAlignment: HorizontalAlignment.Center
                        text: qsTr("GRAVITY") + Retranslate.onLanguageChanged

                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                        }
                    }

                    ToggleButton {
                        id: gravity
                        checked: _starshipApp.getValueFor(objectName, "false") == "true" ? true : false;
                        objectName: "gravity"
                        horizontalAlignment: HorizontalAlignment.Center

                        onCheckedChanged: {
                            _starshipApp.saveValueFor(gravity.objectName, checked)
                        }
                    }
                }
            } // Bottom Container
        } // Content Container
    } //ScrollView
}// Page
