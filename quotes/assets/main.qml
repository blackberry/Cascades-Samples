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
import "AddPage"

NavigationPane {
    id: nav
    property variant _contentView
    
    // State variable used to make sure that the onItemAdded signals are only considered 
    // after showing the add sheet the first time (to avoid signals appearing at start up). 
    property bool addShown: false
    
    Page {
        id: quoteListPage
        Container {
            background: Color.create("#f8f8f8")
            ListView {
                objectName: "quotesList"
                id: quotesList
                
                layout: StackListLayout {
                    headerMode: ListHeaderMode.Sticky
                }

                // An XML model can be used to prototype the UI until the SQL model is in place,
                // to use ituncomment the line below and comment out the quotesModel.
                //dataModel: XmlDataModel { source:"models/quotes.xml" }
                
                // The data model used defined in the attachedObjects for the list.
                dataModel: quotesModel

                // There are two types of item, header for alphabetic groups and standard items
                // showing the person name.
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            reserveImageSpace: false
                            title: {
                                // Some entries only have a last name (we enforce last name always being added)
                                if (ListItemData.firstname == undefined) {
                                    ListItemData.lastname
                                } else {
                                    ListItemData.firstname + " " + ListItemData.lastname;
                                }
                            }
                        }
                    },
                    ListItemComponent {
                        type: "header"
                        HeaderListItem {
                            title: ListItemData
                        }
                    }
                ]
                
                onTriggered: {
                    // Select the item, this will cause a signal in onSelectionChanged, which
                    // will update the _contentView property that is used to present data.
                    select(indexPath);

                    // Create the content page and push it on top to drill down to it.
                    var page = nav.deprecatedPushQmlByString("QuotePage/QuotePage.qml");
                    nav.push(page);
                }
                
                onSelectionChanged: {
                    if (selected) {
                        // When an item is selected we push the recipe Page in the chosenItem file attribute.
                        var chosenItem = dataModel.data(indexPath);
                        
                        // Make sure there is no undefined strings, this would be presented as "undefined" in the UI.
                        if (chosenItem.firstname == undefined) {
                            chosenItem.firstname = "";
                        }
                        
                        // The _contentView property can be resolved in by the ContentPage since it will
                        // share the same context as the main file.
                        _contentView = chosenItem;
                        console.debug(_contentView.firstname+" "+_contentView.lastname);
                    }
                }
                
                attachedObjects: [
                    // A GroupDataModel is used for presenting the SQL data. All set up is done here
                    // except for the actual population of the model, which is done in C++.
                    GroupDataModel {
                        id: quotesModel
                        grouping: ItemGrouping.ByFirstChar
                        sortingKeys: [ "lastname", "firstname"]
                        objectName: "quotesModel"
                        
                        onItemAdded: {
                            if (addShown) {
                                if (nav.top == quoteListPage) {
                                    
                                    // Reset the selection to assure that the onSelectionChanged signal handler is
                                    // called even if the same indexPath is selected, then select the item and
                                    // scroll to that position in the list.
                                    quotesList.clearSelection();
                                    quotesList.select(indexPath);

                                    // Push the content pane.
						            var page = nav.deprecatedPushQmlByString("QuotePage/QuotePage.qml");
						            nav.push(page);
                                }
                            }
                        }
                        
                        onItemRemoved: {
                            
                            // Setting the correct selection in the list is farily complicated, so that
                            // part of the app logics is taken care of in c++. The only thing that is done
                            // here is navigation away from the Content Page if there are no more items in
                            // the list.
                            var lastIndexPath = last();
                            if (lastIndexPath[0] == undefined) {
                                if (nav.top != quoteListPage) {
                                    nav.popAndDelete();
                                }
                            }
                        }
                        
                        onItemUpdated: {
                            
                            // Update the _contentView property with the new data.
                            var chosenItem = data(indexPath);
                            _contentView = chosenItem;
                        }
                    }
                ]
            }
        }
        
        attachedObjects: [
            Sheet {
                id: addSheet
                // The edit screen is launched when adding a new quote record.
                AddPage {
                    id: add
                    onAddPageClose: {
                        addSheet.visible = false;
                    }
                }
                onVisibleChanged: {
                    add.newQuote();
                }
            }
        ]
        
        actions: [
            ActionItem {
                title: "Add"
                imageSource: "asset:///images/Add.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    addSheet.visible = true;
                    nav.addShown = true;
                }
            }
        ]
    }
    
    onTopChanged: {
        if (page == quoteListPage) {
            quotesList.clearSelection();
        }
    }
}
