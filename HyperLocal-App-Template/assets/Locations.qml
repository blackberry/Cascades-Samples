/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.system 1.0

Page {

    /* ==================================================
     *      scrollview
     * ==================================================
     */

    titleBar: TitleBar {
        title: "Choose a Location"
    }

    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill

        ListView {
            id: locationsList

            //property alias locationDataModel: locationDataModel

            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Left
            scrollIndicatorMode: ScrollIndicatorMode.Default
            snapMode: SnapMode.None
            flickMode: FlickMode.Momentum

            onCreationCompleted: {
                locationDataModel.insert(0, listViewArray);
            }

            function itemType(data, indexPath) {
                return "item";
            }

            dataModel: ArrayDataModel {
                id: locationDataModel
            }

            onTriggered: {

                if (tab.navPaneAction === "directions") {
                    tab.invokeMap(listViewArray[indexPath]);
                } else if (tab.navPaneAction === "phone") {
                    tab.invokePhone(listViewArray[indexPath]);
                } else if (tab.navPaneAction === "foursquare") {
                    tab.invokeFoursquare(listViewArray[indexPath]);
                }

            }

            listItemComponents: [
                ListItemComponent {
                    type: "item"

                    StandardListItem {
                        textFormat: TextFormat.Plain
                        title: ListItemData.title
                        description: ListItemData.description
                        status: ListItemData.status
                    }
                } // listitemcomponent
            ]
        }
    }
}