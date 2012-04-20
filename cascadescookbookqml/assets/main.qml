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

NavigationPane {
    id: nav
    
    Page {
        id: recipeListPage
        
        content: Container {
            background: Color.create ("#262626")
            preferredWidth: 768
            layout: DockLayout {
            }

            // The background of the UI is set up by coloring the background Container,
            // two decoration images at the top and bottom and a paper crease creating the illusion of a book.
            // The paper crease is 1px in height and will be stretched vertically by applying vertical alignment fill.
            ImageView {
                id: topDecoration
                preferredHeight: 15
                imageSource: "asset:///images/red_cloth_edge_top.png"
            }
            ImageView {
                id: bottomDecoration
                preferredHeight: 15
                imageSource: "asset:///images/red_cloth_edge_bottom.png"
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Bottom
                }
            }

            // A Container for the list, padded at the top and bottom to make room for decorations.
            Container {
                layout: DockLayout {
                    topPadding: topDecoration.preferredHeight
                    bottomPadding: bottomDecoration.preferredHeight
                }
                ListView {
                    id: recipeList;
                    dataModel: XmlDataModel {
                        source: "models/recipemodel.xml"
                    }
                                        
                    listItemComponents: [
                        ListItemComponent {
                            type: "recipeitem"
                            RecipeItem {
                            }
                        }
                    ]
                    
                    onSelectionChanged: {
                        if (selected) {
                            // When an item is selected we push the recipe Page in the chosenItem file attribute.   
                            var chosenItem = dataModel.data (indexPath);
                            nav.deprecatedPushQmlByString (chosenItem.file);
                        }
                    }
                }
            }
        }
    }
    onTopChanged: {
        if (pane == recipeListPage) {
            // Clear selection when returning to the recipe list page.
            recipeList.clearSelection ();
        }
    }
}
