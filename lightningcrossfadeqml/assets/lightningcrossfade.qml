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

Page {
    content: Container {
        layout: DockLayout {
        }

        // Setting the background color to a dark grey.
        background: Color.create("#262626");

        // The content (images, slider ...) is stacked in this container.
        Container {
            layout: StackLayout {
            }

            // The content should be centered on screen.
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            // The two images are put on top of each other in this container. And
            // a nice looking image frame is put behind them.
            Container {
                layout: DockLayout {
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }

                // Nice looking frame.
                ImageView {
                    imageSource: "asset:///images/picture_frame.png"
                    preferredWidth: 725
                    preferredHeight: 426
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }

                // This the big image that was taking during the night
                // it's at the same position as the day one, but further from the
                // viewer.
                ImageView {
                    id: night
                    imageSource: "asset:///images/lightning.png"
                    preferredWidth: 689
                    preferredHeight: 391
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }

                // Since this image is on top of the night one, we can hide the
                // night image with changing the opacity value of this image.
                ImageView {
                    id: day
                    imageSource: "asset:///images/sunshine.png"
                    preferredWidth: 689
                    preferredHeight: 391
                    opacity: 0.0
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
            }

            // This is the slider you see at the bottom of the page
            // when it get's a onValueChanging event it changes the
            // image with id night's opacity to that value.
            // Two small images are put on each side of the slider.
            Container {
                layout: StackLayout {
                    layoutDirection: StackLayout.LeftToRight
                    leftPadding: 20
                    rightPadding: 20
                }

                // The slider container is aligend to the center in the content container.
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }

                // The minumum position of the slider means night an image of a
                // moon is added to illustrate this.
                ImageView {
                    imageSource: "asset:///images/moon.png"
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                    }

                }

                Slider {
                    leftMargin: 20
                    rightMargin: 20
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Fill
                        spaceQuota:1
                    }
                    onValueChanging: {
                        // This is where the day image opacity value is done.
                        day.opacity = value;
                    }
                }

                // At the max we add an image of a sun.
                ImageView {
                    imageSource: "asset:///images/sun.png"
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
            }
        }
    }
}
