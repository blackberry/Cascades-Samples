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

import bb.cascades 1.0
import custom.lib 1.0

Page {
    content: Container {
        background: Color.create("#272727");
        layout: DockLayout {
        }
        
        ListView {
			id: myList
            objectName: "myList"
            layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
            }
            dataModel: FilteredDataModel {
            }
            
            listItemComponents: [
                // Standard header height is too short to be selectable.
                // Use a larger font for the header 
                ListItemComponent {
                    type: "header"
                    Label {
                        text: ListItemData
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                            fontWeight: FontWeight.Bold
                            color: Color.White
                      }
                  }
              }
              // Default StandardListItem for "item" is fine
           ]
            
            // Treat outer level as header, inner level as item
            function itemType(data, indexPath) {
                if (indexPath.length == 1)
                {
                    return "header";
                }
                return "item";
            }
        }
    }
}
