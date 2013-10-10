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
import bb.cascades 1.2
import bb.cascades.pickers 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Invoke") + Retranslate.onLanguageChanged
    }
    Container {

    }
    actions: [
        InvokeActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            query.uri: "http://developer.blackberry.com"
            query.invokeActionId: "bb.action.SHARE"

        },
        InvokeActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            query.mimeType: "application/vnd.blackberry.string.address"
            query.invokeActionId: "bb.action.NAVIGATETO"
            data: "25202 Prado Del Grandioso, Calabasas, California"

        },
        InvokeActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/bbworld.png"
            title: qsTr("BlackBerry World") + Retranslate.onLanguageChanged
            query {
                invokeTargetId: "sys.appworld"
                invokeActionId: "bb.action.OPEN"
                uri: "appworld://vendor/4"
            }
        },
        ActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/filepicker.png"
            title: qsTr("FilePicker") + Retranslate.onLanguageChanged
            attachedObjects: [
                FilePicker {
                    id: filePicker
                    type: FileType.Picture
                    title: "Select Picture"
                    directories: [ "/accounts/1000/shared/camera" ]
                    onFileSelected: {
                        console.log("FileSelected signal received : " + selectedFiles);
                    }
                }
            ]
            onTriggered: {
                filePicker.open();
            }
        }
    ]
}
