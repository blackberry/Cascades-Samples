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

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        //! [0]
        // The button to start the loading of the images
        Button {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            text: qsTr("Load images")

            onClicked: {
                _app.loadImages()
                visible = false
                listView.visible = true
            }
        }
        //! [0]

        //! [1]
        // The ListView that shows the progress of loading and result images
        ListView {
            id: listView

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            visible: false

            dataModel: _app.model
        //! [1]

            //! [2]
            listItemComponents: ListItemComponent {
                type: ""
                Container {
                    preferredHeight: ui.du(55.6)
                    preferredWidth: ui.du(85.3)

                    layout: DockLayout {}

                    // The ActivityIndicator that is only active and visible while the image is loading
                    ActivityIndicator {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        preferredHeight: ui.du(33.3)

                        visible: ListItemData.loading
                        running: ListItemData.loading
                    }

                    // The ImageView that shows the loaded image after loading has finished without error
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill

                        image: ListItemData.image
                        visible: !ListItemData.loading && ListItemData.label == ""
                    }

                    // The Label that shows a possible error message after loading has finished
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        preferredWidth: ui.du(55.6)

                        visible: !ListItemData.loading && !ListItemData.label == ""
                        text: ListItemData.label
                        multiline: true
                    }
                }
            }
            //! [2]
        }
    }
}
