import bb.cascades 1.2

CustomListItem {
    dividerVisible: true
    highlightAppearance: HighlightAppearance.Default
    
    Container {
        id: contentContainer
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        
        ImageView {
            id: imageViewID
            imageSource: "asset:///images/ca_rss_unread.png"
            verticalAlignment: VerticalAlignment.Center
            accessibilityMode: A11yMode.Collapsed
        }
        
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            Label {
                id: itemText
                text: ListItemData.title
                verticalAlignment: VerticalAlignment.Center
                textStyle.base: SystemDefaults.TextStyles.TitleText
                accessibilityMode: A11yMode.Collapsed
            }
            Label {
                text: ListItemData.feedlink
                textStyle.fontSize: FontSize.XXSmall
                accessibilityMode: A11yMode.Collapsed
            }
            accessibility: CustomA11yObject {
                role: A11yRole.ListItem
                labelledBy: itemText
                
                ComponentA11ySpecialization {
                    onActivationRequested: {
                        if (event.type == A11yComponentActivationType.Release) {
                            contentContainer.ListItem.view.triggered(contentContainer.ListItem.indexPath)
                        }
                    }
                }
            }
        }
        contextActions: ActionSet {
            DeleteActionItem {
            
            }
        }
    
    }
}