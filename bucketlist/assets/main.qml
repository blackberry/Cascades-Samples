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

// Import custom objects for handling BBM posting
import com.bucketlist.bucketbbm 1.0

// Import the bucketmodel set as a type in the application constructor
import com.bucketlist.bucketdata 1.0

NavigationPane {
    id: nav
    
    BucketListPage {
        
        onNewBBMStatus: {
            if (settings.connectToBBM == true) {
                // If no connection has been made to BBM, open the Sheet for letting the user select if
                // it would like to connect or not.
                bbmmanager.showAppRegistration(message, icon);
            } else if (settings.postToBBM == true) {
                //If posting to BBM is enabled set up the ProfilBox message and set the personal message.
                bbmmanager.updateStatus(message, icon);
            }
        }
    }

    // Attached objects of the NavigationPane
    attachedObjects: [
        // Sheet for connecting to BBM
        ComponentDefinition {
            id: bbmConnectSheet
            source: "BBMConnectSheet.qml"   
        },
        // Application settings object, this is where bbm connection status is stored
        BucketSettings {
            id: settings
        },
        // Manager for BBM connection, takes care of registering the app and posting new status.
        BucketBBMManager {
            id: bbmmanager
            
            property variant sheet
            
            onRegisterStatusTextChanged: {
                sheet.errorMessage = registerStatusText;
            }
            
            onAppRegisteredChanged: {
                // Set the connect to BBM property to true if the connection failed,
                // so the user has a chance to remedy the problem.
                settings.connectToBBM = ! appRegistered; //TODO fix so that false is emitted
                
                if (appRegistered) {
                    settings.postToBBM = appRegistered;
                    sheet.close()

                    // The user selected a message when the connection dialog was shown, no that
                    // the application registered successfully with BBM post the first message here.
                    bbmmanager.updateStatus(sheet.waitingMessage, sheet.waitingImage);
                }
            }
            
            function showAppRegistration(message, icon) {
                // Create the sheet for registering the app with BBM from the component definition.
                // Set the parent to bbmmanager so the sheet is destroyed at shut down. 
                sheet = bbmConnectSheet.createObject(bbmmanager);
                
                // Open the registration sheet and store the current message for later posting
                sheet.open();
                sheet.waitingMessage = message;
                sheet.waitingImage = icon;
                
                // Connect to the sheet signal that tells if the manager should connect or not.
                sheet.connectToBBM.connect(connectToBBM)
            }
            
            function connectToBBM(connect) {
                if (connect) {
                    // If the user decide to connect try registering the app with BBM
                    bbmmanager.registerApp();
                } else {
                    // If the user decide not to connect to BBM, update the setting variables
                    // and close the sheet (the user will have to reinstall to connect at a later time).
                    settings.connectToBBM = false;
                    settings.postToBBM = false;
                    sheet.close()
                }
            }
        }
    ]
    
    onCreationCompleted: {
        // When the application NavigationPane is created, we tell Cascades that we support all orientations.
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        
        if (settings.postToBBM == true) {
            // Register the app with BBM if posting is enabled (always false on first launch, since
            // the user is not asked if it wants to use BBM until the first interaction takes place).
            bbmmanager.registerApp();
        }
    }
    
    onPopTransitionEnded: {
        // Transition is done destroy the Page to free up memory.
        page.destroy();
    }
}
