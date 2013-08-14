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

// This the item component for the items in the bucket list. It is a StandardListItem
// with an action set for altering the state of an item and for entering the multi select state.

StandardListItem {
    id: bucketItem
    title: ListItemData.title
    imageSpaceReserved: false
    
    contextActions: [
        ActionSet {
            title: ListItemData.title
            subtitle: qsTr("Bucket action") + Retranslate.onLanguageChanged
            
            ActionItem {
                title: qsTr("Todo") + Retranslate.onLanguageChanged
                enabled: bucketItem.ListItem.view.dataModel.filter == "todo" ? false : true
                imageSource: "asset:///images/todo.png"
                
                onTriggered: {
                    if (enabled) {
                        bucketItem.ListItem.view.dataModel.setStatus(bucketItem.ListItem.indexPath, "todo");
                        bucketItem.ListItem.view.updateBBMStatus(qsTr("Added to my bucket list: ") + bucketItem.title, "images/todo.png");
                    }
                }
            }
            ActionItem {
                title: qsTr("Finished") + Retranslate.onLanguageChanged
                enabled: bucketItem.ListItem.view.dataModel.filter == "finished" ? false : true
                imageSource: "asset:///images/finished.png"
                
                onTriggered: {
                    if (enabled) {
                        bucketItem.ListItem.view.dataModel.setStatus(bucketItem.ListItem.indexPath, "finished");
                        bucketItem.ListItem.view.updateBBMStatus(qsTr("Kicked from my bucket list: ") + bucketItem.title, "images/finished.png");
                    }
                }
            }
            ActionItem {
                title: qsTr("Chickened out") + Retranslate.onLanguageChanged
                enabled: bucketItem.ListItem.view.dataModel.filter == "chickened" ? false : true
                imageSource: "asset:///images/chickened.png"
                
                onTriggered: {
                    if (enabled) {
                        bucketItem.ListItem.view.dataModel.setStatus(bucketItem.ListItem.indexPath, "chickened");
                        bucketItem.ListItem.view.updateBBMStatus(qsTr("Chickened out on my bucket list: ") + bucketItem.title, "images/chickened.png");
                    }
                }
            }
            ActionItem {
                title: qsTr("Share Item") + Retranslate.onLanguageChanged
                imageSource: "asset:///images/ic_share.png"
                onTriggered: {
                    bucketItem.ListItem.view.dataModel.shareBucketItem(bucketItem.title);
                
                }
            }
            DeleteActionItem {
                title: qsTr("Delete") + Retranslate.onLanguageChanged
                
                onTriggered: {
                    bucketItem.ListItem.view.dataModel.deleteBucketItems(bucketItem.ListItem.indexPath);
                }
            }
            
            MultiSelectActionItem {
                multiSelectHandler: bucketItem.ListItem.view.multiSelectHandler
                
                onTriggered: {
                    multiSelectHandler.active;    
                } 
            }
        }
    ]
}
