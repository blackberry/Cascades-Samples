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

import bb.cascades 1.0

// Import the bucketmodel set as a type in the application constructor.
import com.bucketlist.bucketmodel 1.0
import "items"

NavigationPane {
    id: nav
    Page {
        id: bucketListPage
        
        titleBar: TitleBar {
            id: segmentedTitle
            kind: TitleBarKind.Segmented
            //enabled: false
            // The segmented control decides which filter should be set on the
            // dataModel used by the photo bucket list.
            options: [
                Option {
                    text: "Todo"
                    value: "todo"
                },
                Option {
                    text: "Finished"
                    value: "finished"
                },
                Option {
                    text: "Chickened out"
                    value: "chickened"
                }
            ]
            
            onSelectedValueChanged: {
                // When a new Option is selected the dataModel of the ListView, the bucketModels
                // filter is set and the list is repopulated.
                bucketModel.filter = selectedValue;
                if (selectedValue != "todo") {
                    bucketListPage.actionBarVisibility = ChromeVisibility.Hidden
                } else {
                    bucketListPage.actionBarVisibility = ChromeVisibility.Visible
                }
            }
        }
        
        Container {
            topPadding: 20
            
            ListView {
                id: bucketList

                // The data model is defined in the attached object list below.
                dataModel: bucketModel
                attachedObjects: [

                    // The bucket model is a non visible object so it is set up as an attached object.
                    // The model itself is a QListDataModel defined in bucketmodel.h and registered
                    // as a type in the creation of the application.
                    BucketModel {
                        id: bucketModel

                        // The path to the JSON file with initial data, this file will be moved to
                        // the data folder on the first launch of the application (in order to
                        // be able to get write access).
                        jsonAssetPath: "app/native/assets/models/bucket.json"

                        // The filtering is initially set to "todo" to show items which has not
                        // been checked off the list so far.
                        filter: "todo"
                    }
                ]
                
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
                    status: "None selected"

                    // The actions that can be performed in a multi-select sessions are set up in the actions list.
                    actions: [
                        ActionItem {
                            title: "Todo"

                            // Since it is only possible to change the state from one state to another,
                            // ActionItems are disabled if they do not result in a state change.
                            // For example, todo -> finished is allowed but todo -> todo is not.
                            enabled: bucketModel.filter == "todo" ? false : true
                            imageSource: "asset:///images/todo.png"
                            
                            onTriggered: {
                                if (enabled) {
                                    // Change the status of the selected items to "todo".
                                    bucketModel.setStatus(bucketList.selectionList(), "todo");
                                }
                            }
                        },
                        ActionItem {
                            title: "Finished"
                            enabled: bucketModel.filter == "finished" ? false : true
                            imageSource: "asset:///images/finished.png"
                            
                            onTriggered: {
                                if (enabled) {
                                    // Change the status of the selected items to "finished".
                                    bucketModel.setStatus(bucketList.selectionList(), "finished");
                                }
                            }
                        },
                        ActionItem {
                            title: "Chickened out"
                            enabled: bucketModel.filter == "chickened" ? false : true
                            imageSource: "asset:///images/chickened.png"
                            
                            onTriggered: {
                                if (enabled) {
                                    // Change the status of the selected items to "chickened".
                                    bucketModel.setStatus(bucketList.selectionList(), "chickened");
                                }
                            }
                        },
                        // Since the delete action has a reserved space at the bottom of the list in the
                        // context menu it needs to be defined as a special DeleteActionItem to be shown in the
                        // correct place.
                        DeleteActionItem {
                            title: "Delete"
                            
                            onTriggered: {
                                // Delete all the selected items.
                                bucketModel.deleteBucketItems(bucketList.selectionList());
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
                        multiSelectHandler.status = selectionList().length + " items selected";
                    } else if (selectionList().length == 1) {
                        multiSelectHandler.status = "1 item selected";
                    } else {
                        multiSelectHandler.status = "None selected";
                    }
                }
            }
        }
        
        attachedObjects: [
            EditSheet {
                // A sheet is used to add new items to the list, which is the same sheet used to edit items
                id: addNew
                onSaveBucketItem: {
                    bucketModel.addBucketItem(text);
                    bucketList.scrollToPosition(ScrollPosition.Top, ScrollAnimation.Default);
                }
            },
            ComponentDefinition {
                // A Component definition of the Page used to display more details on the Bucket item.
                id: bucketPage
                source: "BucketPage.qml"
            }
        ]
        
        actions: [
            ActionItem {

                // An ActionItem for adding more items to the list
                title: "Add"
                imageSource: "asset:///images/add.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                // If there are 100 items in the list, we're at the max number of items, we cannot add more items.
                enabled: ! bucketModel.bucketIsFull
                
                onTriggered: {
                    addNew.open();
                    addNew.text = "";
                }
            }
        ]
    }
    
    onCreationCompleted: {
        // When the application NavigationPane is created, we tell Cascades that we support all orientations.
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
    
    onPopTransitionEnded: {
        // Transition is done destory the Page to free up memory.
        page.destroy();
    }        
}
