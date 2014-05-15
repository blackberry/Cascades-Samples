/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

/**
*  This is a TabbedPane layout that allows you to switch 3 NavigationPanes (GET, POST, Settings)
*
*  You will learn how to:
*  -- Create a TabbedPane
*  -- Create Tabs
*  -- Add NavigationPane to a Tab
*  -- Use both DockLayout and StackLayout
*  -- Style text using textStyle property of a Label
*  -- Push a new layout on a NavigationPane by pressing a button
*  -- Use a RadioGroup to set a flag in a C++ object
**/
TabbedPane {

    // The tab for presenting GET actions
    Tab {
        title: qsTr("GET")
        imageSource: "asset:///images/get.png"

        NavigationPane {
            id: httpGetNavPane

            onPopTransitionEnded: page.destroy()

            Page {
                Container {
                    layout: DockLayout {}

                    // The background image
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill

                        imageSource: "asset:///images/background.png"
                    }

                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Top

                        background: Color.Black

                        Label {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("GET REQUESTS")

                            textStyle {
                                base: SystemDefaults.TextStyles.BigText;
                                color: Color.White
                                fontStyle: FontStyle.Italic
                                fontWeight: FontWeight.Bold
                                textAlign: TextAlign.Center
                            }
                        }
                    }

                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Center
                        //! [0]
                        Button {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("External IP")

                            onClicked: {
                                httpGetNavPane.push(ipInfoPageDefinition.createObject());
                            }


                            attachedObjects: ComponentDefinition {
                                id: ipInfoPageDefinition
                                source: "ipinfo.qml"
                            }
                        }
                        //! [0]
                        //! [1]
                        Button {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("Get Request Headers")

                            onClicked: {
                                httpGetNavPane.push(requestinfoPageDefinition.createObject());
                            }

                            attachedObjects: ComponentDefinition {
                                id: requestinfoPageDefinition
                                source: "requestinfo.qml"
                            }
                        }
                        //! [1]
                    }
                }
            }
        }
    }

    // The tab for presenting POST actions
    Tab {
        title: qsTr("POST")
        imageSource: "asset:///images/post.png"

        NavigationPane {
            id: httpPostNavPane

            onPopTransitionEnded: page.destroy()

            Page {
                Container {
                    layout: DockLayout {}

                    // The background image
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill

                        imageSource: "asset:///images/background.png"
                    }

                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Top

                        background: Color.Black

                        Label {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("POST REQUESTS")
                            textStyle {
                                base: SystemDefaults.TextStyles.BigText;
                                color: Color.White
                                fontStyle: FontStyle.Italic
                                fontWeight: FontWeight.Bold
                                textAlign: TextAlign.Center
                            }
                        }
                    }
                    //! [2]
                    Container {
                        verticalAlignment: VerticalAlignment.Center
                        leftPadding: ui.du(3.3)
                        rightPadding: ui.du(3.3)

                        TextArea {
                            id: postBody
                            preferredHeight: ui.du(38.9)

                            hintText: qsTr("Enter post body")
                        }

                        Button {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("Post!")

                            onClicked: {
                                _httpsample.postBody = postBody.text
                                httpPostNavPane.push(postPageDefinition.createObject());
                            }

                            attachedObjects: ComponentDefinition {
                                id: postPageDefinition
                                source: "post.qml"
                            }
                        }
                    }
                    //! [2]
                }
            }
        }
    }

    // The tab for configuring the application
    Tab {
        title: qsTr("Settings")

        NavigationPane {
            id: settingsGetNavPane

            Page {
                id: settingspage
                Container {
                    layout: DockLayout {}

                    // The background image
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill

                        imageSource: "asset:///images/background.png"
                    }

                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Top

                        background: Color.Black

                        Label {
                            horizontalAlignment: HorizontalAlignment.Center

                            text: qsTr("CHANGE SETTINGS")
                            textStyle {
                                base: SystemDefaults.TextStyles.BigText;
                                color: Color.White
                                fontStyle: FontStyle.Italic
                                fontWeight: FontWeight.Bold
                                textAlign: TextAlign.Center
                            }
                        }
                    }

                    Container {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center

                        preferredWidth: ui.du(55.6)
                        //! [3]
                        RadioGroup {
                            Option {
                                text: qsTr("HTTP (normal)")
                                selected: !_httpsample.useHttps
                            }

                            Option {
                                text: qsTr("HTTPS (secure)")
                                selected: _httpsample.useHttps
                            }

                            onSelectedIndexChanged: {
                                _httpsample.useHttps = (selectedIndex == 1)
                            }
                        }
                        //! [3]
                    }
                }
            }
        }
    }
}
