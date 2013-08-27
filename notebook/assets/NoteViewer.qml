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

import bb.cascades 1.0

Page {
    id: root

    titleBar: TitleBar {
        title: qsTr ("Note Details")
    }

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

            leftPadding: 30
            topPadding: 100
            rightPadding: 30

            //! [0]
            ViewerField {
                horizontalAlignment: HorizontalAlignment.Fill
                title: qsTr ("title")
                value: _noteBook.noteViewer.title
            }

            ViewerField {
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 50

                title: qsTr ("description")
                value: _noteBook.noteViewer.description
            }
            //! [0]

            ViewerField {
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 50

                title: qsTr ("due date")
                value: _noteBook.noteViewer.dueDateTime
            }

            ViewerField {
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 50

                title: qsTr ("status")
                value: _noteBook.noteViewer.status
            }
        }
    }

    //! [1]
    actions: [
        ActionItem {
            title: qsTr ("Edit")
            imageSource: "asset:///images/action_editnote.png"

            onTriggered: {
                _noteBook.editNote()
                navigationPane.push(noteEditor.createObject())
            }
        },
        DeleteActionItem {
            onTriggered: {
                _noteBook.deleteNote()

                navigationPane.pop()
            }
        }
    ]
    //! [1]

    //! [2]
    attachedObjects: [
        ComponentDefinition {
            id: noteEditor
            source: "NoteEditor.qml"
        }
    ]
    //! [2]
}
