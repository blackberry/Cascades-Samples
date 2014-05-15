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

// Container for organizing the digital good visula display
Container {
    id: digitalGood
    background: digitalGood.ListItem.selected ? Color.create("#5D00FF") : Color.Transparent
    preferredHeight: ui.du(7.3)

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    // A standard Label for displaying item name
    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }

        text: ListItemData.name
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
        }
    }

    // A standard Label for displaying item id
    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }

        text: ListItemData.id
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
        }
    }

    // A standard Label for displaying item sku
    Label {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }

        text: ListItemData.sku
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            color: Color.White
        }
    }
}