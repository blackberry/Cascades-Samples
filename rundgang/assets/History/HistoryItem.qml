/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

// A Standard list item used to present a historical feedback event. 
StandardListItem {
    id: item
    title: ListItemData.topic
    
    // Determining imageSource based on the item type.
    imageSource: {
        if(ListItemData.type == "image") {
            "asset:///images/rundgang_camera.png"
        } else if(ListItemData.type == "text") {
            "asset:///images/ic_text.png"
        } else if(ListItemData.type == "audio") {
            "asset:///images/rundgang_microphone.png"
        } else if(ListItemData.type == "qr") {
            "asset:///images/ic_qr_frame.png"
        }
    }
    
    status: ListItemData.time
    
    contextActions: [
        ActionSet {
            title: ListItemData.topic
            subtitle: qsTr("History Item Actions") + Retranslate.onLanguageChanged
            
            DeleteActionItem {
                title: qsTr("Delete") + Retranslate.onLanguageChanged
                
                onTriggered: {
                    // Call list view function to delete items.
                    item.ListItem.view.deleteSelectedItems()
                }
            }
            
            MultiSelectActionItem {
                multiSelectHandler: item.ListItem.view.multiSelectHandler
                
                onTriggered: {
                    // Activate multi-select.
                    multiSelectHandler.active;
                }
            }
        }
    ]
    
    accessibility.description: ListItemData.type
}