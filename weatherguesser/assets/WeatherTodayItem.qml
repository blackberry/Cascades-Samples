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

// A custom item used in the list on the WeatherPage.
Container {
    id: weatherItem
    layout: DockLayout {
    }

    ImageView {
        imageSource: {
            if (ListItemData.icon == undefined) {
                "asset:///images/icons/big/0.png"
            } else {
                "asset:///images/icons/big/" + ListItemData.icon + ".png"
            }
        }
    }

    Container {
        layout: StackLayout {
            leftPadding: 77
        }

        layoutProperties: DockLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }

        attachedObjects: [
            TextStyleDefinition {
                id: normalStyle
                base: SystemDefaults.TextStyles.TitleText
                size: 60
                color: Color.White
                fontWeight: FontWeight.Normal
            }
        ]

        Container {
            bottomMargin: 0
            layout: StackLayout {
                topPadding: 96
                layoutDirection: LayoutDirection.LeftToRight
            }

            Container {
                layout: StackLayout {
                    // We have to offset the small text with small one since otherwise the bottom alignment would not look good.
                    bottomPadding: 10
                }

                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Bottom
                }

                Label {
                    text: "hi"
                    textStyle {
                        base: normalStyle.style
                    }
                }
            }

            Label {
                leftMargin: 17
                text: ListItemData.temphi + "\u00B0"
                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Bottom
                }

                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    size: 115
                    color: Color.White
                    fontWeight: FontWeight.Normal
                }
            }
        }

        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }

            Label {
                text: "lo"
                textStyle {
                    base: normalStyle.style
                }
            }

            Label {
                leftMargin: 5
                text: ListItemData.templo + "\u00B0"
                textStyle {
                    base: normalStyle.style
                }
            }
        }
    }
}
