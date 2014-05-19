/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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

// Item component for the item list presenting available recipes
CustomListItem {
    id: recipeItem
    property bool active: ListItem.active
    property bool selected: ListItem.selected

    preferredHeight: ui.du(17.6)
    dividerVisible: false
    highlightAppearance: HighlightAppearance.None

    Container {
        topPadding: ui.du(0.2)
        bottomPadding: ui.du(0.6)

        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: itemPaint.imagePaint
        
        layout: DockLayout {
        }

        Container {
            id: highlightContainer
            background: Color.create("#75b5d3")
            opacity: (active || selected) ? 0.9 : 0
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }

        // The Item content an image and a text
        Container {
            leftPadding: ui.du(0.3)
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            ImageView {
                preferredHeight: ui.du(16.8)
                scalingMethod: ScalingMethod.AspectFit
                
                // The image is bound to the data in models/recipemodel.xml image attribute.
                imageSource: ListItemData.image
            }

            Label {
                // The title is bound to the data in models/recipemodel.xml title attribute.
                text: ListItemData.title
                leftMargin: ui.du(3)
                verticalAlignment: VerticalAlignment.Center
                textStyle.base: SystemDefaults.TextStyles.TitleText
                textStyle.color: Color.Black
            }
        }
    }

    attachedObjects: ImagePaintDefinition {
        id: itemPaint
        imageSource: "asset:///images/white_photo.amd"
    }

}
