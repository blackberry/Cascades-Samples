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
 This component shows a page with a list from a specified feed
 */
 
import bb.cascades 1.0
import bb.data 1.0
import com.netimage 1.0
import my.systemDialogs 1.0

Page {
    property alias feedlink: rssSource.source
    property alias title: titleBar.title
    
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
                var chosenItem = dataModel.data(indexPath);
                select(selected(), false);
                select(indexPath, true);
                feedsList.scrollToItem(indexPath, ScrollAnimation.Default);
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
                sortingKeys: ["pubDate"]
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
}
