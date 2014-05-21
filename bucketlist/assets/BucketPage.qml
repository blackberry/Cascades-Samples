/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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

// This details Page for a bucket item that shows the entire task contained in a bucket
// item and adds functionality for editing the item.

Page {
    id: bucketPage
    property variant item
    property variant bucketModel
    property alias title: titleBar.title
    
    titleBar: TitleBar {
        id: titleBar
        visibility: ChromeVisibility.Visible
    }
    
    Container {
        layout: DockLayout {
        }
        
        Container {
            leftPadding: ui.du(2)
            rightPadding: leftPadding
            horizontalAlignment: HorizontalAlignment.Left
            
            Label {
                id: bucketText
                multiline: true
                text: bucketPage.item.title
                textStyle.base: SystemDefaults.TextStyles.TitleText
            }
        }
    }
    
    shortcuts: [
        SystemShortcut {
            // The edit short cut shows the edit sheet.
            type: SystemShortcuts.Edit
            onTriggered: {
                editSheet.open();
                editSheet.text = bucketText.text;
            }
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("Edit") + Retranslate.onLanguageChanged
            imageSource: "asset:///images/edit.png"
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
                editSheet.open();
                editSheet.text = bucketText.text;
            }
        }
    ]
    
    attachedObjects: [
        EditSheet {
            id: editSheet
            title: qsTr("Edit") + Retranslate.onLanguageChanged
            hintText: "Update bucket item description"
            
            onSaveBucketItem: {
                // Call the function to update the item data.  
                bucketModel.editBucketItem(bucketPage.item, text);
                
                // Update the current item property data used in this Page to do this
                // one has to copy all values to 'tempItem'.
                var tempItem = bucketPage.item
                // Update the item property
                tempItem.title = text
                // Then copy all values back to 'bucketPage.item'
                bucketPage.item = tempItem 
            }
        }
    ]
}
