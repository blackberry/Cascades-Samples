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
import bb.data 1.0
import com.quotes.data 1.0

// The Page Containing the list of quotes. Data is loaded from SQL using the CustomSqlDataSource
// and puts into a GroupModel (QuotesModel.qml), which populates the list.
Page {
    id: quoteListPage

    // State variable used to make sure that the onItemAdded signals are only considered
    // after showing the add sheet the first time (to avoid signals appearing at start up).
    property bool addShown: false
    property variant selectedData

	// We store the add sheet in a variable while its shown so we can destroy it once it is closed.
    property Sheet addSheet

    // Custom signals used to communicate navigation
    signal showQuotesPage()
    signal listEmpty()
    
    Container {
        ListView {
            id: quotesList
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }

            // The data model used defined in the attachedObjects for the list.
            dataModel: quotesModel

            // There are two types of item; header for alphabetic groups and standard items
            // showing the person name.
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: quoteItem
                        // Some entries only have a last name (we enforce last name always being added)
                        title: ((ListItemData.firstname == undefined) ? "" : ListItemData.firstname + " ") + ListItemData.lastname
                    }
                },
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData
                    }
                }
            ]
            
            onTriggered: {
                // To avoid triggering navigation when pressing the header items, we check so that the
                // index path length is larger then one (one entry would be a group under a header item).
                if (indexPath.length > 1) {
                    // Update the data that will be shown in the Quotes Page.
                    quoteListPage.selectedData = dataModel.data(indexPath);
                    quoteListPage.showQuotesPage();
                }
            }
            
            attachedObjects: [
                // See QuotesModel.qml for set up of the GroupDataModel
                QuotesModel {
                    id: quotesModel
                    onUpdateSelectedData: {
                        if (indexPath[0] >= 0 && indexPath[1] >= 0) {
                            //quotesList.scrollToItem(indexPath, ScrollAnimation.Default);
                            quoteListPage.selectedData = data(indexPath);
                        } else {
                            quoteListPage.listEmpty();
                        }
                    }
                },
                // Custom data source object, loads data using a SqlConnection, see customsqldatasource.cpp
                CustomSqlDataSource {
                    id: asynkDataSource
                    source: "sql/quotes.db"
                    query: "SELECT * FROM quotes ORDER BY lastname LIMIT 10"
                    property int loadCounter: 0
                    
                    onDataLoaded: {
                        if (data.length > 0) {
                            quotesModel.insertList(data);
                            // Load the next batch, the last parameter is set to zero to tell the data source that we are loading data to the list.
                            var offsetData = {"offset": (10 + 5 * loadCounter)};
                            execute("SELECT * FROM quotes ORDER BY lastname LIMIT 5 OFFSET :offset", offsetData, 0); 
                            loadCounter ++;
                        }
                    }
                }
            ] // attachedObjects
            
            onCreationCompleted: {
                // When the list view has been created load the data.
                asynkDataSource.load();
            }
        } // ListView
    } // Container

    shortcuts: [
        SystemShortcut {
            // The create new short cut shows the add sheet.
            type: SystemShortcuts.CreateNew
            onTriggered: {
                showAddSheet();
            }
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("Add") + Retranslate.onLanguageChanged
            imageSource: "asset:///images/Add.png"
            ActionBar.placement: ActionBarPlacement.Signature
            
            onTriggered: {
            	showAddSheet();
            }
        }
    ]
    
    onCreationCompleted: {
        // When a new quote is added navigate to the Quotes Page, so the showQuotesPage signal is connected to the  models new data item signal.
        quotesModel.newDataItem.connect(showQuotesPage);
    }

    attachedObjects: [
        ComponentDefinition {
            // Component definition for the Sheet that will be shown when pressing the Add action below
            id: addSheetDef
            source: "AddSheet.qml"
        }
    ]

    function showAddSheet(){
        // Create the add sheet and connect to its signals.
        addSheet = addSheetDef.createObject();
        addSheet.addNewRecord.connect(addQuote);
        addSheet.closed.connect(addSheetClosed);
        addSheet.open();
        quoteListPage.addShown = true;
    }

    function addSheetClosed() {
        // The add sheet is destroyed once it is completely closed otherwise there would be a memory leak.
        addSheet.deleteLater()
    }

    // Slot functions for add/delete/update operations, makes calls to update the data base and modifies the model data.
    function deleteSelectedQuote() {
        var itemData = {"id": quoteListPage.selectedData["id"]};
        asynkDataSource.execute("DELETE FROM quotes WHERE id=:id", itemData);        
        quotesList.dataModel.remove(quoteListPage.selectedData);
    }
    
    function updateSelectedQuote(firstName, lastName, quote) {
    	var itemData = {"firstname": firstName, "id": quoteListPage.selectedData["id"], "lastname": lastName, "quote": quote }
        asynkDataSource.execute("UPDATE quotes SET firstname=:firstName, lastname=:lastname, quote=:quote WHERE id=:id", itemData);

		// Update the data model item at the indexPath of the currently selectedData item.
        var indexPath = quotesList.dataModel.find(quoteListPage.selectedData);
        quotesList.dataModel.updateItem(indexPath, itemData);
    }
    
    function addQuote(firstName, lastName, quote) {
        var itemData = {"firstname": firstName, "lastname": lastName, "quote": quote }        
        asynkDataSource.execute("INSERT INTO quotes (firstname, lastname, quote) VALUES(:firsname, :lastName, :quote)", itemData);
        quotesList.dataModel.insert(itemData);
    }
    
}// Page
