/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2
import "Common"

// This recipe shows how to set up Labels with different system fonts.
RecipePage {
    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            // All the available header type fonts are stacked in a Container.
            Container {
                rightMargin: 60
                Label {
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "BigText"
                    textStyle.base: textStyleBoldBig.style
                }
                Label {
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "TitleText"
                    textStyle.base: textStyleBoldTitle.style
                }
                Label {
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "BodyText"
                    textStyle.base: textStyleBoldBody.style
                }
                Label {
                    bottomMargin: 50
                    horizontalAlignment: HorizontalAlignment.Right
                    text: "SubtitleText"
                    textStyle.base: textStyleBoldSubtitle.style
                }
                Label {
                    bottomMargin: 50
                    text: "SmallText"
                    horizontalAlignment: HorizontalAlignment.Right
                    textStyle.base: textStyleBoldSmall.style
                }
            } // Container

            // The second stacked Container shows the Paragraph style fonts.
            Container {
                Label {
                    bottomMargin: 50
                    text: "BigText"
                    textStyle.base: SystemDefaults.TextStyles.BigText
                }
                Label {
                    bottomMargin: 50
                    text: "TitleText"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                }
                Label {
                    bottomMargin: 50
                    text: "BodyText"
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }
                Label {
                    bottomMargin: 50
                    text: "SubtitleText"
                    textStyle.base: SystemDefaults.TextStyles.SubtitleText
                }
                Label {
                    bottomMargin: 50
                    text: "SmallText"
                    textStyle.base: SystemDefaults.TextStyles.SmallText
                }
            } // second Container
        } // main Container
    } // RecipeContainer
    
    attachedObjects: [
        TextStyleDefinition {
            id: textStyleBoldBig
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Bold
        },
        TextStyleDefinition {
            id: textStyleBoldTitle
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Bold
        },
        TextStyleDefinition {
            id: textStyleBoldBody
            base: SystemDefaults.TextStyles.BodyText
            fontWeight: FontWeight.Bold
        },
        TextStyleDefinition {
            id: textStyleBoldSubtitle
            base: SystemDefaults.TextStyles.SubtitleText
            fontWeight: FontWeight.Bold
        },
        TextStyleDefinition {
            id: textStyleBoldSmall
            base: SystemDefaults.TextStyles.SmallText
            fontWeight: FontWeight.Bold
        }
    ]
}// RecipePage
