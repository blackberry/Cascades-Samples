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

// Import the bucketmodel set as a type in the application constructor
import com.bucketlist.bucketdata 1.0

// Import the items folder for the custom list items
import "items"

ListView {
    id: bucketList

    // Signal that tells the application that the BBM status should be updated
    signal newBBMStatus(string message, string icon)

    // The data model is defined in the attached object list below.
    dataModel: bucketModel
    
    layoutProperties: StackLayoutProperties {
        spaceQuota: 1
    }
    
    listItemComponents: [
        ListItemComponent {
            type: "todo"
            
            TodoItem {
                // List item component (see items/TodoItem.qml for definition).
            }
        }
    ]

    // The multi-select handler of the ListView, which will add an additional multi-select item to the list-items
    // context menu (accessed by long-pressing an item). In the multi-select session, it is possible to select
    // several items in the list and perform a collective action on them (like e.g. deleting many items at once).
    multiSelectHandler {
        status: qsTr("None selected") + Retranslate.onLanguageChanged

        // The actions that can be performed in a multi-select sessions are set up in the actions list.
        actions: [
            ActionItem {
                title: qsTr("Todo") + Retranslate.onLanguageChanged
                
                // Since it is only possible to change the state from one state to another,
                // ActionItems are disabled if they do not result in a state change.
                // For example, todo -> finished is allowed but todo -> todo is not.
                enabled: bucketModel.filter == "todo" ? false : true
                imageSource: "asset:///images/todo.png"
                
                onTriggered: {
                    if (enabled) {
                        // Change the status of the selected items to "todo", clear selection before
                        // performing the action, since otherwise the ListItem will not be in the correct
                        // state when it is recycled by the list view (it will blink)
                        var selectionList = bucketList.selectionList();
                        bucketList.clearSelection();
                        bucketModel.setStatus(selectionList, "todo");
                        updateBBMStatus(qsTr("Added some items to my bucket list"), "images/todo.png");
                    }
                }
            },
            ActionItem {
                title: qsTr("Finished") + Retranslate.onLanguageChanged
                enabled: bucketModel.filter == "finished" ? false : true
                imageSource: "asset:///images/finished.png"
                
                onTriggered: {
                    if (enabled) {
                        // Change the status of the selected items to "finished". Clear selection before items are manipulated to avoid blink.
                        var selectionList = bucketList.selectionList();
                        bucketList.clearSelection();
                        bucketModel.setStatus(selectionList, "finished");
                        updateBBMStatus("Finished some items to my bucket list", "images/finished.png");
                    }
                }
            },
            ActionItem {
                title: qsTr("Chickened out") + Retranslate.onLanguageChanged
                enabled: bucketModel.filter == "chickened" ? false : true
                imageSource: "asset:///images/chickened.png"
                
                onTriggered: {
                    if (enabled) {
                        // Change the status of the selected items to "chickened". Clear selection before items are manipulated to avoid blink.
                        var selectionList = bucketList.selectionList();
                        bucketList.clearSelection();
                        bucketModel.setStatus(selectionList, "chickened");
                        updateBBMStatus(qsTr("Chickened out on some items to my bucket list"), "images/chickened.png");
                    }
                }
            },
            // Since the delete action has a reserved space at the bottom of the list in the
            // context menu it needs to be defined as a special DeleteActionItem to be shown in the
            // correct place.
            DeleteActionItem {
                title: qsTr("Delete") + Retranslate.onLanguageChanged
                
                onTriggered: {
                    // Delete the selected items. Clear selection before items are manipulated to avoid blink.
                    var selectionList = bucketList.selectionList();
                    bucketList.clearSelection();
                    bucketModel.deleteBucketItems(selectionList);
                }
            }
        ]
    }
    onTriggered: {
        // When an item is triggered, a navigation takes place to a detailed
        // view of the item where the user can edit the item. The page is created
        // via the ComponentDefinition from the attached objects in the NavigationPane.
        var chosenItem = dataModel.data(indexPath);
        var page = bucketPage.createObject();

        // Set the Page properties and push the Page to the NavigationPane.
        page.item = chosenItem;
        page.bucketModel = bucketModel;
        var option = segmentedTitle.selectedOption;
        page.title = option.text;
        nav.push(page);
    }
    onSelectionChanged: {
        // Call a function to update the number of selected items in the multi-select view.
        updateMultiStatus();
    }
    function itemType(data, indexPath) {
        // There is only have one type of item in the list, so "todo" is always returned.
        return "todo";
    }
    function updateMultiStatus() {

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

    function updateBBMStatus(message, image) {
        newBBMStatus(message, image);
    }
}
