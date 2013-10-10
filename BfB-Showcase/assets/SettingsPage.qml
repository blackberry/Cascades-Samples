import bb.cascades 1.2

Page {
    id: myPage
    // A custom title bar is set up using the TldrTitleBar component.
    titleBar: TldrTitleBar {
        tldrTitle: qsTr("Settings") + Retranslate.onLanguageChanged
    }
    
    Container {
        topPadding: 10
        Container {
            id: addFeedContainer
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            
            }
            TextField {
                id: feedUrl
                hintText: qsTr("Add feed url") + Retranslate.onLanguageChanged
                inputMode: TextFieldInputMode.Url
                clearButtonVisible: true
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                input {
                    submitKeyFocusBehavior: SubmitKeyFocusBehavior.Lose
                    submitKey: SubmitKey.Submit
                    onSubmitted: {
                        addFeedContainer.addFeed();
                    }
                }
            
            }
            Button {
                text: qsTr("Add") + Retranslate.onLanguageChanged
                preferredWidth: feedUrl.focused ? 20 : undefined
                onClicked: {
                    addFeedContainer.addFeed();
                }
            }
            function addFeed() {
                // TODO:: add the feed
                
                // Reset the TextField to its original state
                feedUrl.loseFocus()
                feedUrl.text = ""
            }
        }
        ListView {
            topMargin: 50
            dataModel: XmlDataModel {
                source: "models/feeds.xml"
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    SettingsListItem {
                    }
                }
            ]
        }
    }
}
