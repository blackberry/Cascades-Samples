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

NavigationPane {
    id: navigationPane

    onPopTransitionEnded: page.destroy()

    Page {
        actions: [
            //! [0]
            ActionItem {
                title: qsTr("Update profile")

                onTriggered: {
                    navigationPane.push(updateProfilePage.createObject())
                }
            }
            //! [0]
        ]
        BbmProfileScrollView {

            Container {
                layout: DockLayout {
                }

                ImageView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    imageSource: "asset:///images/background_blurred.png"
                }

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    topPadding: ui.du(2.2)
                    leftPadding: ui.du(2.2)
                    rightPadding: ui.du(2.2)

                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }

                        //! [1]
                        ImageView {
                            preferredHeight: ui.du(33.3)
                            preferredWidth: ui.du(33.3)

                            image: _profile.displayPicture
                        }

                        Button {
                            text: qsTr("Get Avatar")
                            onClicked: {
                                _profile.requestDisplayPicture()
                            }
                        }
                        //! [1]
                    }

                    Divider {
                    }

                    Container {
                        minHeight: ui.du(5.6)

                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }

                        //! [2]
                        ImageView {
                            verticalAlignment: VerticalAlignment.Center

                            imageSource: "images/busy.png"
                            visible: _profile.busy
                        }

                        Label {
                            layoutProperties: StackLayoutProperties {
                                spaceQuota: 1
                            }

                            text: _profile.displayName
                            textStyle {
                                color: Color.White
                                fontWeight: FontWeight.Bold
                            }
                        }
                        //! [2]
                    }

                    //! [3]
                    Field {
                        title: qsTr("status message")
                        value: _profile.statusMessage
                    }
                    //! [3]

                    Field {
                        title: qsTr("personal message")
                        value: _profile.personalMessage
                    }

                    Field {
                        title: qsTr("pp id")
                        value: _profile.ppid
                    }

                    Field {
                        title: qsTr("app version")
                        value: _profile.appVersion
                    }

                    Field {
                        title: qsTr("handle")
                        value: _profile.handle
                    }

                    Field {
                        title: qsTr("platform version")
                        value: _profile.platformVersion
                    }
                }
            }
        }
    }

    //! [4]
    attachedObjects: [
        ComponentDefinition {
            id: updateProfilePage
            source: "UpdateProfile.qml"
        }
    ]
    //! [4]
}
