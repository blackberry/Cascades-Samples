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
import bb.cascades 1.2

NavigationPane {
    id: nav
    Page {

        // A custom title bar is set up using the TldrTitleBar component.
        titleBar: TldrTitleBar {
            tldrTitle: qsTr("Too Long; Didn't Read") + Retranslate.onLanguageChanged
        }

        Container {

            ListView {
                dataModel: XmlDataModel {
                    source: "models/feeds.xml"
                }

                listItemComponents: [

                    ListItemComponent {
                        type: "item"
                        
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

                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath);

                    // Create the content page and push it on top to drill down to it.
                    var page = feedPage.createObject();

                    // Set the title and source of the feed that the user selected. 
                    page.feedlink = chosenItem.feedlink
                    page.title = chosenItem.title
                    
                    nav.push(page);
                }
            }
        }        
    }
    attachedObjects: [
        ComponentDefinition {
            id: feedPage
            source: "FeedPage.qml"
        }
    ]
    
    onPopTransitionEnded: {
        // Transition is done destroy the Page to free up memory.
        page.destroy();
    }
}
