/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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
        //! [0]
        function pushPane()
        {
            navigationPane.push(viewTypes.selectedValue.createObject())
        }

        onCreationCompleted: _artifactline.artifactsLoaded.connect(pushPane)
        //! [0]

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

                topPadding: ui.du(2.2)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)
                bottomPadding: ui.du(2.2)

                layout: DockLayout {}

                Container {
                    id: screenInfo
                    verticalAlignment: VerticalAlignment.Top

                    Label {
                        text: qsTr("Enter a artifact name: ")
                        textStyle {
                            color: Color.Gray
                        }
                    }

                    //! [1]
                    TextField {
                        id: screenName
                        text: "Guice"
                    }

                    Button {
                        horizontalAlignment: HorizontalAlignment.Center

                        enabled: !_artifactline.active

                        text: qsTr("Artifacts")
                        onClicked: {
                            _artifactline.requestArtifact(screenName.text);
                        }
                    }
                    //! [1]
                }

                //! [2]
                Label {
                    verticalAlignment: VerticalAlignment.Center

                    visible: _artifactline.error

                    multiline: true

                    text: _artifactline.errorMessage
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText;
                        color: Color.Gray
                    }
                }
                //! [2]

                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom

                    Label {
                        horizontalAlignment: HorizontalAlignment.Center

                        text: qsTr("Select a ListView type for\ndisplaying the results feed")
                        textStyle {
                            color: Color.Gray
                            textAlign: TextAlign.Center
                        }
                        multiline: true
                    }

                    //! [3]
                    SegmentedControl {
                        id: viewTypes

                        Option {
                            id: basicView
                            text: qsTr("Standard")
                            value: standardViewPage
                            selected: true
                        }

                        Option {
                            id: richView
                            text: qsTr("Custom")
                            value: customViewPage
                        }
                    }
                    //! [3]
                }
            }
        }

        //! [4]
        attachedObjects: [
            ComponentDefinition {
                id: standardViewPage
                source: "StandardTimelineView.qml"
            },
            ComponentDefinition {
                id: customViewPage
                source: "CustomTimelineView.qml"
            }
        ]
        //! [4]
    }
}
