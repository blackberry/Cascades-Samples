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
                property real flametime: 400
                imageSource: "asset:///images/customdialog/flame.png"
                
                // The pivot points are set to the middle-bottom of the image
                // so that it can be scaled upwards in the animation below.
                pivotX: 445/2
                pivotY: 514/2

                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 180
                    positionY: 0
                }
                
                // The flame animation gradually scales the flame up in Y direction and 
                // finally triggers the CustomDialog.
                animations: [
                    SequentialAnimation {
                        id: risingFlame
	                    ScaleTransition { toY: 1.2; duration: flametime}
                        ScaleTransition { toY: 1.1; duration: flametime}
                        ScaleTransition { toY: 1.4; duration: flametime}
                        ScaleTransition { toY: 1.3; duration: flametime}
                        ScaleTransition { toY: 1.6; duration: flametime}
                        ScaleTransition { toY: 1.5; duration: flametime}
                        ScaleTransition { toY: 1.9; duration: flametime}
                        ScaleTransition { toY: 1.7; duration: flametime}
                        ScaleTransition { toY: 2.0; duration: flametime}
                        
                        onEnded: {
                            customdialog.open()
                        }
                    }
                ]
                onCreationCompleted: risingFlame.play();
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
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                // Override the back button in order to stop the dialog if navigation away from the Page occur
                risingFlame.stop()
                
                // The NavigationPane id is set in main.qml and this Page share the same context, so it is possible
                // to pop the Page from here.
                nav.pop();
            }
        }
    }
    
}// RecipePage
