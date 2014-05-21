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

// A list for presenting 
ListView {
    id: historyList
    
    // Signal that is emitted when detail about an item should be presented.
    signal showHistoryDetails(variant itemData);
    
    // Signal that is emitted when a list of items should be deleted.
    signal deleteHistoryItems(variant deletionList);
        
    listItemComponents: [
        ListItemComponent {
            type: "header"
            Header {
                
                // Show the date as a string formatted by Java script.
                title: {
                    var date = new Date(ListItemData);
                    date.toDateString()   
                }
            }
        },
        ListItemComponent {
            type: "item"
            HistoryItem {
            }
        }
    ]
    
    multiSelectHandler {
        status: qsTr("None selected") + Retranslate.onLanguageChanged
        
        // The actions that can be performed in a multi-select sessions are set up in the actions list.
        actions: [
            DeleteActionItem {
                title: qsTr("Delete") + Retranslate.onLanguageChanged
                
                onTriggered: {
                    historyList.deleteSelectedItems()
                }
            }
        ]
    }
    
    onTriggered: {
        // Emit signal that tells the receiver to present all details in the itemData.
        var itemData = dataModel.data(indexPath);
        showHistoryDetails(itemData);
    }
    
    onSelectionChanged: {    
        // The status text of the multi-select handler is updated to show how
        // many items are currently selected.
        if (selectionList().length > 1) {
            multiSelectHandler.status = selectionList().length + qsTr(" items selected");
        } else if (selectionList().length == 1) {
            multiSelectHandler.status = qsTr("1 item selected");
        } else {
            multiSelectHandler.status = qsTr("None selected");
        }
    }
    
    function deleteSelectedItems() {
        // Emit signal that will remove the items from the list.
        deleteHistoryItems(historyList.selectionList());
    }
    
    function itemType(data, indexPath) {
        if (indexPath.length == 1) {
            return "header";
        } 
        return "item"
    }
}