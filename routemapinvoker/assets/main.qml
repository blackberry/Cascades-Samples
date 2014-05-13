/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
import bb.platform 1.0
import bb.data 1.0

Page {
    Container {
        onCreationCompleted: cinemasDataSource.load()

        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: ui.du(3.3)
            topPadding: ui.du(3.3)
            rightPadding: ui.du(3.3)
            bottomPadding: ui.du(3.3)

            // The title label
            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: qsTr ("Route from main station to...")
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    textAlign: TextAlign.Center
                }
            }

            //! [0]
            // The address list view
            ListView {
                topMargin: ui.du(5.6)

                dataModel: cinemasModel

                listItemComponents: ListItemComponent {
                    type: "item"

                    StandardListItem {
                        title: ListItemData.Title
                        description: qsTr ("%1, %2").arg(ListItemData.Address).arg(ListItemData.City)
                    }
                }

                onTriggered: {
                    clearSelection()
                    select(indexPath)

                    routeInvoker.endLatitude = dataModel.data(indexPath).Latitude
                    routeInvoker.endLongitude = dataModel.data(indexPath).Longitude
                }
            }
            //! [0]

            //! [1]
            // The 'Get route' button
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: ui.du(5.6)

                text: qsTr ("Get route")

                onClicked: routeInvoker.go();
            }
            //! [1]
        }
    }

    attachedObjects:[
        //! [2]
        // The wrapper object to invoke the route map
        RouteMapInvoker {
            id: routeInvoker

            // Use fixed coordinates for start position ...
            // Note: This does not work with current BB10 version but will work with future ones
            //startLatitude: 42.814513
            //startLongitude: -73.94206

            // ... and center the map view around them.
            centerLatitude: startLatitude
            centerLongitude: startLongitude
        },
        //! [2]

        //! [3]
        // The data source that loads the cinema addresses
        DataSource {
            id: cinemasDataSource

            source: "cinemas.xml"
            query: "/ResultSet/Result"

            onDataLoaded: {
                cinemasModel.clear()
                cinemasModel.insertList(data)
            }
        },

        // The data model that contains the cinema addresses
        GroupDataModel {
            id: cinemasModel

            grouping: ItemGrouping.None
        }
        //! [3]
    ]
}
