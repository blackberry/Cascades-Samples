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

//! [0]
// ActionItem that deals with the Data Quality decorator,
// which decorates the list data with quality icons based 
// on the quality field value.
ActionItem {
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        dataQualityModel.load()
        listView.dataModel = dataQualityDecorator
    }

    attachedObjects: [
        // DataQualityModelDecorator changes dataType in order to
        // use the custom ListItemComponent to display this data.
        DataQualityModelDecorator {
            id: dataQualityDecorator
            model: 
            AsyncDataModel {
                id: dataQualityModel
                query: DataQualityDataQueryDecorator {
                    query: SqlDataQuery {
                        source: _sqlImagesDir + "/../discogs_small.db"
                        query: "select name, data_quality,primary_image from artist"
                        countQuery: "select count(*) from artist"
                        onDataChanged: console.log("data changed: revision=" + revision)
                        onError: console.log("SQL query error: " + code + ", " + message)
                    }
                }
                onLoaded: console.log("initial model data is loaded")
            }
        }
    ]
}
//! [0]