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
import bb.cascades.maps 1.0
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

//! [0]
        // The list view with the cinema addresses
        ListView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: 30
            topPadding: 50
            rightPadding: 30
            bottomPadding: 30

            dataModel: cinemasModel

            listItemComponents: ListItemComponent {
                type: "item"

                StandardListItem {
                    title: ListItemData.Title
                    description: qsTr("%1, %2").arg(ListItemData.Address).arg(ListItemData.City)
                }
            }

            onTriggered: {
                // Configure the map view with the selected location
                mapView.latitude = dataModel.data(indexPath).Latitude
                mapView.longitude = dataModel.data(indexPath).Longitude
                mapView.altitude = 10000

                // Update the content of the address label
                addressLabel.text = qsTr("%1, %2").arg(dataModel.data(indexPath).Address)
                                                   .arg(dataModel.data(indexPath).City)

                // Show the sheet with the map view
                mapSheet.open()
            }
        }
//! [0]
    }

    attachedObjects: [
//! [1]
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
        },
//! [1]

//! [2]
        // The sheet that contains the map view
        Sheet {
            id: mapSheet

            Page {
                Container {
                    background: Color.Black

                    // The address label
                    Label {
                        id: addressLabel

                        horizontalAlignment: HorizontalAlignment.Center

                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                            color: Color.White
                            textAlign: TextAlign.Center
                        }
                    }

                    // The map view
                    MapView {
                        id: mapView

                        horizontalAlignment: HorizontalAlignment.Fill

                        // Enforce the usage of the 3D rendering engine
                        onCreationCompleted: setRenderEngine("RenderEngine3D")
                    }

                    // The 'Back' button
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center

                        text: qsTr ("Back")

                        onClicked: mapSheet.close()
                    }
                }
            }
        }
//! [2]
    ]
}