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

/*
 * This component shows a page with a list from a specified feed
 */
import bb.cascades 1.2
import bb.data 1.0
import com.netimage 1.0
import my.systemDialogs 1.0

Page {
    property alias feedlink: rssSource.source
    property alias title: titleBar.tldrTitle
    property variant chosenItem
    
    property NavigationPane nav: null
    
    titleBar: TldrTitleBar {
        id: titleBar
        scrollBehavior: TitleBarScrollBehavior.NonSticky
    }
    
    Container {
        layout: DockLayout {
        }
        
        // We have the activity indicator here to be able to show something while retrieving the feed.
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            
            // A non sticky title bar will hide the bottom most content, so in order for the
            // activity indicator, which is at the bottom, to be visible padding has to be added.
            // This is a work around that we hope to get rid of eventually.
            bottomPadding: titleBar.layoutHeight
            
            CommonActivityIndicator {
                id: indicator
                indicatorText: qsTr("Loading...") + Retranslate.onLanguageChanged
            }
        }
        
        ListView {
            id: feedsList
            property variant listImageManager: feedImageManager
            dataModel: feedModel
            layout: GridListLayout {
                headerMode: ListHeaderMode.StickyOverlay
                columnCount: 2
                cellAspectRatio: 1
                horizontalCellSpacing: 0
                verticalCellSpacing: 0
            }
            listItemComponents: [
                ListItemComponent {
                    type: "imageItem"
                    ImageItem {
                        contextActions: ActionSet {
                            title: ListItemData.title
                            InvokeActionItem {
                                objectName: "readMore"
                                id: readMore
                                title: qsTr("Read More") + Retranslate.onLanguageChanged
                                ActionBar.placement: ActionBarPlacement.OnBar
                                imageSource: "file:///usr/share/icons/bb_action_open.png"
                                query {
                                    invokeTargetId: "sys.browser"
                                    // A valid URI is needed for the action item to appear.
                                    // We will change the URI when an item is selected.
                                    uri: "http://www.blackberry.com/"	// TODO:: add real link?
                                    // We have to tell it to tell it to watch for updates,
                                    // since the URI is dynamic (see the ListView onTriggered).
                                    onQueryChanged: readMore.query.updateQuery()
                                }
                            }
                            InvokeActionItem {
                                objectName: "shareArticle"
                                id: shareArticle
                                title: qsTr("Share Article") + Retranslate.onLanguageChanged
                                ActionBar.placement: ActionBarPlacement.InOverflow
                                query {
                                    mimeType: "text/plain"
                                    data: "..." // TODO:: add real text and link to share?
                                    invokeActionId: "bb.action.SHARE"
                                }
                                onTriggered: {
                                    // First, convert unicode string before calling invoke; the
                                    // receiver app will have to call decodeURIComponent(escape(str))
                                    // to get the unicode string back. BBM, Browser, etc. all do this.
                                    //
                                    // The replacements here convert any numeric entities in the
                                    // form of &#032; into their proper characters, to have more
                                    // WYSIWYG output when sharing. Note that this doesn't clean
                                    // up HTML entities such as &amp; and &quot; nor does it clean
                                    // up HTML tags such as <a> and <p>.
                                    // Those are exercises for the reader, as Cascades doesn't do it.
                                    data = unescape(encodeURIComponent(chosenItem.title.replace(/&#(\d*);/g, function (m, c) {
                                                    return String.fromCharCode(c)
                                    }) + ": " + chosenItem.description.replace(/&#(\d*);/g, function (m, c) {
                                        return String.fromCharCode(c)
                                    }) + " " + chosenItem.link))
                                }
                            }
                        }
                    }
                },
                ListItemComponent {
                    type: "textItem"
                    TextItem {
                    }
                },
                ListItemComponent {
                    type: "header"
                    Header {
                        id: header
                        property string headerDate : (ListItemData.toDateString() != undefined) ? ListItemData.toDateString() : "" 
                        onHeaderDateChanged: {
                            var today = new Date();
                            var yesterday = new Date(today.getTime()-(1000*60*60*24));
                            if (headerDate == today.toDateString()) {headerDate = "Today"}
                            if (headerDate == yesterday.toDateString()) {headerDate = "Yesterday"}
                        } 
                        title: headerDate
                        contextActions: ActionSet {
                            ActionItem {
                                title: qsTr("To Top") + Retranslate.onLanguageChanged 
                                // TODO:: add icon
                                onTriggered: {
                                    header.ListItem.view.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Smooth)
                                }
                            }
                            ActionItem {
                                title: qsTr("To Bottom") + Retranslate.onLanguageChanged
                                // TODO:: add icon
                                onTriggered: {
                                    header.ListItem.view.scrollToPosition(ScrollPosition.End, ScrollAnimation.Smooth)
                                }
                            }
                        }
                    }
                }
            ]
            onTriggered: {
                // Check so that the triggered item is not a header 
                if (indexPath.length > 1) {
                    
                    // Get the data from the triggered item
                    chosenItem = dataModel.data(indexPath);
                    
                    // Create the drill down page and add populate it with the list item data
                    var page = detailPage.createObject()
                    page.chosenItem = chosenItem
                    page.listImageManager = feedsList.listImageManager
                    
                    nav.push(page)
                }
            }
            
            // Item type-mapping
            function itemType(data, indexPath) {
                if (indexPath.length == 1) {
                    return 'header';
                } else {
                    if (data.imageSource != "") {
                        // The data contain an image return an item that can display an image.
                        return 'imageItem'
                    } else {
                        // No image in the data display a text item.
                        return 'textItem'
                    }
                }
            }
            attachedObjects: [
                // Images from the internet are tracked via a NetImageTracker and it
                // in turn NetImageManager to make requests and cache images. Be careful
                // not to use NetImageManager with many different cacheId:s since the cached
                // images are stored and use up disk space on the device.
                NetImageManager { // TODO:: replace with better image cache
                    id: feedImageManager
                    cacheId: "feedImageManager"
                },
                ComponentDefinition {
                    id: detailPage
                    source: "DetailPage.qml"
                }
            ]
        }
        attachedObjects: [
            GroupDataModel {
                id: feedModel
                sortingKeys: [ "pubDate" ]
                sortedAscending: false
                grouping: ItemGrouping.ByFullValue
            },
            // DataSource that load RSS data see RSSDataSource.qml
            RSSDataSource {	// TODO:: replace with better data model
                id: rssSource
                
                // The GroupDataModel above that is populated with data.
                dataModel: feedModel
                onDataLoaded: {
                    indicator.stopIndicator();
                }
                onError: {
                    indicator.stopIndicator();
                    myQmlDialog.show();
                }
            },
            // System dialog displayed when the feed can not be shown.
            SystemDialog {
                id: myQmlDialog
                title: qsTr("Something bad happened.") + Retranslate.onLanguageChanged
                body: qsTr("There has been an unfortunate error with the download of this RSS feed, perhaps it or the Internet is missing?") + Retranslate.onLanguageChanged
            }
        ]
        onCreationCompleted: {
            indicator.startIndicator();
        }
    }

}
