/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

// The rundgang application menu, accessible everywhere int the app
// except when on one of the pages launched by the menu itself.
MenuDefinition {
    id: menuDef

    // The application Menu needs a NavigationPane to push pages to,
    // this has to be ensured by the root control.
    property variant navigationPane

    settingsAction: SettingsActionItem {
        onTriggered: {
            // Show the settings page.
            menuDef.showAppPage(settingsdef.createObject())
        }
    }
    helpAction: HelpActionItem {
        onTriggered: {
            // Show the help page.
            menuDef.showAppPage(helpdef.createObject())
        }
    }

    attachedObjects: [
        ComponentDefinition {
            id: settingsdef
            source: "asset:///Common/SettingsPage.qml"
        },
        ComponentDefinition {
            id: helpdef
            source: "asset:///Common/HelpPage.qml"
        }
    ]

    function showAppPage(page) {
        // Push the menu page and disable the menu while it is shown.
        navigationPane.push(page);
        Application.menuEnabled = false;

        // Connect to the pop transition ended that will destroy the
        // menu page and make the menu available again.
        navigationPane.popTransitionEnded.connect(menuDef.destroyAppPage);
        navigationPane.popTransitionEnded.connect(page.destroy);
    }

    function destroyAppPage(page) {
        // Once navigation away from the page is done enable the app menu and destroy the Page.
        Application.menuEnabled = true;
        navigationPane.popTransitionEnded.disconnect(menuDef.destroyAppPage);
    }
}
