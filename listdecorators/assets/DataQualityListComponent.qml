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

// Custom ListItemComponent to display the data
// that is provided by the DataQualityDecorator.
ListItemComponent {
    type: "dataQualityDecorated"
    Container {
        id: item
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center

        CoverArt {
        }
        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 5
            }
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center

            text: ListItemData.name
            textStyle {
                base: SystemDefaults.TextStyles.PrimaryText
            }
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            text: "["
            textStyle {
                base: SystemDefaults.TextStyles.PrimaryText
            }
        }
        ImageView {
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///images/" + ListItemData.dataQualityImage
        }
        Label {
            verticalAlignment: VerticalAlignment.Center
            text: "]"
            textStyle {
                base: SystemDefaults.TextStyles.PrimaryText
            }
        }
    }
}