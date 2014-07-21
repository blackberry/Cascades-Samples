/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import bb.cascades 1.3
import "Common"
import "cover"

NavigationPane {
    id: nav

    // The menu is created in the onCreationCompleted signal handler
    // This is not necessary (you can add it directly), we do it like this
    // since it has a SystemToast that will make the QML preview stop previewing.
    property variant menu;
    Menu.definition: menu

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
                topPadding: ui.px(15) 
                bottomPadding: topPadding
                ListView {
                    id: recipeList
                    scrollRole: ScrollRole.Main
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
                }
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: recipePage
            source: "Intro.qml"
        },
        ComponentDefinition {
            id: cookbookMenu
            source: "CookbookMenu.qml"
        },
        MultiCover {
            id: multi
            SceneCover {
                MultiCover.level: CoverDetailLevel.High
                // Check AppCover.qml
                content: AppCover {
                }
            }
            SceneCover {
                MultiCover.level: CoverDetailLevel.Medium
                content: Container {
                    layout: DockLayout {}
                    background: Color.create("#060606")
                    
                    ImageView {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        maxHeight: ui.px(150) 
                        imageSource: "asset:///images/active_frames_pepper.png"
                        scalingMethod: ScalingMethod.AspectFit
                    }
                    
                    AppCoverHeader {
                        title: "QML"
                        backgroundOpacity: 0.5
                        headerColor: "#36412d"
                    }  
            
                }
            }
        }  
    ]
    
    onCreationCompleted: {
        // We want to only display in portrait-mode in this view.
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
        
        // Create the app menu for the cookbook.
        menu = cookbookMenu.createObject();
        
        // Set the scene cover for the app.
        Application.setCover(multi);
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
}
