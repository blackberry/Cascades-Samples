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
// The use-case of the dialog is similar to Sheet, in that it is used when something happens
// out-side the normal UI flow, like e.g. an alarm or message is triggered.
// In this recipe it shown how a CustomDialog is created and shown as a cozy candle triggers
// the fire alarm.
RecipePage {
    RecipeContainer {
        Container {
            layout: AbsoluteLayout {
            }

            ImageView {
                property real flametime: 400
                imageSource: "asset:///images/customdialog/flame"
                
                // The pivot points are set to the middle bottom of the image
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
                            customdialog.visible = true
                        }
                    }
                ]
                onCreationCompleted: risingFlame.play();
            }
            
            ImageView {
                imageSource: "asset:///images/customdialog/background"
            }
            attachedObjects: [
                // The CustomDialog is added as an attached object since it is visible in the
                // UI from the start. Since it is often the case that a dialog is used in many
                // different places in an application it is set up as a separate component, so that
                // it easily can be added to other Pages.
                CustomDialogAlarm {
                    id: customdialog
                    onVisibleChanged: {
                        if(!visible) {
                            risingFlame.play();
                        }
                    }
                }
            ]
        }
    }
}
