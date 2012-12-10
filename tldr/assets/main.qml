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

NavigationPane {
    id: nav
    Page {
        Container {
            ImageView {
                imageSource: "asset:///images/custom_title.png"
            }
            layout: StackLayout {
            }
            ListView {
                dataModel: XmlDataModel {
                    source: "models/feeds.xml"
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            imageSource: "asset:///images/ca_rss_unread.png"
                            title: ListItemData.title
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
