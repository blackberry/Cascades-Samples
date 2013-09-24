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
 */
import bb.cascades 1.2

// This QML sheet is used for editing and adding new items to the Bucket List application.

Sheet {
    id: editSheet

    // Custom properties
    property alias title: addBar.title
    property alias hintText: itemText.hintText
    property alias text: itemText.text

    // A custom signal is triggered when the acceptAction is triggered.
    signal saveBucketItem(string text)
    Page {
        id: addPage
        titleBar: TitleBar {
            id: addBar
            title: qsTr("Add") + Retranslate.onLanguageChanged
            visibility: ChromeVisibility.Visible
            
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLanguageChanged
                onTriggered: {
                    // Hide the Sheet.
                    editSheet.close()
                }
            }
            
            acceptAction: ActionItem {
                title: qsTr("Save") + Retranslate.onLanguageChanged
                onTriggered: {
                    // Hide the Sheet and emit signal that the item should be saved.
                    editSheet.close();
                    editSheet.saveBucketItem(itemText.text);
                }
            }
        }
        Container {
            id: editPane
            property real margins: 40
            background: Color.create("#f8f8f8")
            topPadding: editPane.margins
            leftPadding: editPane.margins
            rightPadding: editPane.margins
            
            layout: DockLayout {
            }
            
            attachedObjects: [
                TextStyleDefinition {
                    id: editTextStyle
                    base: SystemDefaults.TextStyles.TitleText
                }
            ]
            Container {
                TextArea {
                    id: itemText
                    hintText: qsTr("New item on the bucket list") + Retranslate.onLanguageChanged
                    topMargin: editPane.margins
                    bottomMargin: topMargin
                    preferredHeight: 450
                    maxHeight: 450
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    textStyle {
                        base: editTextStyle.style
                    }
                }
            }// Text Area Container
        }// Edit pane Container
    }// Page
    
    onOpened: {
        itemText.requestFocus()
    }
    
}// Sheet
