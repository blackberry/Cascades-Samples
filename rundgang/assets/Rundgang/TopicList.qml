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
import com.rundgang 1.0

// A Page with a list of topics with multi-select for deletion.
Page {
    property alias listModel: topicList.dataModel
    
    // Signal emitted when a topic has been selected and should become the current topic of the app.
    signal topicSelected(variant data);
    
    // Signal emitted when a view for adding a new topic should be shown.
    signal addTopic();
    
    // Signal triggered when a list of items should be removed from the list (removed from the model).
    signal deleteTopics(variant deletionList)

    titleBar: TitleBar {
    	title: qsTr("Topics");
    }

    ListView {
        id: topicList

        listItemComponents: ListItemComponent {
            type: "item"

            CustomListItem {
                id: topicItem
                highlightAppearance: HighlightAppearance.Full

                content: TopicItem {
                    id: item
                    topic: ListItemData.topic
                    adressList: ListItemData.adressList
                    
                    attachedObjects: [
                        // When using a CustomListItem currently the height have to be adjusted of the 
                        // item after the layouting has been done or the entire item will not be shown
                        LayoutUpdateHandler {
                            onLayoutFrameChanged: {
                                topicItem.preferredHeight = layoutFrame.height
                            }
                        }
                    ]
                }
                
                // Item actions that are shown in the overflow when long pressing an item.
                contextActions: [
                    ActionSet {
                        title: ListItemData.topic
                        subtitle: qsTr("Topic Actions") + Retranslate.onLanguageChanged

                        DeleteActionItem {
                            title: qsTr("Delete") + Retranslate.onLanguageChanged

                            onTriggered: {
                                // Call the list view delete selected Items function for deletion.
                                topicItem.ListItem.view.deleteSelectedItems()
                            }
                        }

                        MultiSelectActionItem {
                            multiSelectHandler: topicItem.ListItem.view.multiSelectHandler

                            onTriggered: {
                                // Set the list views multi select handler as active.
                                multiSelectHandler.active;
                            }
                        }
                    }
                ]
            }
        }
        
        multiSelectHandler {
            status: qsTr("None selected") + Retranslate.onLanguageChanged
            
            // The actions that can be performed in a multi-select sessions are set up in the actions list.
            actions: [
                DeleteActionItem {
                    title: qsTr("Delete") + Retranslate.onLanguageChanged
                    
                    onTriggered: {
                        // Call the ListView delete function.
                        topicList.deleteSelectedItems()
                    }
                }
            ]
        }

        onTriggered: {
            // An item has been selected it will now become the current item. 
            topicSelected(dataModel.data(indexPath));
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
            // Emit signal for deleting the items currently in the selection list.
            deleteTopics(topicList.selectionList());
        }
    }

    actions: [
        ActionItem {
            title: qsTr("New Topic") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///images/rundgang_add_entry.png"
            ActionBar.placement: ActionBarPlacement.Signature

            onTriggered: {
                // Emit signal that the user want to add a new topic.
                addTopic();
            }
        },
        MultiSelectActionItem {
            onTriggered: {
                // Set the multi-select handler to active enabling multiple select in the list.
                topicList.multiSelectHandler.active = true;
            }
        }
    ]
    
    shortcuts: [
        SystemShortcut {
            type: SystemShortcuts.CreateNew
            onTriggered: {
                // When the create new shortcut is triggered emit the signal that the user want to add a new topic.
                addTopic();
            }
        }
    ]
}
