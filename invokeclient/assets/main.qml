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

Page {
    //! [0]
    actions: [
        ActionItem {
            title: qsTr ("Invoke (best-fit)")
            imageSource: "asset:///images/icon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _app.invoke()
            }
        },
        ActionItem {
            title: qsTr ("Query")
            imageSource: "asset:///images/query.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _app.query()
            }
        },
        InvokeActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            query {
                mimeType: "image/png"
                invokeActionId: "bb.action.OPEN"
            }
        },
        InvokeActionItem {
            title: qsTr ("OpenImage1")
            ActionBar.placement: ActionBarPlacement.OnBar
            query {
                mimeType: "image/png"
                invokeActionId: "bb.action.OPEN"
                invokeTargetId: "com.example.bb10samples.invocation.openimage1"
            }
        },
        InvokeActionItem {
            title: qsTr ("OpenImage2")
            ActionBar.placement: ActionBarPlacement.OnBar
            query {
                mimeType: "image/png"
                invokeActionId: "bb.action.OPEN"
                invokeTargetId: "com.example.bb10samples.invocation.openimage2"
            }
        }
    ]
    //! [0]

    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                topPadding: 50
                leftPadding: 50
                rightPadding: 50

                //! [1]
                DropDown {
                    horizontalAlignment: HorizontalAlignment.Fill

                    title: qsTr ("Invocation Type:")

                    Option {
                        selected: true
                        text: qsTr ("All")
                        description: qsTr ("All types of invocation targets.")
                        value: 0
                    }

                    Option {
                        text: qsTr ("Application")
                        description: qsTr ("Targets that launch in a new window.")
                        value: 1
                    }

                    Option {
                        text: qsTr ("Viewer")
                        description: qsTr ("Targets that launch embedded.")
                        value: 2
                    }

                    Option {
                        text: qsTr ("Service")
                        description: qsTr ("Targets that launch in background.")
                        value: 3
                    }

                    Option {
                        text: qsTr ("Card")
                        description: qsTr ("Targets that embeds as Card.")
                        value: 3
                    }

                    onSelectedValueChanged: {
                        _app.targetType = selectedValue
                    }
                }
                //! [1]

                Container {
                    horizontalAlignment: HorizontalAlignment.Center

                    layout: DockLayout {}

                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Top

                        imageSource: "asset:///images/nolight.png"
                    }

                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Top

                        imageSource: "asset:///images/light.png"

                        animations: [
                            FadeTransition {
                                id: fadeout
                                fromOpacity: 1
                                toOpacity: 0
                                duration: 300
                                onEnded: {
                                    fadein.play();
                                }
                            },
                            FadeTransition {
                                id: fadein
                                duration: fadeout.duration
                                fromOpacity: 0
                                toOpacity: 1
                                easingCurve: StockCurve.BounceInOut
                                onEnded: {
                                    fadeout.play();
                                }
                            }
                        ]
                        onCreationCompleted: {
                            fadeout.play();
                        }
                    }
                }

                //! [2]
                DropDown {
                    id: actionSelector

                    horizontalAlignment: HorizontalAlignment.Fill

                    title: qsTr ("Action:")

                    Option {
                        selected: true
                        text: qsTr ("All")
                        description: qsTr ("Valid for queries only.")
                        value: "__All"
                    }

                    Option {
                        selected: true
                        text: qsTr ("Menu Actions")
                        description: qsTr ("Valid for queries only.")
                        value: "__MenuActions"
                    }

                    Option {
                        selected: true
                        text: qsTr ("bb.action.OPEN")
                        description: qsTr ("A menu action for opening content.")
                        value: "bb.action.OPEN"
                    }

                    Option {
                        text: qsTr ("bb.action.SET")
                        description: qsTr ("A menu action for setting content as")
                        value: "bb.action.SET"
                    }

                    Option {
                        text: qsTr ("bb.action.SHARE")
                        description: qsTr ("A menu action for sharing content.")
                        value: "bb.action.SHARE"
                    }

                    Option {
                        text: qsTr ("Custom")
                        description: qsTr ("Specify a custom action.")
                        value: ""
                    }

                    onSelectedValueChanged: {
                        _app.action = selectedValue
                    }
                }
                //! [2]

                TextField {
                    visible: (actionSelector.selectedValue == "")
                    hintText: qsTr ("[Custom Action]")

                    onTextChanging: {
                        _app.action = text
                    }
                }

                //! [3]
                TextField {
                    hintText: qsTr ("[MIME Type]")
                    text: "image/png"

                    onTextChanging: {
                        _app.mimeType = text
                    }
                }
                //! [3]

                TextField {
                    hintText: qsTr ("[URI]")

                    onTextChanging: {
                        _app.uri = text
                    }
                }

                TextField {
                    hintText: qsTr ("[Data]")

                    text: qsTr ("Test data..")

                    onTextChanging: {
                        _app.data = text
                    }
                }

                TextField {
                    hintText: qsTr ("[Target ID - for Bound Invocation]")

                    onTextChanging: {
                        _app.target = text
                    }
                }

                Label {
                    text: _app.statusMessage
                }
            }
        }

        //! [4]
        attachedObjects: [
            QueryResultSheet {},
            ErrorDialog {}
        ]
        //! [4]
    }
}
