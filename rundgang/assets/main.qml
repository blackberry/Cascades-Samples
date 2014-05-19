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
import bb.system 1.2
import "Rundgang"
import "History"
import "Common"
import "models"

// The root Control of the app is a TabbedPane with two tabs:
// The rundgang tab where the feedback topic can be managed and 
// actions to collect data that is to be fed back either in the 
// form of an image, a sound or a text, can be taken. The second
// tab contain the history of items that has been sent.  
TabbedPane {
    id: rundgangTabbedPane
    
    // The application menu that can be opened anywhere in the app.
    Menu.definition: AppMenu {
        id: appMenu
        navigationPane: activePane
    }
    
    // The rundgang tab from which feedback is set up and sent.
    Tab {
        imageSource: "asset:///images/ic_rundgang.png"
        title: qsTr("Rundgång") + Retranslate.onLocaleOrLanguageChanged
        
        delegate: Delegate {
            RundGangTab {
            }
        }
    }
    
    // The history tab where previously sent items can be browsed.
    Tab {
        imageSource: "asset:///images/rundgang_history.png"
        title: qsTr("History") + Retranslate.onLocaleOrLanguageChanged

        delegate: Delegate {
            id: historyDelegate
            source: "asset:///History/HistoryTab.qml"
            
            onObjectChanged: {
                if(object) {
                    // As the history tab object is created by the delegate its time to load
                    // the history data into the model used by the list in the view.
                    historyData.loadHistory();
                }
            }
        }
    }

	onActivePaneChanged: {
        // Update the app menus NavigationPane in order to push settings/help page.
        appMenu.navigationPane = activePane;
    }
    
    // Application global objects, two data objects handling a data source
    // that can populate a model and a system dialog used to display information
    // about missing permissions.
    attachedObjects: [
        TopicData {
            id: topicData
        },
        HistoryData {
            id: historyData
        },
        SystemDialog {
            id: errorDialog
            title: qsTr("Something went wrong.")
            confirmButton.label: qsTr("Launch Settings")

            onFinished: {
                if (value == SystemUiResult.ConfirmButtonSelection) {
                    // The app settings object can also launch the system settings app.
                    _appSettings.invokeSystemSettings("settings://permissions");
                }
            }
        }
    ]

	// This function is called when something goes wrong due to an permission not being granted.
    function displayPermissionError(error) {
        errorDialog.body = error + qsTr(" You have to restart the app after changing permissions.");
        errorDialog.show();
    }
}
