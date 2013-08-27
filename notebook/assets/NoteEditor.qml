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
import com.example.bb10samples.pim.notebook 1.0

Page {
    id: root

    //! [0]
    onCreationCompleted: {
        if ( _noteBook.noteEditor.mode == NoteEditor.EditMode) {
            // Fill the editor fields after the UI has been created
            titleField.text = _noteBook.noteEditor.title
            descriptionField.text = _noteBook.noteEditor.description
            dueDateTimeField.value = _noteBook.noteEditor.dueDateTime
            completedField.checked = _noteBook.noteEditor.completed
        }
    }
    //! [0]

    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar

        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: (_noteBook.noteEditor.mode == NoteEditor.CreateMode ? qsTr ("Create" ) : qsTr ("Save"))

            onTriggered: {
                _noteBook.noteEditor.saveNote()

                navigationPane.pop()
            }
        }

        // The 'Cancel' action
        dismissAction: ActionItem {
            title: qsTr ("Cancel")

            onTriggered: navigationPane.pop()
        }
    }
    //! [1]

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
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: 30
            topPadding: 30
            rightPadding: 30

            //! [2]
            TextField {
                id: titleField

                hintText: qsTr ("Title")

                onTextChanging: _noteBook.noteEditor.title = text
            }
            //! [2]

            TextArea {
                id: descriptionField

                hintText: qsTr ("Description")

                onTextChanging: _noteBook.noteEditor.description = text
            }

            DateTimePicker {
                id: dueDateTimeField

                title: qsTr ("Due")

                onValueChanged: _noteBook.noteEditor.dueDateTime = value
            }

            CheckBox {
                id: completedField

                text: qsTr ("Completed")

                onCheckedChanged: _noteBook.noteEditor.completed = checked
            }
        }
    }
}
