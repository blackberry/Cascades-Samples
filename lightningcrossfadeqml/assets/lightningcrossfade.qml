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
    // The content (images, slider ...) is stacked in this container.
    Container {
        // Setting the background color to almost white but not quite.
        background: Color.create ("#f8f8f8")
        layout: StackLayout {
            topPadding: 20
        }

        // The two images are put on top of each other in this container.
        Container {
            layout: DockLayout {
            }
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }

            // This the big image that was taking during the night
            // it's at the same position as the day one, but further from the
            // viewer.
            ImageView {
                id: night
                imageSource: "asset:///images/night.jpg"
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }

            // Since this image is on top of the night one, we can hide the
            // night image with changing the opacity value of this image.
            ImageView {
                id: day
                opacity: 0
                imageSource: "asset:///images/day.jpg"
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }

        // This is the slider you see at the bottom of the page
        // when it gets a onValueChanging event it changes the
        // image with id night's opacity to that value.
        // Two small images are put on each side of the slider.
        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
                leftPadding: 20
                rightPadding: 20
                topPadding: 25
                bottomPadding: 25
            }

            // The minimum position of the slider means night an image of a
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
                    spaceQuota: 1
                }
                onValueChanging: {
                    // This is where the day night opacity value is done.
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
