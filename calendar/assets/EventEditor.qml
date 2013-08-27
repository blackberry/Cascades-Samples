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
import com.example.bb10samples.pim.calendar 1.0

Page {
    id: root

    //! [0]
    onCreationCompleted: {
        if (_calendar.eventEditor.mode == EventEditor.EditMode) {
            subjectField.text = _calendar.eventEditor.subject
            locationField.text = _calendar.eventEditor.location
            startTimeField.value = _calendar.eventEditor.startTime
            endTimeField.value = _calendar.eventEditor.endTime
        }

        _calendar.eventEditor.initializeFolderDropDown(folderField)
    }
    //! [0]

    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar

        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: (_calendar.eventEditor.mode == EventEditor.CreateMode ? qsTr ("Create" ) : qsTr ("Save"))

            onTriggered: {
                _calendar.eventEditor.saveEvent()
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
                id: subjectField

                hintText: qsTr ("Subject")

                onTextChanging: _calendar.eventEditor.subject = text
            }
            //! [2]

            TextField {
                id: locationField

                hintText: qsTr ("Location")

                onTextChanging: _calendar.eventEditor.location = text
            }

            DateTimePicker {
                id: startTimeField

                title: qsTr ("Start")

                onValueChanged: _calendar.eventEditor.startTime = value
            }

            DateTimePicker {
                id: endTimeField

                title: qsTr ("End")

                onValueChanged: _calendar.eventEditor.endTime = value
            }

            DropDown {
                id: folderField

                topMargin: 50

                title: qsTr ("Store in:")

                visible: (_calendar.eventEditor.mode == EventEditor.CreateMode)

                onSelectedValueChanged: {
                    _calendar.eventEditor.folderId = selectedValue.folderId
                    _calendar.eventEditor.accountId = selectedValue.accountId
                }
            }
        }
    }
}
