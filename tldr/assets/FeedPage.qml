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
import bb.cascades 1.0
import bb.data 1.0
import com.netimage 1.0
import my.systemDialogs 1.0

Page {
    property alias feedlink: rssSource.source
    property alias title: titleBar.title
    property variant chosenItem
    titleBar: TitleBar {
        id: titleBar
    }
    Container {
        layout: DockLayout {
        }

        //We have the activity indicator here to be able to show something while retrieving the feed
        ActivityIndicator {
            id: dataLoadIndicator
            preferredWidth: 400
            preferredHeight: 400
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
        }
        ListView {
            id: feedsList
            property variant listImageManager: feedImageManager
            dataModel: feedModel
            layout: StackListLayout {
                headerMode: ListHeaderMode.Sticky
            }
            listItemComponents: [
                ListItemComponent {
                    type: "imageItem"
                    ImageItem {
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
                        title: if (ListItemData.toDateString() != undefined) ListItemData.toDateString()
                    }
                }
            ]
            onTriggered: {
                chosenItem = dataModel.data(indexPath);
                select(selected(), false);
                select(indexPath, true);
                feedsList.scrollToItem(indexPath, ScrollAnimation.Default);
                readMore.enabled = true;
                // We can either update the URI here or in the action item's onTriggered.
                readMore.query.uri = chosenItem.link;
                shareArticle.enabled = true;
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
                NetImageManager {
                    id: feedImageManager
                    cacheId: "feedImageManager"
                }
            ]
        }
        attachedObjects: [
            GroupDataModel {
                id: feedModel
                sortingKeys: [ "pubDate"]
                sortedAscending: false
                grouping: ItemGrouping.ByFullValue
            },
            // DataSource that load RSS data see RSSDataSource.qml
            RSSDataSource {
                id: rssSource

                // The GroupDataModel above that is populated with data.
                dataModel: feedModel
                onDataLoaded: {
                    dataLoadIndicator.stop();
                }
                onError: {
                    dataLoadIndicator.stop();
                    myQmlDialog.show();
                }
            },
            // System dialog displayed when the feed can not be shown.
            SystemDialog {
                id: myQmlDialog
                title: "Something bad happened."
                body: "There has been an unfortunate error with the download of this RSS feed, perhaps it or the Internet is missing?"
            }
        ]
        onCreationCompleted: {
            dataLoadIndicator.start();
        }
    }
    actions: [
        InvokeActionItem {
            objectName: "readMore"
            id: readMore
            title: "Read More"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "file:///usr/share/icons/bb_action_open.png"
            query {
                invokeTargetId: "sys.browser"
                // A valid URI is needed for the action item to appear.
                // We will change the URI when an item is selected.
                uri: "http://www.blackberry.com/"
                // We have to tell it to tell it to watch for updates,
                // since the URI is dynamic (see the ListView onTriggered).
                onQueryChanged: readMore.query.updateQuery()
            }
            onCreationCompleted: {
                // The action item will be enabled once a list item is selected.
                // See the ListView's onTriggered.
                readMore.enabled = false;
            }
        },
        InvokeActionItem {
            objectName: "shareArticle"
            id: shareArticle
            title: "Share Article"
            ActionBar.placement: ActionBarPlacement.InOverflow
            query {
                mimeType: "text/plain"
                data: "I tried sharing something with you, but it wasn't selected when I hit the Share button."
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
            onCreationCompleted: {
                // The action item will be enabled once a list item is selected.
                // See the ListView's onTriggered.
                shareArticle.enabled = false;
            }
        }
    ]
}
