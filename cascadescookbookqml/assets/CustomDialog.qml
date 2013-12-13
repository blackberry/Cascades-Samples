/* Copyright (c) 2012 BlackBerry Limited.
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

// A CustomDialog is a full-screen view that is displayed as a transparent layer on top
// of the current screen. Basically, it's a separate view within the current application.
// The use-case of the dialog is similar to Sheet. It is used when something happens
// outside the normal UI flow, like e.g. an alarm or message is triggered.
// In this recipe, it shown how to create a CustomDialog. In our case, the CustomDialog is 
// a cozy candle that triggers the fire alarm.
RecipePage {
    id: customDialogPage
    RecipeContainer {
        Container {
            layout: AbsoluteLayout {
            }

            ImageView {
                id: flameImage
                property real flametime: 400
                imageSource: "asset:///images/customdialog/flame.png"
                
                // The flame animation gradually scales the flame up in Y direction and 
                // finally triggers the CustomDialog.
                animations: [
                    SequentialAnimation {
                        id: risingFlame
                        ScaleTransition { toY: 1.2; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.1; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.4; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.3; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.6; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.5; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.9; duration: flameImage.flametime}
                        ScaleTransition { toY: 1.7; duration: flameImage.flametime}
                        ScaleTransition { toY: 2.0; duration: flameImage.flametime}
                        
                        onEnded: {
                            customdialog.open()
                        }
                    }
                ]
                onCreationCompleted: risingFlame.play()

                attachedObjects: [
                    LayoutUpdateHandler {
                        onLayoutFrameChanged: {
                            // A layout update handler is used to find the size of the image so the
                            // pivot point can be set to the  middle-bottom of the image, which makes
                            // the image scale up from the bottom in the risingFlame animation above.
                            flameImage.pivotX = layoutFrame.width / 2
                            flameImage.pivotY = layoutFrame.height / 2
                        }
                    }
                ]
            }
            
            ImageView {
                imageSource: "asset:///images/customdialog/background.png"
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
                        risingFlame.play();
                        customDialogPage.actionBarVisibility = ChromeVisibility.Default                       
                    }
                }// CustomDialogAlarm
            ]// attachedObjects
        }// Container
    }// RecipeContainer
    
    attachedObjects: [
        TextStyleDefinition {
            id: textStyleLightTitle
            base: SystemDefaults.TextStyles.TitleText            
            color: Color.White            
        }   
    ]
    
    function  peekEnded()	 {
        risingFlame.play()
    }
    
    function peekStarted() {
        risingFlame.stop()
    }
    
    onCreationCompleted: {
        // To avoid the custom dialog triggering while we are peeking back
        // at the list page we connect to its peek signals. We have access
        // to the list page since it is sharing the same context as this Page,
        // they are both under the same NavigationPane (root AbstractPane).
        // Ordinary it would be better to connect to the functions when creating
        // the page in main.qml, we do it this way here in order to not obfuscate
        // the code in main.qml (for pedagogic reasons). 
        recipeListPage.peekStarted.connect(peekStarted)
        recipeListPage.peekEnded.connect(peekEnded)
    }
}// RecipePage
