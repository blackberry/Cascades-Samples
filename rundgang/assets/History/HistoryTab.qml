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
import "../"

// The History Tab present a list of feedback that has been sent historically. 
// With functionality for deleting old items and navigating to a details view.
NavigationPane {
    id: navHistory

    Page {
        titleBar: TitleBar {
            title: qsTr("History") + Retranslate.onLocaleOrLanguageChanged
        }

        Container {
            HistoryList {
                id: historyList
                
                // The data model is kept in a global data object set up in main.qml
                dataModel: historyData.model
                
                onShowHistoryDetails: {
                    // Push the Details view to present more information about the item.
                    pushHistoryDetails(itemData);
                }
                
                onDeleteHistoryItems: {
                    // Delete the items in the list from the data source and ultimately from the model and thereby the list.
                    historyData.deleteHistory(deletionList);
				}
            }
        }

		actions: [
            // The specialized ActionItem for multi select sets the lists multiSelectHandler when triggered.
            MultiSelectActionItem {
                onTriggered: {
                    historyList.multiSelectHandler.active = true;
                }
            }
        ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: detailsPage
            source: "asset:///History/HistoryDetails.qml"
        }
    ]
    
    onPopTransitionEnded: {
        // All pages in this NavigationPane is created via a Component definition, so they should be destroyed.
        page.destroy();
    }
    
    function pushHistoryDetails(itemData){
        // Create the details page and set its properties from the selected data.
        var historyDetails = detailsPage.createObject();
        historyDetails.topic = itemData.topic;
        historyDetails.adressList = itemData.adressList;
        historyDetails.date = itemData.date;
        historyDetails.filePath = itemData.filePath;
        historyDetails.additionalData = itemData.text;
        historyDetails.informationType = itemData.type
        navHistory.push(historyDetails);
    }
}