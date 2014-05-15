/*
 * Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades.datamanager 1.2
import bb.system 1.0

/*
 * This page shows database rows, with menu options to allow switching
 * between various dataModels to demonstrate the performance difference of the cache
 * when using sqlDataQuery with optional properties such as keyColumn or revisionColumn.
 */
Page {
    id: root
    //! [0]
    // global property to indicate when live revision update is in progress
    // so that a concurrent selection of live updates terminates the process
    property bool liveUpdate: false;
    
    // sql query for updating the revision id in the revision table
    property string updateRevision: "update revision set revision_id = revision_id + 1"
    // sql query to select revision id from the revision table
    property string selectRevision: "select revision_id from revision"
    // sql query to insert a new artist row. This query makes use of bound properties and up-to-date revision id through select statement.
    property string insertQuery: "insert into artist(revision_id,name,realname,profile,data_quality,primary_image,is_group,master_count,group_master_count) select revision_id,:name,:realname,:profile,:dataquality,:primaryimage,0,0,0 from revision"
    //! [0]
    // Screen title bar with app name
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                leftPadding: ui.du(1.1)
                rightPadding: ui.du(1.1)
                
                verticalAlignment: VerticalAlignment.Center
                
                Label {
                    text: "Datamanager Usage"
                    
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 7
                    }
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.White
                    }
                    verticalAlignment: VerticalAlignment.Center
                
                }
            }
        }
    }
    
    Container {
        // The ListView that's used to display the artist data
        //! [1]
        ListView {
            id: listView
            // ListView properties in order to be accessible through ListItemComponents
            property alias deleteq: queryExec
            property string selectRevision: "select revision_id from revision"
            // sql query to delete artist with specific name
            property string deleteQuery: "delete from artist where name = :name"
            property string updateRevision: "update revision set revision_id = revision_id + 1"
            // this variable holds the current SqlDataQuery that the ListView.dataModel is using
            property variant dq: defaultDataQuery
            
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            // The current ListView data model being used
            dataModel: defaultModel
            
            // Function that returns access path to the data images
            // _dataDir is a path exposed to the qml context via applicationui.cpp to the apps data folder
            function imageurl(image) {
                if ("" == image || !_app.fileExists(image)) {
                    return _dataDir + "../../native/assets/images/no_image.png";
                
                }
                return _dataDir + "images/" + image;
            }
            
            listItemComponents: [
                // ListComponent to represent default dataModel's
                ListItemComponent {
                    // Displays the row data
                    StandardListItem {
                        id: itemRoot
                        title: ListItemData.name
                        imageSource: itemRoot.ListItem.view.imageurl(ListItemData.primary_image)
                        description: ListItemData.realname

                        // function that emits data changed signal with the revision update in order
                        // to signal for the ListView to refresh; dq holds the dataQuery that the user has selected
                        function onDeleteExecuted(resultData) {
                            var revision = resultData[0].revision_id;
                            console.log("live delete was performed ");
                            itemRoot.ListItem.view.dq.emitDataChanged(revision);
                            itemRoot.ListItem.view.deleteq.executed.disconnect(itemRoot.onDeleteExecuted)
                        }
                        
                        gestureHandlers: [
                            LongPressHandler {
                                onLongPressed: {
                                    // Set the sql queries to retrieve revision and delete the row with the bound name value
                                    itemRoot.ListItem.view.deleteq.queries = [ itemRoot.ListItem.view.deleteQuery, itemRoot.ListItem.view.updateRevision, itemRoot.ListItem.view.selectRevision ]
                                    itemRoot.ListItem.view.deleteq.bindValues = { "name" : ListItemData.name }
                                    // Connect to the javascript function, which emits signal to force ListView to refresh after change
                                    itemRoot.ListItem.view.deleteq.executed.connect(itemRoot.onDeleteExecuted)
                                    itemRoot.ListItem.view.deleteq.execute()
                                }
                            }
                        ]
                    }
                }
            ]
            onDataModelChanged: {
                console.log("onDataModelChanged....")
            }
            onSelectionChanged: {
                console.log("onSelectionChanged, selected: " + selected)
            }
            onActivationChanged: {
                console.log("onActivationChanged, active: " + active)
            }
        }
        //! [1]
    }
    
    // Load the default model on application startup
    onCreationCompleted: {
        defaultModel.load()
    }
    
    attachedObjects: [
        //! [2]
        // One of the default provided DataModel's
        AsyncDataModel {
            id: defaultModel
            // Standard data query that does not make use of any performance improving
            // properties (i.e. keyColumn, revisionColumn)
            query: SqlDataQuery {
                id: defaultDataQuery
                source: _dataDir + "discogs_small.db"
                query: "select id, name, realname, data_quality, primary_image, is_group, revision_id from artist order by name"
                countQuery: "select count(*) from artist"
                onDataChanged: console.log("data changed: revision=" + revision)
                onError: console.log("SQL query error: " + code + ", " + message)
            }
            onLoaded: console.log("initial model data is loaded")
        },
        //! [2]
        //! [3]
        AsyncDataModel {
            id: withKeyModel
            // Data query that uses one of the performance improving properties - keyColumn
            query: SqlDataQuery {
                id: kdq
                source: _dataDir + "discogs_small.db"
                query: "select id, name, realname, data_quality, primary_image, is_group, revision_id from artist order by name"
                countQuery: "select count(*) from artist"
                keyColumn: "id"
                onDataChanged: console.log("data changed: revision=" + revision)
                onError: console.log("SQL query error: " + code + ", " + message)
            }
            onLoaded: console.log("initial model data is loaded")
        },
        //! [3]
        //! [4]
        AsyncDataModel {
            id: withRevisionModel
            // Data query that uses one of the performance improving properties - (revisionColumn, revisionQuery)
            query: SqlDataQuery {
                id: rdq
                source: _dataDir + "discogs_small.db"
                query: "select id, name, realname, data_quality, primary_image, is_group, revision_id from artist order by name"
                countQuery: "select count(*) from artist"
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision"
                onDataChanged: console.log("data changed: revision=" + revision)
                onError: console.log("SQL query error: " + code + ", " + message)
            }
            onLoaded: console.log("initial model data is loaded")
        },
        //! [4]
        //! [5]
        AsyncDataModel {
            id: withKeyAndRevisionModel
            // Data query that uses both performance improving properties - keyColumn, (revisionColumn, revisionQuery)
            query: SqlDataQuery {
                id: krdq
                source: _dataDir + "discogs_small.db"
                query: "select id, name, realname, data_quality, primary_image, is_group, revision_id from artist order by name"
                countQuery: "select count(*) from artist"
                keyColumn: "id"
                revisionColumn: "revision_id"
                revisionQuery: "SELECT revision_id FROM revision"
                onDataChanged: console.log("data changed: revision=" + revision)
                onError: console.log("SQL query error: " + code + ", " + message)
            }
            onLoaded: console.log("initial model data is loaded")
        },
        //! [5]
        //! [6]
        QueryExec {
            // ***************************************************************************
            // Start it by calling execute(). Can be stopped by calling stop().
            // Each time it will update x rows (including both overall revision, item revision)
            // and then notify the data model via its associated query.
            // Next time (after 1 second delay) it will go down 100 rows and do the same.
            //
            
            // Query sequence:
            property string updateContact: "update artist set revision_id = (select revision_id from revision) " +
            "where rowid >= :startRow and rowid < (:startRow + 5)"
            property int startRow: 0
            id:         updateQuery
            times:      10 // execute x times (default is 1)
            interval:   5000 // delay y milliseconds before each execution (default is 1000)
            source:     _dataDir + "discogs_small.db"
            queries:    [ updateRevision, updateContact, selectRevision ]
            bindValues: { "startRow" : startRow }
            onError:    console.log("live update error: " + errorType + ", " + errorMessage)
            onExecuted: {
                var revision = data[0].revision_id;
                console.log("live query update was performed: startRow=" + startRow + "; revision=" + revision);
                listView.dq.emitDataChanged(revision);
                startRow = (startRow + 100) % 1000; // next time start further down, wrapping at the bottom of the 1024 row table
            }
        },
        //! [6]
        //! [7]
        // QueryExec instance used for the various SqlDataQuery's
        QueryExec {
            id: queryExec
            source: _dataDir + "discogs_small.db"
            queries: [ ]
            onError: console.log("error: " + errorType + ", " + errorMessage)
        },
        //! [7]
        // Toast informing the user on how to delete items in the list
        SystemToast {
            id: deleteToast
            body: qsTr("Long press an item to remove from list")
            button.label: qsTr("Ok")
            button.enabled: true
        }
    ]
    
    actions: [
        //! [8]
        // action item to select the key supported sqlDataQuery
        ActionItem {
            title: "sqlDataQuery+key"
            imageSource: "images/query_with_key.png"
            onTriggered: {
                withKeyModel.load()
                listView.dq = kdq
                listView.dataModel = withKeyModel
            } 
        },
        // action item to select the revision supported sqlDataQuery
        ActionItem {
            //ActionBar.placement: ActionBarPlacement.OnBar
            title: "sqlDataQuery+rev"
            imageSource: "images/query_with_revision.png"
            onTriggered: {
                withRevisionModel.load()
                listView.dq = rdq
                listView.dataModel = withRevisionModel
            } 
        },
        //action item to select the sqlDataQuery supporting both key and revision
        ActionItem {
            title: "sqlDataQuery+key+rev"
            imageSource: "images/both.png"
            onTriggered: {
                withKeyAndRevisionModel.load()
                listView.dq = krdq
                listView.dataModel = withKeyAndRevisionModel
            } 
        },
        //! [8]
        //! [9]
        // action item to activate live updating of revisions
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "LiveUpdate"
            imageSource: "images/ic_edit.png"
            onTriggered: {
                if(!liveUpdate) {
                    updateQuery.execute()
                    liveUpdate = true
                } else {
                    updateQuery.stop()
                    liveUpdate = false
                }
            }
        },
        // action item to activate toast describing delete functionality
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "delete"
            imageSource: "images/ic_delete.png"
            onTriggered: {
                deleteToast.exec()
            } 
        },
        // action item to reset back to default model
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Reset"
            imageSource: "images/list_reset.png"
            onTriggered: {
                defaultModel.load()
                listView.dq = defaultDataQuery
                listView.dataModel = defaultModel
            } 
        },
        // action item to add new row item
        ActionItem {
            id: addAction
            
            ActionBar.placement: ActionBarPlacement.OnBar
            title: "Add"
            imageSource: "images/ic_add.png"
            
            // function emits data changed signal with new revision in order
            // for ListView to refresh so that the addition is visually displayed in the list
            function onAddExecuted(resultData) {
                console.log("live record addition ")
                var revision = resultData[0].revision_id;
                listView.dq.emitDataChanged(revision);
                queryExec.executed.disconnect(addAction.onAddExecuted)
            }
            
            onTriggered: {
                // provide query for inserting new row item
                queryExec.queries = [ updateRevision, selectRevision, insertQuery]
                // query bound values containing the new row artist values
                queryExec.bindValues = { "name" : "Homer" + Math.random().toString(36).substring(2, 8),
                                         "realname" : "Baercis",
                                         "profile" : "Sr.Attorney at large",
                                         "dataquality" : "Correct",
                                         "primaryimage" : "blah.jpg"}
                // connects to the javascript function which emits signal to force ListView to refresh
                queryExec.executed.connect(addAction.onAddExecuted)
                queryExec.execute()
            } 
        }
        //! [9]
    ]
}
