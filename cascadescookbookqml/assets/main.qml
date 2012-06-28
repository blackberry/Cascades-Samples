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
        Container {
            layout: DockLayout {
            }
            
            // A nine-sliced book image is used as background of the cookbook.
            ImageView {
                imageSource: "asset:///images/Book_background.png"
                
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }

            // A Container for the list, padded at the top and bottom to make room for decorations.
            Container {
                layout: DockLayout {
                    topPadding: 15
                    bottomPadding: topPadding
                }

                ListView {
                    id: recipeList
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
                    onTriggered: {
                        // When an item is selected we push the recipe Page in the chosenItem file attribute.
                        var chosenItem = dataModel.data(indexPath);
                        var recipePage = nav.deprecatedPushQmlByString(chosenItem.file);
                        recipePage.title = chosenItem.title;
                    }
                }
            }
        }
    }
    onCreationCompleted: {
        // We want to only display in portrait in this view. 
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
    }
    onTopChanged: {
        if (page == recipeListPage) {
            // Clear selection when returning to the recipe list page.
            recipeList.clearSelection ();
            // We want to only display in portrait in this view, so if it has been changed, let's reset it. 
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
        }
    }
}
