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

// Import the bucketmodel set as a type in the application constructor
import com.bucketlist.bucketdata 1.0

Page {
    id: bucketListPage

    // Signal that tells the application that the BBM status should be updated
    signal newBBMStatus(string message, string icon)

    titleBar: TitleBar {
        id: segmentedTitle
        kind: TitleBarKind.Segmented
        scrollBehavior: TitleBarScrollBehavior.Sticky

        // The segmented control decides which filter should be set on the
        // dataModel used by the photo bucket list.
        options: [
            Option {
                // We dont want to allow the user to navigate while multiselecting so we disable if the lists multiSelectHandler is active.
                enabled: !bucketList.multiSelectHandler.active
                text: qsTr("Todo") + Retranslate.onLanguageChanged
                value: ("todo")
            },
            Option {
                enabled: !bucketList.multiSelectHandler.active
                text: qsTr("Finished") + Retranslate.onLanguageChanged
                value: ("finished")
            },
            Option {
                enabled: !bucketList.multiSelectHandler.active
                text: qsTr("Chickened") + Retranslate.onLanguageChanged
                value: ("chickened")
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

        // The ListView is a separate QML component kept in BucketList.qml
        BucketList {
            id: bucketList

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
        }
    }

    shortcuts: [
        SystemShortcut {

            type: SystemShortcuts.CreateNew
            onTriggered: {
                if (! bucketModel.bucketIsFull) {
                    addNew.open();
                    addNew.text = "";
                }
            }
        }
    ]

    // Attached objects of the Bucket List Page
    attachedObjects: [
        EditSheet {
            // A sheet is used to add new items to the list, which is the same sheet used to edit items
            id: addNew

            onSaveBucketItem: {
                bucketModel.addBucketItem(text);
                bucketList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Default);
            }
        },
        ComponentDefinition {
            // A Component definition of the Page used to display more details on the Bucket item.
            id: bucketPage
            source: "BucketPage.qml"
        }
    ]

    actions: [
        // An TextInputActionItem ActionItem for adding more items to the list
        TextInputActionItem {

            hintText: qsTr("Add bucket item") + Retranslate.onLanguageChanged
            // If there are 100 items in the list, we're at the max number of items, we cannot add more items.
            enabled: ! bucketModel.bucketIsFull
            input {
                //change the name of the "submit" key.
                submitKey: SubmitKey.Submit
                // When we change focus to the next focusable item, we will get rid of the keyboard.
                submitKeyFocusBehavior: SubmitKeyFocusBehavior.Next
                //Let's add the text when submitted!
                onSubmitted: {
                    bucketModel.addBucketItem(text);
                    bucketList.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Default);
                    text = ""
                }
            }
        },
        MultiSelectActionItem {
            onTriggered: {
                bucketList.multiSelectHandler.active = true;
            }
        }
    ]

    onCreationCompleted: {
        // Connect the list signal to this page signal to re-emit it when it is triggered
        bucketList.newBBMStatus.connect(newBBMStatus);

        // Connect to the application incoming item signal, fired when an item is
        // received via the invocation framework
        _app.incomingBucketItem.connect(onIncomingBucketItem);
    }

    //The slot used when the application is invoked with a .buk file.
    function onIncomingBucketItem() {
        addNew.open();
        addNew.text = _app.bucketItemTitle;
    }
}
