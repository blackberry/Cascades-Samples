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
 * Datsource component is for the RSS list
 */
 
 import bb.cascades 1.0
 import bb.data 1.0

// A neat trick is that we can load data from a RSS feed by specifying a query, and then we get XML.

DataSource {
    id: ds
    property variant dataModel
    
    // Set up a query to request the items in the RSS xml file
    query: "/rss/channel/item"
    
    onDataLoaded: {
        // When data is recieved clear the data model that is going to be populated
        dataModel.clear();
        
        // The data need some preparation so we create an array for holding the modified data.
        var tempdata = new Array();
        for (var i = 0; i < data.length; i ++) {
            
            // Create a Date object for the publication Date (for easier presentation)
            var newDate = new Date(Date.parse(data[i].pubDate, "dd mmmm yyyy h:MM:ss o"));
            newDate.setHours(0);
            newDate.setMinutes(0);
            newDate.setSeconds(0);
            
            // Set the data to temp data, we need to do this in order to modify the vaules
            // it is not possible to directly change the data array.
            tempdata[i] = data[i]
            
            // Set the date
            tempdata[i].pubDate = newDate;
            
            // An invokable function in the C++ app code is used to parse the
            // data and look for an image that can be presented together with the text. 
            tempdata[i].imageSource = tldrApp.findImage(data[i]);
            
            // Some RSS texts contain tags of different kinds, so we call an invokable C++ 
            // funciton that clean up the description 
            tempdata[i].description = tldrApp.plainText(data[i].description);
        }
        
        // Finally insert the data in the dataModel and it will be presented in the list.
        dataModel.insertList(tempdata)
    }
    
    onError: {
        console.log("rss channel load error[" + errorType + "]: " + errorMessage)
    }
    
    onSourceChanged: {
        dataModel.clear();
        ds.load();
    }
}
