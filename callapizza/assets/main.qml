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
import bb.system.phone 1.0
import bb.data 1.0

Page {
    Container {
        id: root

//! [0]
        // A helper function to avoid code duplication
        function startSearch()
        {
            showListViewAnimation.play()

            _pizzeriaSearcher.zipCode = zipCodeField.text
        }
//! [0]

        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        // The 'Find My Pizza' logo
        ImageView {
            id: logoImage

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            translationY: 50

            imageSource: "asset:///images/logo.png"
        }

        // The search area
        Container {
            id: searchArea

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            ImageView {
                id: upperHintImage

                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                imageSource: "asset:///images/upper_hint.png"

                visible: !zipCodeField.focused
            }

            Container {
                layout: DockLayout {}


                ImageView {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    imageSource: "asset:///images/searcharea.png"
                }

//! [1]
                TextField {
                    id: zipCodeField

                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    preferredWidth: 530
                    preferredHeight: 65

                    hintText: ""

                    input {
                        submitKey: SubmitKey.Search

                        onSubmitted: root.startSearch()
                    }
                }

                ImageButton {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center

                    translationX: -60

                    defaultImageSource: "asset:///images/find_button.png"
                    pressedImageSource: "asset:///images/find_button.png"

                    onClicked: root.startSearch()
                }
//! [1]
            }

            ImageView {
                id: lowerHintImage

                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom

                imageSource: "asset:///images/lower_hint.png"

                visible: !zipCodeField.focused
            }
        }

        Container {
            id: resultView

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            translationY: 1280

            layout: DockLayout {}

            // The background image of the list view
            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Bottom

                imageSource: "asset:///images/listview_background.png"
            }

//! [2]
            ListView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Bottom

                preferredHeight: 1030

                dataModel: _pizzeriaSearcher.model

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        PizzeriaItem {
                            title: ListItemData.Title
                            description: ListItemData.Phone
                        }
                    }
                ]

                onTriggered: {
                    phone.requestDialpad(dataModel.data(indexPath).Phone)
                }
            }
//! [2]

            onTouch: {
                if (event.isDown() && event.localY < 250)
                    hideListViewAnimation.play()
            }
        }

        animations: [
            ParallelAnimation {
                id: showListViewAnimation

                SequentialAnimation {
                    target: logoImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 1.0
                        toOpacity: 0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: upperHintImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 1.0
                        toOpacity: 0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: lowerHintImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 1.0
                        toOpacity: 0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: searchArea
                    TranslateTransition {
                        duration: 500
                        fromY: 0
                        toY: -1000
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: resultView
                    TranslateTransition {
                        duration: 500
                        fromY: 1280
                        toY: 0
                        easingCurve: StockCurve.Linear
                    }
                }
            },
            ParallelAnimation {
                id: hideListViewAnimation

                SequentialAnimation {
                    target: logoImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 0
                        toOpacity: 1.0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: upperHintImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 0
                        toOpacity: 1.0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: lowerHintImage
                    FadeTransition {
                        duration: 500
                        fromOpacity: 0
                        toOpacity: 1.0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: searchArea
                    TranslateTransition {
                        duration: 500
                        fromY: -1000
                        toY: 0
                        easingCurve: StockCurve.Linear
                    }
                }
                SequentialAnimation {
                    target: resultView
                    TranslateTransition {
                        duration: 500
                        fromY: 0
                        toY: 1280
                        easingCurve: StockCurve.Linear
                    }
                }
            }
        ]
    }

//! [3]
    attachedObjects: [
        Phone {
            id: phone
        }
    ]
//! [3]
}
