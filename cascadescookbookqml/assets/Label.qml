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
import "Common"

// Show how to set up Labels with different system fonts.
Page {
    content: RecipeContainer {
        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            // Here all the available header type fonts are stacked in a Container.
            Container {
                rightMargin: 60
                Label {
                    bottomMargin: 50
                    text: "BigText"
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        fontWeight: FontWeight.Bold
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "TitleText"
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        fontWeight: FontWeight.Bold
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "BodyText"
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        fontWeight: FontWeight.Bold
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "SubtitleText"
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        fontWeight: FontWeight.Bold
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "SmallText"
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        fontWeight: FontWeight.Bold
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Right
                    }
                }
            }

            // The second stacked Container show the Paragraph style fonts.
            Container {
                Label {
                    bottomMargin: 50
                    text: "BigText"
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "TitleText"
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "BodyText"
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "SubtitleText"
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                    }
                }
                Label {
                    bottomMargin: 50
                    text: "SmallText"
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                    }
                }
            }
        }
    }
}
