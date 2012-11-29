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
import bb.cascades.advertisement 1.0

// TabbedPane laying out the visual components

TabbedPane {
    showTabsOnActionBar: true
    Tab {
        // describes the size of banner used
        title: qsTr("320x50")
        Page {
            id: tab1
            Container {
                layout: DockLayout {}

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background.png"
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    // define tab content here
                    Label {
                        text: qsTr("320x50 banner")
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
//! [0]
                    // this component is used for displaying banner Ad's
                    Banner {
                        // zone id is used to identify your application and to track Ad performance
                        // metrics by the Advertising Service
                        zoneId: 117145
                        refreshRate: 60
                        preferredWidth: 320
                        preferredHeight: 50
                        transitionsEnabled: true
                        // Place holder used when there is no connection to the Advertising Service
                        placeHolderURL: "asset:///images/placeholder_728x90.png"
                        backgroundColor: Color.Green
                        borderColor: Color.Gray
                        borderWidth: 2
                        horizontalAlignment: HorizontalAlignment.Center
                    }
//! [0]
                    Weather {
                    }
                }
            }
        }
    }
//! [1]
    Tab {
        // describes the size of banner used
        title: qsTr("300x50")
        Page {
            id: tab2
            Container {
                layout: DockLayout {}

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background.png"
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    // define tab content here
                    Label {
                        text: qsTr("300x50 banner")
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    // this component is used for displaying banner Ad's
                    Banner {
                        // zone id is used to identify your application and to track Ad performance
                        // metrics by the Advertising Service
                        zoneId: 117145
                        refreshRate: 60
                        preferredWidth: 300
                        preferredHeight: 50
                        transitionsEnabled: true
                        // Place holder used when there is no connection to the Advertising Service
                        placeHolderURL: "asset:///images/placeholder_728x90.png"
                        backgroundColor: Color.Green
                        borderColor: Color.Blue
                        borderWidth: 2
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Weather {
                    }
                }
            }
        }
    }
//! [1]
    Tab {
        // describes the size of banner used
        title: qsTr("320x48")
        Page {
            id: tab3
            Container {
                layout: DockLayout {}

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background.png"
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    // define tab content here
                    Label {
                        text: qsTr("320x48 banner")
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    // this component is used for displaying banner Ad's
                    Banner {
                        // zone id is used to identify your application and to track Ad performance
                        // metrics by the Advertising Service
//! [2]
                        zoneId: 117145
                        refreshRate: 60
                        preferredWidth: 320
                        preferredHeight: 48
                        transitionsEnabled: true
                        // Place holder used when there is no connection to the Advertising Service
                        placeHolderURL: "asset:///images/placeholder_728x90.png"
//! [2]
                        backgroundColor: Color.Green
                        borderColor: Color.Gray
                        borderWidth: 2
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Weather {
                    }
                }
            }
        }
    }
}
