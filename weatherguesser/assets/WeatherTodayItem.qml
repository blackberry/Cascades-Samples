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

// A custom item used in the list on the WeatherPage for Today item

Container {
    id: weatherItem
    layout: DockLayout {
    }

    // Icon image
    ImageView {
        imageSource: "asset:///images/icons/big/" + ListItemData.icon + ".png"
    }
    
    Container {
        leftPadding: 77
        topPadding: 110
        
        Container {
            bottomMargin: 0
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            // High temp Container
            Container {
                bottomPadding: 28
                verticalAlignment: VerticalAlignment.Bottom

                // Add bottom padding so the bottom layout won't be difficult to see.
                Label {
                    text: "hi"
                    textStyle.base: weatherItem.ListItem.view.itemBigTextNormalWhite.style
                }
            }
            Label {
                leftMargin: 17
                text: ListItemData.temphi + "\u00B0"
                verticalAlignment: VerticalAlignment.Bottom
                textStyle.base: weatherItem.ListItem.view.itemNormalWhiteLargeFont.style
            } // Label
        } // Container

        // Low temp Container
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Container {
                bottomPadding: 8
                verticalAlignment: VerticalAlignment.Bottom
                
                Label {
                    text: "lo"
                    textStyle.base: weatherItem.ListItem.view.itemBigTextNormalWhite.style                
                }
            }
            Label {
                leftMargin: 25
                text: ListItemData.templo + "\u00B0"
                textStyle.base: weatherItem.ListItem.view.itemBigTextNormalWhite.style
            }
        } // Low temp Container
    } // Container
} // Container
