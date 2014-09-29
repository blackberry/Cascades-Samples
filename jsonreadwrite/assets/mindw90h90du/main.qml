/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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
    // topmost container for entire page
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            imageSource: "asset:///images/background.png"
        }

        Container {
            leftPadding: ui.du(2.0)
            topPadding: ui.du(3.3)
            rightPadding: ui.du(3.3)
            bottomPadding: ui.du(3.3)

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            // left-side button container
            Container {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1 // ie. 1/3 of width of page
                }

                Button {
                    text: qsTr ("Load JSON")

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    onClicked: {
                        _app.loadOriginalJson ()
                    }
                }

                Button {
                    text: qsTr ("JSON to Qt")

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    enabled: _app.state > 0

                    onClicked: {
                        _app.convertJsonToQt ()
                    }
                }

                Button {
                    text: qsTr ("Qt to JSON")

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    enabled: _app.state > 1

                    onClicked: {
                        _app.convertQtToJson ()
                    }
                }

                Button {
                    text: qsTr ("Write & Reload")

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    enabled: _app.state > 2

                    onClicked: {
                        _app.writeToJsonFileAndReload ()
                    }
                }
            }

            // right-side content container
            Container {
                leftMargin: ui.du(2.2)
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2 // ie. 2/3 of width of page
                }

                // text areas container
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 5 // ie. large % of height
                    }

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    // left-hand JSON text area container
                    Container {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1 // ie. 1/2 of width
                        }
                        leftMargin: ui.du(2.0)

                        // left-hand-side text area
                        Label {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr ("JSON Data")

                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: Color.White
                                textAlign: TextAlign.Center
                            }
                        }

                        TextArea {
                            id: jsonTextArea
                            text: _app.jsonData
                            hintText: ""
                            textStyle.base: SystemDefaults.TextStyles.SmallText
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1
                            }
                            onTextChanged: {
                                _app.updateJsonDataFromQml (text);
                            }
                        }
                    }

                    // right-hand output text area container
                    Container {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1 // ie. 1/2 of width
                        }

                        Label {
                            id: rhsTextLabel
                            horizontalAlignment: HorizontalAlignment.Center
                            text: _app.rhsTitle

                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                color: Color.White
                                textAlign: TextAlign.Center
                            }
                        }

                        Container {
                            background: Color.create("#66000000")

                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1
                            }

                            layout: DockLayout {}

                            TextArea {
                                id: rhsTextArea

                                text: _app.rhsText
                                editable: false
                                textStyle {
                                    base: SystemDefaults.TextStyles.SmallText
                                    color: Color.DarkYellow
                                }
                            }
                        }
                    }
                }

                // bottom results text line
                Container {
                    topMargin: ui.du(2.2)

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1 // ie. small % of height
                    }

                    layout: DockLayout {}

                    background: Color.create("#66000000")

                    TextArea {
                        id: resultsText
                        text: _app.result
                        enabled: false
                        backgroundVisible: false

                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.Red
                        }
                    }
                }
            }
        }
    }
}
