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

// A CustomDialog is a full-screen view that is displayed as a transparent layer on top
// of the current screen. Basically, it's a separate view within the current application.
// The use-case of the dialog is similar to Sheet. It is used when something happens
// outside the normal UI flow, like e.g. an alarm or message is triggered.
// In this recipe, it shown how to create a CustomDialog. In our case, the CustomDialog is 
// a cozy candle that triggers the fire alarm.
RecipePage {
    id: customDialogPage
    RecipeContainer {
        
        // The CustomDialog is in this case triggered by a burning candle that 
        // emits a signal when it burns to hot.
        CustomDialogCandle {
            id: candle
            
            onTriggerFireAlarm: {
                customdialog.open();
            }
        }
        
        attachedObjects: [
            // The CustomDialog is added as an attached object since it is visible in the
            // UI from the start. Since a dialog is often used in many different places in
            // an application, a dialog is set up as a separate component to easily be added to other Pages.
            CustomDialogAlarm {
                id: customdialog
                onOpened: {                        
                    customDialogPage.actionBarVisibility = ChromeVisibility.Hidden
                }
                onClosed: {
                    candle.startFire();
                    customDialogPage.actionBarVisibility = ChromeVisibility.Default                       
                }
            }
        ]
    }
    
    onCreationCompleted: {
        // To avoid the custom dialog triggering while we are peeking back
        // at the list page we connect to its peek signals. We have access
        // to the list page since it is sharing the same context as this Page,
        // they are both under the same NavigationPane (root AbstractPane).
        // Ordinary it would be better to connect to the functions when creating
        // the page in main.qml, we do it this way here in order to not obfuscate
        // the code in main.qml (for pedagogic reasons). 
        recipeListPage.peekStarted.connect(candle.stopFire);
        recipeListPage.peekEnded.connect(candle.startFire);
    }
}
