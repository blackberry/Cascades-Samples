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

// The TopicData Component handles the topic (subject and address list)
// model and its source. This data component does has a Sheet
// for adding a new Topic as its visual representation.
TopicDataSheet {
    property alias model: topicModel
    
    // Properties keeping the most current topic.
    property string currentTopic: qsTr("Press to add topic");
    property string currentAdressList: "";
    
    // This property is false as long as there is no data in the database.
    property bool noTopics: true;  

	attachedObjects: [
	    
        // The model is GroupDataModel populated with all the topics in the
        // data source, ordered by date and not grouped in any way. 
        GroupDataModel {
            id: topicModel
            signal firstItemUpdated(variant data);
            
            sortingKeys: [ "date" ]
            grouping: ItemGrouping.None
            sortedAscending: false
            
            onItemAdded: {
                updateFirstItemInfo(indexPath);
            }
            
            onItemRemoved: {
                updateFirstItemInfo(indexPath);
            }
            
            // Call this function if the first item in the model might
            // have updated (if it is updated is decided by the indexPath).
            function updateFirstItemInfo(indexPath) {
                if(indexPath[0] == [0]) {
                    // Signal that the first item has been updated.
                    firstItemUpdated(data(indexPath));
                }                                
            }
        },
        
        // A custom data source is used (see src/common/customsqldatasource.cpp/.h)
        // It works in a very similar fashion as the DataSource object, but it
        // has functionality for executing queries that alter the database.
        CustomSqlDataSource {
            id: asynkDataSource
            property int loadCounter: 0
            
            // The path to the database relative to the assets folder.
            source: "models/rundgang.db"
            
            // The query that will be executer when the load() function is called.
            query: "SELECT * FROM topics ORDER BY topic LIMIT 10"
            
            // A connection has to be set.
            connection: "topic_connection"            

            onDataLoaded: {
                if (data.length > 0) {
                    // Insert the data in the topic model.
                    topicModel.insertList(data);

                    // If 5 (the incremental offset) or more (10 is requested the first time) items are received keep polling for more items from the source.
                    if (data.length >= 5) {
                        // Load the next batch, the last parameter is set to zero to tell the data source that we are loading data to the list.
                        var offsetData = {
                            "offset": (10 + 5 * loadCounter)
                        };
                        execute("SELECT * FROM topics ORDER BY topic LIMIT 5 OFFSET :offset", offsetData, 0);
                        loadCounter ++;
                    }
                }
            }

			// Insert a new topic in the the topics table of the rundgang.db database. 
            function insertItem(topic, adressList) {
                // First an item that can be added to the data source is created and the query to 
                // add it is executed
                var nowDate = new Date();
                var itemData = {
                    "topic": topic,
                    "adressList": adressList,
                    "date": getFormatedDate()
                }
                execute("INSERT INTO topics (topic, adressList, date) VALUES(:topic, :adressList, :date)", itemData);

                // To get the correct data base id of the item that was just added a consecutive call call to select it is made
                // Using 0 as the last parameter will emit a loadData signal when the data is retrieved and it will be 
                // added to the model in the onDataLoaded signal handler above 
                execute("SELECT * FROM topics WHERE topic=:topic AND adressList=:adressList AND date=:date", itemData, 0);
            }
            
            // Update a topic in the the topics table of the rundgang.db database. 
            function updateItemDate(data) {
                
                // Get the current data model index, needed to update the model once the data source
                // has been updated.
                var currentIndex = topicModel.find(data);
                
                // Update the date to the current date
                data.date = getFormatedDate();
                var result = executeAndWait("UPDATE topics SET topic=:topic, adressList=:adressList, date=:date WHERE topicsid=:topicsid", data, 2);                
                
                if(result) {
                    // If the query on the data source was successful, make the same change in the model.
                    topicModel.updateItem(currentIndex, data);
                }
            }
            
            // Delete a topic in the the topics table of the rundgang.db database.
            function deleteItem(data) {
            	
                    var itemData = {
                        "topicsid": data["topicsid"]
                    };

					var result = executeAndWait("DELETE FROM topics WHERE topicsid=:topicsid", itemData, 3);                

                    if(result) {
                        // If the query on the data source was successful, remove the item from the model as well.
                        topicModel.remove(data);
                    } 
                

            }

			// Helper function for setting up a date in a format that is handled by the database.
            function getFormatedDate() {
                var nowDate = new Date();
                
                // Create a date string in the format MM/DD/YYYY HH:MM:SS (with leading zeros).
                return ('0' + (nowDate.getUTCMonth() + 1)).slice(-2) + "/" + ('0' + nowDate.getUTCDate()).slice(-2) + "/" + nowDate.getFullYear() + " " + ('0' + nowDate.getUTCHours()).slice(-2) + ":" + ('0' + nowDate.getUTCMinutes()).slice(-2) + ":" + ('0' + nowDate.getUTCSeconds()).slice(-2);
            }
        }
    ]

    onCreationCompleted: {
        // Load all data. To get a shorter start up time it might be a good idea to 
        // do this at a later time. 
        asynkDataSource.load();
        
        // Connect to the custom model signal for first item updates to set the current topic.
        topicModel.firstItemUpdated.connect(updateCurrentTopic);
    }
    
    // The TopicDataSheet emit a saveTopic signal when a new item should be added. 
    onSaveTopic: {
        asynkDataSource.insertItem(topic, adressList);
    }

	// Call this function to remove a number of items both from the database and the model.
    function deleteTopics(deletionList) {
        // Start from the end of the list so that that model elements are not shifted,
        // as multiple deletion commence.
        for(var i = deletionList.length - 1; i >= 0; i--) {
            var data = topicModel.data(deletionList[i]);
            asynkDataSource.deleteItem(data);
        }
    }

	// Call this function to "select" a topic, in this context this means update
	// the date so that it is the most current item (moves it to the top of the data in the model).
	function selectTopic(data) {
        asynkDataSource.updateItemDate(data);
    }

	// Call this function to show the TopicDataSheet for adding a new item.
    function newTopic() {
        open();
    }

	// Update the current topic, corresponding to the first item in the model.
    function updateCurrentTopic(data) {
        if(data == undefined) {
            // If the no data exist, the model is empty and the properties are reset to default.
            currentTopic = qsTr("Press to add topic");
            currentAdressList = "";
            noTopics = true;
        } else {
            currentTopic = data.topic;
            currentAdressList = data.adressList
            noTopics = false;            
        }
    }
}
