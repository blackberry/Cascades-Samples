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

// ActionItem that deals with ExpandableDataModel decorator,
// which allows the list to expand or hide, based on a custom
// field value that is injected into the data set by the decorator.
ActionItem {
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        expandableModel.load()
        listView.dataModel = expandableDecorator
        listView.selectionChanged.connect(expandableDecorator.expandHeader)
    }

    attachedObjects: [
        // Decorator for allowing the expansion/contraction of header items.
        ExpandableDataModelDecorator {
            id: expandableDecorator
            model: AsyncHeaderDataModel {
                id: expandableModel
                query: SqlHeaderDataQuery {
                    source: _sqlImagesDir + "/../discogs_small.db"
                    query: "select title as name, primary_image, master_genre.genre from master_genre, master where master_genre.master_id=master.id order by genre"
                    countQuery: "select count(*) from master_genre, master where master_genre.master_id=master.id"
                    headerQuery: "select master_genre.genre as header, count(*) from master_genre, master where master_genre.master_id=master.id group by header"
                    onDataChanged: console.log("data changed: revision=" + revision)
                    onError: console.log("SQL query error: " + code + ", " + message)
                }
            }
        }
    ]
}
