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

// The HistoryData Component handles the history model and its source.
// This data component does not have any visual associated with it,
// the model can be used to populate for example a list.
Container {
    property alias model: historyModel
    
    attachedObjects: [
        // The model is GroupDataModel populated with the history items in the
        // data source, ordered and grouped by date. 
        GroupDataModel {
            id: historyModel
            property bool dataLoaded: false

            sortingKeys: [ "date", "time" ]
            sortedAscending: false
            grouping: ItemGrouping.ByFullValue
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
            query: "SELECT * FROM history ORDER BY date LIMIT 10"
            
            // A connection has to be set.
            connection: "history_connection"
                        
			onDataLoaded: {
                if (data.length > 0) {
                    historyModel.insertList(data);
                    
                    // If 5 (the incremental offset) or more (10 is requested the first time) items are received keep polling for more items from the source.
                    if (data.length >= 5) {
                        // Load the next batch, the last parameter is set to zero to tell the data source that we are loading data to the list.
                        var offsetData = {
                            "offset": (10 + 5 * loadCounter)
                        };
                        execute("SELECT * FROM history ORDER BY date LIMIT 5 OFFSET :offset", offsetData, 0);
                        loadCounter = loadCounter + 1;
                    }
                }
            }
            
            // Insert a new history item in the the history table of the rundgang.db database.
            function insertItem(topic, adressList, filePath, type, additionalData) {
                // First an item that can be added to the data source is created and the query to 
                // add it is executed.
                var nowDate = new Date();
                var itemData = {
                    "topic": topic,
                    "adressList": adressList,
                    "filePath": filePath,
                    "type": type,
                    "text": additionalData,
                    "date": getFormatedDate(),
                    "time": getFormatedTime()
                }
                execute("INSERT INTO history (topic, adressList, filePath, type, text, date, time) VALUES(:topic, :adressList, :filePath, :type, :text, :date, :time)", itemData);

                // If the model has been populated select the newly added item, so that it will be added as well.
                if (historyModel.dataLoaded) {
                    // Using 0 as the last parameter will emit a loadData signal when the data is retrieved and it will be
                    // added to the model in the onDataLoaded signal handler above
                    execute("SELECT * FROM history WHERE topic=:topic AND adressList=:adressList AND filePath=:filePath AND type=:type AND text=:text AND date=:date AND time=:time", itemData, 0);
                }
            }
            
            // Delete a history item in the the history table of the rundgang.db database.
            function deleteItem(data) {
                var itemData = {
                    "historyid": data["historyid"]
                };
                
                var result = executeAndWait("DELETE FROM history WHERE historyid=:historyid", itemData, 3);                
                
                if(result) {
                    // If the query on the data source was successful, remove the item from the model as well.
                    historyModel.remove(data);
                } 
            }

			// Helper function to get the current time.
			function getFormatedDate() {
                var nowDate = new Date();
                
                // Create a date string in the format YYYY-MM-DD (with leading zeros).
                return nowDate.getFullYear() + "/" + ('0' + (nowDate.getUTCMonth() + 1)).slice(-2) + "/" + ('0' + nowDate.getUTCDate()).slice(-2); 
            }
            
            // Helper function to get the current date.
            function getFormatedTime() {
                var nowDate = new Date();
                
                // Create a time string in the format HH:MM:SS (with leading zeros).
                return ('0' + nowDate.getHours()).slice(-2) + ":" + ('0' + nowDate.getMinutes()).slice(-2) + ":" + ('0' + nowDate.getSeconds()).slice(-2);
            }
        }
    ]
    
    // Call this function to load the data in the data source, which will add it to the model.
    function loadHistory() {
        // Only tell the data source to populate the model if it has not already been done.
        if(!historyModel.dataLoaded) {
            asynkDataSource.load();
            historyModel.dataLoaded = true;
        }
    }
    
    // Call this function to remove a number of items both from the database and the model. 
    function deleteHistory(deletionList){
        // Start from the end of the list so that that model elements are not shifted,
        // as multiple deletion commence.
        for(var i = deletionList.length - 1; i >= 0; i--) {
            var data = historyModel.data(deletionList[i]);
            asynkDataSource.deleteItem(data);
        }
    }

	// Call this function to add a history item to the data source.
	function addHistory(emailData, adressList, type, additionalData) {
	    // Make sure all attributes are defined or problems with database queries might occur.
        var filePath = (emailData["attachment"] == undefined) ? "" : emailData["attachment"];
        var topic = (emailData["subject"] == undefined) ? "" : emailData["subject"];
        var type = (type == undefined) ? "" : type;
        var additionalData = (additionalData == undefined) ? "" : additionalData;        
        var adressList = (adressList == undefined) ? "" : adressList;
        
        // Call the data source to insert a new item.
        asynkDataSource.insertItem(topic, adressList, filePath, type, additionalData);
    }
}
