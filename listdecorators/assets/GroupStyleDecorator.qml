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

// ActionItem dealing with the group style query decorator, which
// which groups items that have the same header names under one header.
ActionItem {
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        groupStyleModel.load()
        listView.dataModel = groupStyleModel
    }

    attachedObjects: [
        AsyncHeaderDataModel {
            id: groupStyleModel
            // Decorator that groups all items with the same header name under one header,
            // based on album style (genre).
            query: GroupStyleDataQueryDecorator {
                query: SqlHeaderDataQuery {
                    source: _sqlImagesDir + "/../discogs_small.db"
                    query: "select primary_image, title as name, master_style.style from master inner join master_style on master_style.master_id=master.id order by style"
                    countQuery: "select count(*) from master inner join master_style on master_style.master_id=master.id"
                    headerQuery: "select style as header, count(*) from master_style inner join master on master_style.master_id=master.id group by header"
                    onDataChanged: console.log("data changed: revision=" + revision)
                    onError: console.log("SQL query error: " + code + ", " + message)
                }
            }
            onLoaded: console.log("initial model data is loaded")
        }
    ]
}
