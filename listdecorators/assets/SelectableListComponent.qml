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

import bb.cascades 1.3

// Custom ListItemComponent that allows for the visual
// representation of the selectableDecorator functionality by
// using the decorator injected "selected" data value.
ListItemComponent {
    type: "selectable"
    Container {
        id: item
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center
        topPadding: ui.du(0.3)
        bottomPadding: ui.du(0.3)
        leftPadding: ui.du(0.6)
        rightPadding: ui.du(0.6)
        
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            background: Color.create(ListItemData.selected)

            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            CoverArt {
            }
            Label {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                
                text: ListItemData.title
                textStyle {
                    base: SystemDefaults.TextStyles.PrimaryText
                }
            }
        }
    }
}