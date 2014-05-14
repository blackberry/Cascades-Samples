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
import bb.cascades.pickers 1.0

Page {

    //! [0]
    actions: [
        ActionItem {
            title: qsTr("Invoke (best-fit)")
            imageSource: "asset:///images/invoke.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _app.invoke()
            }
        },
        ActionItem {
            title: qsTr("Query")
            imageSource: "asset:///images/query.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                _app.query()
            }
        },
        // This invoke brings up the system UI for platform actions such as SHARE, OPEN and SET
        ActionItem {
            title: qsTr("Platform Invoke")
            imageSource: "asset:///images/menuinvoke.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                _app.platformInvoke()
            }
        }
    ]
    //! [0]
    Container {
        layout: DockLayout {
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
                topPadding: ui.du(2.2)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)
                bottomPadding: ui.du(2.2)
                Header {
                    title: "Target Type"
                    subtitle: "For queries only"
                }
                //! [1]
                DropDown {
                    horizontalAlignment: HorizontalAlignment.Fill
                    title: qsTr("Invocation Type:")
                    Option {
                        selected: true
                        text: qsTr("All")
                        description: qsTr("All types of invocation targets.")
                        value: 0
                    }
                    Option {
                        text: qsTr("Application")
                        description: qsTr("Targets that launch in a new window.")
                        value: 1
                    }
                    Option {
                        text: qsTr("Service")
                        description: qsTr("Targets that launch in background.")
                        value: 3
                    }
                    Option {
                        text: qsTr("Card")
                        description: qsTr("Targets that embeds as Card.")
                        value: 3
                    }
                    onSelectedValueChanged: {
                        _app.targetType = selectedValue
                    }
                    onCreationCompleted: {
                        _app.targetType = selectedValue
                    }
                }
                //! [1]
                Header {
                    title: "Action"
                }
                //! [2]
                DropDown {
                    id: actionSelector
                    horizontalAlignment: HorizontalAlignment.Fill
                    title: qsTr("Action:")
                    Option {
                        text: qsTr("All")
                        description: qsTr("Valid for queries only.")
                        value: "__All"
                    }
                    Option {
                        text: qsTr("Menu Actions")
                        description: qsTr("Valid for queries only.")
                        value: "__MenuActions"
                    }
                    Option {
                        text: qsTr("bb.action.OPEN")
                        description: qsTr("A menu action for opening content.")
                        value: "bb.action.OPEN"
                    }
                    Option {
                        text: qsTr("bb.action.SET")
                        description: qsTr("A menu action for setting content as")
                        value: "bb.action.SET"
                    }
                    Option {
                        selected: true
                        text: qsTr("bb.action.SHARE")
                        description: qsTr("A menu action for sharing content.")
                        value: "bb.action.SHARE"
                    }
                    Option {
                        text: qsTr("Custom")
                        description: qsTr("Specify a custom action.")
                        value: ""
                    }
                    onSelectedValueChanged: {
                        _app.action = selectedValue
                    }
                    onCreationCompleted: {
                        _app.action = selectedValue
                    }
                }
                //! [2]
                TextField {
                    visible: (actionSelector.selectedValue == "")
                    hintText: qsTr("E.g. com.example.BEBOLD")
                    inputMode: TextFieldInputMode.Url
                    onTextChanging: {
                        _app.action = text
                    }
                    onCreationCompleted: {
                        _app.action = text
                    }
                }
                Header {
                    title: "MIME Type"
                }
                //! [3]
                TextField {
                    hintText: qsTr("E.g. image/png")
                    text: "text/plain"
                    inputMode: TextFieldInputMode.Url
                    onTextChanging: {
                        _app.mimeType = text
                    }
                    onCreationCompleted: {
                        _app.mimeType = text
                    }
                }
                //! [3]
                Header {
                    title: "URI"
                }
                TextField {
                    id: uriField
                    hintText: qsTr("Enter URI or double tap to pick a file")
                    inputMode: TextFieldInputMode.Url
                    onTextChanging: {
                        _app.uri = text
                    }
                    onCreationCompleted: {
                        _app.uri = text
                    }
                    gestureHandlers: [
                        DoubleTapHandler {
                            onDoubleTapped: {
                                filePicker.open();
                            }
                        }
                    ]
                    attachedObjects: [
                        FilePicker {
                            id: filePicker
                            type: FileType.Picture
                            title: "Select Image"
                            mode: FilePickerMode.Picker
                            viewMode: FilePickerViewMode.GridView
                            directories: [
                                "/accounts/1000/shared"
                            ]
                            onFileSelected: {
                                var path = selectedFiles[0];
                                uriField.text = "file://" + path;
                            }
                        }
                    ]
                }
                Header {
                    title: "Data"
                }
                TextField {
                    hintText: qsTr("E.g. anything!")
                    text: qsTr("Test data..")
                    inputMode: TextFieldInputMode.Url
                    onTextChanging: {
                        _app.data = text
                    }
                    onCreationCompleted: {
                        _app.data = text
                    }
                }
                Header {
                    title: "Target ID"
                }
                TextField {
                    hintText: qsTr("E.g. com.example.app")
                    inputMode: TextFieldInputMode.Url
                    onTextChanging: {
                        _app.target = text
                    }
                    onCreationCompleted: {
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
            QueryResultSheet {
            }
        ]
        //! [4]
    }
}
