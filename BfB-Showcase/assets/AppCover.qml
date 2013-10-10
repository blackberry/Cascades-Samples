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

Container {
    id: root
    background: Color.Black
    layout: DockLayout {
    }

    onCreationCompleted: {
        // TODO: Should show some kind of aggregated feed instead
        rssSource.source = "http://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml"
        indicator.startIndicator();
        activeFrame.updateChanged.connect(root.refresh);
    }
    
    function refresh() {
        // TODO: Temporary solution, should be data-driven from c++ instead
        rssSource.refresh();        
    }

    // Main content
    ImageView {
        id: image
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        scalingMethod: ScalingMethod.AspectFill
        image: tracker.image

        attachedObjects: [
            NetImageTracker {
                id: tracker
                manager: feedImageManager
            }
        ]
    }
    ImageView {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        imageSource: "asset:///images/gradient.png"
    }
    Container {
        leftPadding: 20
        rightPadding: leftPadding
        bottomPadding: leftPadding
        verticalAlignment: VerticalAlignment.Bottom
	    DSLabel {
	        id: title
	        maxLineCount: 4
	    }
    }

    // We have the activity indicator here to be able to show something while retrieving the feed.
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        CommonActivityIndicator {
            id: indicator
            indicatorText: qsTr("Loading...") + Retranslate.onLanguageChanged
        }
    }

    // Container displayed when the feed can not be shown.
    Container {
        id: myQmlDialog
        background: Color.Black
        visible: false
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        //background: Color.Black
        ImageView {
            imageSource: "asset:///images/ic_connections.png"
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            text: qsTr("Download of this RSS feed failed, not connected to Internet?") + Retranslate.onLanguageChanged
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.textAlign: TextAlign.Center
            multiline: true
        }
    }

    attachedObjects: [
        // Images from the internet are tracked via a NetImageTracker and it
        // in turn NetImageManager to make requests and cache images. Be careful
        // not to use NetImageManager with many different cacheId:s since the cached
        // images are stored and use up disk space on the device.
        NetImageManager { // TODO:: replace with better image cache
            id: feedImageManager
            cacheId: "feedImageManagerCover"
        },
        GroupDataModel {
            id: feedModel
            sortingKeys: [ "pubDate" ]
            sortedAscending: false
            grouping: ItemGrouping.ByFullValue
        },
        // DataSource that load RSS data see RSSDataSource.qml
        RSSDataSource { // TODO:: replace with better data model
            id: rssSource

            // The GroupDataModel above that is populated with data.
            dataModel: feedModel
            onDataLoaded: {
                indicator.stopIndicator();
                tracker.source = feedModel.data([ 0, 0 ]).imageSource;
                title.text = feedModel.data([ 0, 0 ]).title;
            }
            onError: {
                indicator.stopIndicator();
                myQmlDialog.visible = true;
            }
        }
    ]

}
