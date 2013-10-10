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
    property alias dataModelSource : dataModel.source
    
    Menu.definition: MenuDefinition {
        id: menuDef
        settingsAction: SettingsActionItem {
            onTriggered: {
                // Disable the application menu when navigating to the settings page
                // so that the application menu cannot be invoked from the settings page
                Application.menuEnabled = false

                // Connect the pop-signal to the function that re-activates
                // the application menu
                nav.popTransitionEnded.connect(menuDef.reactivateApplicationMenu)

                // Navigate to the settings page by pushing the page to
                // the NavigationPage
                var page = settingsPage.createObject()
                nav.push(page)
            }
        }

        // Function that reactivates the application menu after the user navigates
        // back from the settings page
        function reactivateApplicationMenu() {
            Application.menuEnabled = true
            nav.popTransitionEnded.disconnect(menuDef.reactivateApplicationMenu)
        }
    }
    Page {

        // A custom title bar is set up using the TldrTitleBar component.
        titleBar: TldrTitleBar {
            tldrTitle: qsTr("Too Long; Didn't Read") + Retranslate.onLanguageChanged
        }

        Container {

            ListView {
                dataModel: XmlDataModel {
                    id: dataModel 
                }

                listItemComponents: [
                    ListItemComponent {
                        type: "item"

                        FeedListItem {
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
                    page.nav = nav

                    nav.push(page);
                }
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: feedPage
            source: "FeedPage.qml"
        },
        ComponentDefinition {
            id: settingsPage
            source: "SettingsPage.qml"
        }
    ]

    onPopTransitionEnded: {
        // Transition is done destroy the Page to free up memory.
        page.destroy();
    }
}
