/*
 * Copyright (c) 2013 BlackBerry Limited.
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

//! [0]
// Custom ListItemComponent that provides a different visual
// display for group data items which have been identified in
// conjunction with the GroupArtistDecorator.
ListItemComponent {
    type: "group"
    Container {
        id: item
        horizontalAlignment: HorizontalAlignment.Fill

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            background: Color.Black

            CoverArt {
            }
            Container {
                layout: DockLayout {
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Fill
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    imageSource: "asset:///images/grouptag.png"
                    opacity: 0.7
                }
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    leftPadding: 85
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        text: ListItemData.name
                        textStyle {
                            base: SystemDefaults.TextStyles.PrimaryText
                        }
                    }
                }
            }
        }
    }
}
//! [0]