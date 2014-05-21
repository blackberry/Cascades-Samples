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

// The application cover, simply shows the current topic.
Container {
    background: Color.create("#171113")
    topPadding: ui.du(2)
    leftPadding: topPadding
    rightPadding: topPadding
    
    layout: DockLayout {
    }

    // The rundgang icon in the bottom right corner.
    ImageView {
        imageSource: "asset:///images/cover_icon.png"
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Bottom
    }

    // The current topic is set when the application enters thumbnail mode (see the updateData function)
    Label {
        id: appCoverText
        text: "#RUNDGÅNG"
        multiline: true
        autoSize {
            maxLineCount: 2
        }
        textStyle {
            color: Color.create("#fefefe")
            base: SystemDefaults.TextStyles.TitleText
        }
    }

    attachedObjects: [
        CustomSqlDataSource {
            // A data source for accessing the topics data base and get the
            // latest modified topic (the currently selected topic).
            id: asynkDataSource
            source: "models/rundgang.db"
            query: "SELECT * FROM topics ORDER BY date DESC LIMIT 1 "
            connection: "app_cover_connection"

            onDataLoaded: {
                if (data.length > 0) {
                    appCoverText.text = data[0].topic;
                }
            }
        }
    ]

    onCreationCompleted: {
        // Connect to the Application thumbnail signal in order to update
        // the data shown on the cover when the app is minimized.
        Application.thumbnail.connect(updateData);
    }

    function updateData() {
        // Load the data, this will trigger the data source query.
        asynkDataSource.load();
    }
}