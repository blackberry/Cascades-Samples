/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades.pickers 1.0

// Select an image to share
//! [0]
Container {
    topPadding: 40.0

    property string invokeTarget

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    Container {
        rightPadding: 20.0

        Button {
            horizontalAlignment: HorizontalAlignment.Center
            text: qsTr("Select Image")
            onClicked: {
                picker.open()
            }
        }

        Button {
            id: btnPostText
            text: qsTr("Post")
            visible: (picker.selectedFile != "")
            onClicked: {
                _socialInvocation.invoke(invokeTarget, "bb.action.SHARE",
                                         "image/jpeg",
                                         "file://" + picker.selectedFile)
            }
        }
    }

    ImageView {
        imageSource: "file://" + picker.selectedFile
        maxHeight: 475
        maxWidth: 325
        scalingMethod: ScalingMethod.AspectFit
        verticalAlignment: VerticalAlignment.Top
    }

    attachedObjects: [
        FilePicker {
            id: picker
            property string selectedFile
            title: qsTr("File Picker")
            mode: FilePickerMode.Picker
            type: FileType.Picture
            viewMode: FilePickerViewMode.GridView
            onFileSelected: {
                selectedFile = selectedFiles[0]
            }
        }
    ]
}
//! [0]
