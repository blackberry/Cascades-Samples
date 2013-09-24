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

import bb.cascades 1.0
import bb.cascades.datamanager 1.2
import bb.cascades.decorators 1.2

Page {
    // Screen title bar with image
    titleBar: TitleBar {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                leftPadding: 10
                rightPadding: 10

                verticalAlignment: VerticalAlignment.Center

                Label {
                    text: "List Decorators"

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 7
                    }
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.White
                    }
                    verticalAlignment: VerticalAlignment.Center

                }
                ImageView {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    imageSource: "asset:///images/ic_view_list.png"
                }
            }
        }
    }
    //! [0]
    // Each action item loads the appropriate data model and assigns it to the ListView
    actions: [
        // GroupArtistDecorator menu ActionItem with attached object model
        GroupArtistDecorator {
            title: qsTr("GroupArtistModelDecorator")
            imageSource: "asset:///images/list_group.png"
        },
        // GroupStyleDecorator menu ActionItem with attached object model
        GroupStyleDecorator {
            title: qsTr("GroupStyleDataQueryDecorator")
            imageSource: "asset:///images/list_group.png"
        },
        // DataQualityDecorator menu ActionItem with attached object model
        DataQualityDecorator {
            title: qsTr("DataQualityDataQueryDecorator")
            imageSource: "asset:///images/list_quality.png"
        },
        // ExpandableDecorator menu ActionItem with attached object model
        ExpandableDecorator {
            title: qsTr("ExpandableDecorator")
            imageSource: "asset:///images/list_expandable.png"
        },
        // SelectableDecorator menu ActionItem with attached object model
        SelectableDecorator {
            title: qsTr("SelectableDecorator")
            imageSource: "asset:///images/list_selection.png"
        },
        // DataLoggerDecorator menu ActionItem with attached object model
        DataLoggerDecorator {
            title: qsTr("DataLoggerDecorator")
            imageSource: "asset:///images/logging.png"
        },
        // Default menu ActionItem using the default attached asyncDataModel
        ActionItem {
            title: qsTr("Reset")
            imageSource: "asset:///images/list_reset.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                listView.dataModel = defaultModel
            }
        }
    ]
    //! [0]

    Container {
        //! [1]
        // The ListView that's used to display the data using various decorators.
        ListView {
            id: listView

            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            dataModel: defaultModel

            // This function deals with providing a default image
            // when none is provided.
            function imageurl(image) {
                if ("" == image || !_app.fileExists(image)) {
                    return _sqlImagesDir + "../../../native/assets/images/no_image.png"
                    
                }
                return _sqlImagesDir + image;
            }

            onTriggered: {
                clearSelection()
                select(indexPath)
            }

            listItemComponents: [
                // ListComponent's (header and "") to represent default dataModel's
                ListItemComponent {
                    type: "header"
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Center
                        background: Color.create("#0098F0")

                        Label {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            preferredHeight: 200
                            text: ListItemData.header
                        }
                        Container {
                            background: Color.White
                            Divider {
                            }
                        }
                    }
                },
                ListItemComponent {
                    type: ""
                    Container {
                        id: item
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        CoverArt {
                        }
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: ListItemData.name
                            textStyle {
                                base: SystemDefaults.TextStyles.PrimaryText
                            }
                        }
                    }
                },
                // ListComponent definitions to support the associated
                // data that they wish to display.
                GroupArtistListComponent {
                },
                DataQualityListComponent {
                },
                SelectableListComponent {
                }
            ]
            onSelectionChanged: {
                console.log("onSelectionChanged, selected: " + selected)
            }
            onActivationChanged: {
                console.log("onActivationChanged, active: " + active)
            }
        }
        //! [1]
        onCreationCompleted: {
            defaultModel.load()
        }
        
        //! [2]
        attachedObjects: [
            // One of the default provided DataModel's
            AsyncDataModel {
                id: defaultModel
                query: SqlDataQuery {
                    source: _sqlImagesDir + "/../discogs_small.db"
                    query: "select name, primary_image, is_group from artist order by name"
                    countQuery: "select count(*) from artist"
                    onDataChanged: console.log("data changed: revision=" + revision)
                    onError: console.log("SQL query error: " + code + ", " + message)
                }
                onLoaded: console.log("initial model data is loaded")
            }
        ]
        //! [2]
    }
}