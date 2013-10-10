import bb.cascades 1.2

CustomListItem {
    dividerVisible: false
    highlightAppearance: HighlightAppearance.Default
    id: listItemRoot

    Container {
        id: contentContainer
        topPadding: 5
        horizontalAlignment: HorizontalAlignment.Fill
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.create("#333333")

            ImageView {
                id: imageViewID
                imageSource: "asset:///images/ca_rss_unread.png"
                verticalAlignment: VerticalAlignment.Center
                scalingMethod: ScalingMethod.AspectFit
                accessibilityMode: A11yMode.Collapsed
            }

            Label {
                id: itemText
                text: ListItemData.title
                verticalAlignment: VerticalAlignment.Center
                textStyle.base: SystemDefaults.TextStyles.TitleText
                accessibilityMode: A11yMode.Collapsed

            }

            accessibility: CustomA11yObject {
                role: A11yRole.ListItem
                labelledBy: itemText
                description: itemText.text

                ComponentA11ySpecialization {
                    onActivationRequested: {
                        if (event.type == A11yComponentActivationType.Release) {
                            contentContainer.ListItem.view.triggered(contentContainer.ListItem.indexPath)
                        }
                    }
                }

            }
        }

    }

}