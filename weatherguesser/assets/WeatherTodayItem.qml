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
        preferredWidth: 768
        preferredHeight: 450
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
                layoutDirection: StackLayout.LeftToRight
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
                layoutDirection: StackLayout.LeftToRight
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
