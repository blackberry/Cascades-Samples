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
import "Common"

NavigationPane {
    // The menu is created in the onCreationCompleted signal handler
    // This is not necessary (you can add it directly), we do it like this
    // since it has a SystemToast that will make the QML preview stop previewing.
    property variant menu;
    Menu.definition: menu

    id: nav
    Page {
        id: recipeListPage
        Container {
            layout: DockLayout {
            }

            // A nine-sliced book image is used as background of the cookbook.
            ImageView {
                imageSource: "asset:///images/Book_background.amd"
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }

            // A Container for the list is padded at the top and bottom to make room for decorations.
            Container {
                topPadding: 15
                bottomPadding: topPadding
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
                        // When an item is selected, we push the recipe Page in the chosenItem file attribute.
                        var chosenItem = dataModel.data(indexPath);

                        // Set the correct file source on the ComponentDefinition, create the Page, and set its title.
                        recipePage.source = chosenItem.file;
                        var page = recipePage.createObject();
                        page.title = chosenItem.title;

                        // Push the new Page.
                        nav.push(page);
                    }
                } // ListView
            } // Container
        } // Container
    } // Page
    attachedObjects: [
        ComponentDefinition {
            id: recipePage
            source: "Intro.qml"
        },
        ComponentDefinition {
            id: cookbookMenu
            source: "CookbookMenu.qml"
        }
    ]
    onCreationCompleted: {
        // We want to only display in portrait-mode in this view.
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
        
        // Create the app menu for the cookbook.
        menu = cookbookMenu.createObject();
    }
    onTopChanged: {
        if (page == recipeListPage) {
            // We want to only display in portrait-mode in this view, so if it has been changed, let's reset it.
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
        }
    }
    onPopTransitionEnded: {
        // Transition is done destroy the Page to free up memory.
        page.destroy();
    }
}// NavigationPane
