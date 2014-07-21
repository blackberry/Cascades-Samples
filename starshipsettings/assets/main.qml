/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
    // A custom ScrollView is used here, depending on the resolution
    // the UI will have the ability to scroll or not.
    StarshipScrollView {
        Container {
            topPadding: ui.du(3)
            bottomPadding: ui.du(3)
            horizontalAlignment: HorizontalAlignment.Fill

            layout: DockLayout {
            }

            ImageView {
                imageSource: "asset:///images/Background.png"
                horizontalAlignment: HorizontalAlignment.Center
            }

            // Content Container
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                preferredWidth: ui.du(54.5)

                // Set Container to not clip its content to avoid clipping of the tool tip.
                clipContentToBounds: false

                // Top Container with a RadioButtonGroup and title
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
                    // Button 1
                    Option {
                        text: qsTr("HYPERDRIVE") + Retranslate.onLanguageChanged
                    }

                    // Button 2
                    Option {
                        text: qsTr("SAUNA") + Retranslate.onLanguageChanged
                    }

                    onSelectedIndexChanged: {
                        appSettings.powerDivert = selectedIndex;
                    }
                    onCreationCompleted: {
                        selectedIndex = appSettings.powerDivert
                    }
                }

                // This is our custom component with warp-core image, slider with title, and tooltip
                // which is based from Container in WarpDrive.qml.
                WarpDrive {
                }

                CheckBox {
                    topMargin: ui.du(3)
                    text: qsTr("URANUS SCANNER") + Retranslate.onLanguageChanged
                    checked: appSettings.uranuscanner

                    onCheckedChanged: {
                        appSettings.uranuscanner = checked
                    }
                }

                Label {
                    id: gravityLabel
                    horizontalAlignment: HorizontalAlignment.Center
                    text: qsTr("GRAVITY") + Retranslate.onLanguageChanged

                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                    }
                }

                ToggleButton {
                    id: gravity
                    checked: appSettings.gravity
                    objectName: "gravity"
                    horizontalAlignment: HorizontalAlignment.Center

                    onCheckedChanged: {
                        appSettings.gravity = checked;
                    }

                    accessibility {
                        labelledBy: gravityLabel
                    }
                }
            }
        }
    }
}
