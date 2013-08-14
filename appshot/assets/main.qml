/* Copyright (c) 2013 Research In Motion Limited.
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
import bb.system 1.2
import com.appshot 1.0
import "AlbersWallpaper"

// The main purpose of this sample app is to show how one can make
// a programmatic screen shot of a Cascades Application. To illustrate
// how this i done the UI consists of a wallpaper generator.
Page {	
	actionBarVisibility: ChromeVisibility.Overlay
	
	Container {
	    layout: DockLayout {
        }	    
	    
        // The AlbersWallpaper component with the graphical elements.
        AlbersWallpaper {
        }
        
        // An instruction screen that is only shown the first time the application
        // launches, controlled via the custom  _app.showInstruction property
		ControlDelegate {
		    id: instructionDelegate
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            sourceComponent: instructionDef
            delegateActive: false
            visible: delegateActive
            
            onControlChanged: {
                if(control != undefined){
                    // Connect to the delegate control function custom hide signal, so the _app 
                    // property can be altered after first launch.
                    control.hideInstruction.connect(hideAppInstruction);
                }
            }
            
            function hideAppInstruction() {
                _app.showInstruction = false;
                delegateActive: false;
            }
        }

        onCreationCompleted: {
            if (_app.showInstruction) {
                // Activate the Instruction ControlDelegate if its the first time the app launches.
                instructionDelegate.delegateActive = true;
            }
        }
    }

    attachedObjects: [
        Screenshot {
            // This is the Object that takes a screen shot of the application.         
            id: appShot
        },
        QTimer {
            // A timer is needed to delay the screen shot until the ActionBar is hidden.
            // Here we use the QTimer, which has to be registered as a type in the application
            // constructor to work (see constructor in appshotapp.cpp).
            id: hideActionBarTimer
            interval: 200
            singleShot: true
            
            onTimeout: {
                // Take a screen shot by calling the captureScreen function of the AppShot object.
                var filePath = appShot.captureWindow( "file:data/screenshot.jpg", Application.mainWindow.handle );

                // Show the screen shot in a pictures card (see invokalble function in appshotapp.cpp).
                _app.showPhotoInCard(filePath);
                
                // Show the action bar again, so that more screen shots can be made by the user.
                actionBarVisibility = ChromeVisibility.Overlay
            }
        },
        ComponentDefinition {
            id: instructionDef;
            source: "Instruction.qml"
        }
    ]
    
    actions: [
        ActionItem {
            title: qsTr("Shoot Screen")
            imageSource: "asset:///images/ic_done.png"
            ActionBar.placement: ActionBarPlacement.OnBar;
            enabled: !_app.showInstruction
            
            onTriggered: {
                // Hide the ActionBar and start the timer that will take the screen shot
                // once it is no longer visible.
                actionBarVisibility = ChromeVisibility.Hidden;
                hideActionBarTimer.start();
            }
        }
    ]
}
