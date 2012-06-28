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

    Page {
        id: quoteListPage
        Container {
            background: Color.create("#f8f8f8")

            ListView {
                objectName: "quotesList"
                id: quotesList

                layout: StackListLayout {
                    headerMode : ListHeaderMode.Sticky
                 }

                // An XML model can be used to prototype the UI until the SQL model is in place
                //dataModel: XmlDataModel { source:"models/quotes.xml" }

                // There are two types of item, header for alphabetic groups and standard items
                // showing the person name.
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            reserveImageSpace: false
                            title: {
                                // Some entries only have a last name (we enforce last name always being added)
                                if(ListItemData.firstname == undefined) {
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
                            title: ListItemData;
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
                }
            }
        ]
    }

    onTopChanged: {
        if (page == quoteListPage) {
            quotesList.clearSelection ();
        }
    }
}
