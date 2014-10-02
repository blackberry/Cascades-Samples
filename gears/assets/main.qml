/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
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
import my.library 1.0

Page {
    // For storing the total amount of gear rotation.
    property int rotation: 0
    
    // For storing the current x position when keyboard is touched(Down event)
    // X axis will be used to determine the delta in order to change gears.
    property int positionX: 0
    
    // For storing the current y position when keyboard is touched(Down event)
    // Y axis will be used to determine the delta in order to move the gears.
    property int positionY: 0
    
    // List for holding the imageViews.
    property variant imageList
    
    // Place holder for list size
    property int listSize: 0
    
    // Initial index in the imageList
    property int horizontalIndex: 2
    
    // Indicates delta amount between X axis swipes that will cause the selection change
    // to occur.
    property int selectionSensitivity: 300
    
    // Indicates delta amount between Y axis swipes that will cause the gears to spin.
    property int movementSensitivity: 100
    
    // The amount to rotate image along the z axis with each movement.
    property int rotationAmount: 7
    
    attachedObjects: [
        QTimer {
            id: timer
            interval: 300
            singleShot: true
            onTimeout :{
                gearContainer.requestFocus()
            }
        }
    ]
    
    Container {
        layout: DockLayout {
        }
        // The applicaiton background texture
        ImageView {
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///images/background.png"
        }
        
        // Label describing app functionality
        Label {
            verticalAlignment: VerticalAlignment.Top
            
            multiline: true
            // Localized text with the dynamic translation and locale updates support
            text: qsTr("Spin the selected gear by swiping up/down on keyboard. Swipe right/left to select gear.") + Retranslate.onLocaleOrLanguageChanged
            textStyle.base: SystemDefaults.TextStyles.BigText
            textStyle.color: Color.White
        }
        // Container for grouping the gears images
        Container {
            id: gearContainer
            verticalAlignment: VerticalAlignment.Center
            
            // @ekkescorner
            // this helps not to loose the focus if touching on other containers / images
            focusRetentionPolicyFlags: FocusRetentionPolicy.LoseToFocusable
            
            // Focus touch is a requirement in order to get keyboard events
            // without this you will not receive the capacitive keyboard events.
            focusPolicy: FocusPolicy.Touch

            
            layout: AbsoluteLayout {
            }
            
            // Small gear image
            ImageView {
                id: gear
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 730
                    positionY: 160
                }
                imageSource: "asset:///images/gear.png"
                
                rotationZ: 0
                
                // Disable implicit animations to avoid ugly 'jumps' when switching from 180 degree to -180 degree and vice versa
                attachedObjects: [
                    ImplicitAnimationController {
                        propertyName: "rotationZ"
                        enabled: true
                    }
                ]
            }
            
            // Medium gear image
            ImageView {
                id: medGear
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 565
                    positionY: 365
                }
                imageSource: "asset:///images/medium_gear.png"
                
                rotationZ: 0
                
                // Disable implicit animations to avoid ugly 'jumps' when switching from 180 degree to -180 degree and vice versa
                attachedObjects: [
                    ImplicitAnimationController {
                        propertyName: "rotationZ"
                        enabled: true
                    }
                ]
            }
            
            // Biggest gear image
            ImageView {
                id: biggerGear
                
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 400
                }
                
                imageSource: "asset:///images/bigger_gear.png"
                
                rotationZ: 0
                
                // Disable implicit animations to avoid ugly 'jumps' when switching from 180 degree to -180 degree and vice versa
                attachedObjects: [
                    ImplicitAnimationController {
                        propertyName: "rotationZ"
                        enabled: true
                    }
                ]
            }
            
            onCreationCompleted: {
                // @ekkescorner: doesn't make sense before the Page is created 
                // requestFocus();
                imageList = gearContainer.controls
                listSize = imageList.length - 1
                selectedGear(horizontalIndex)
            }
            
            eventHandlers: [
                // Keyboard handler which receives the capacitive keyboard events. Remeber that
                // this will not work without having set focusPolicy: FocusPolicy.Touch on the
                // container first.
                TouchKeyboardHandler {
                    
                    onTouch: {
                        if(event.touchType == TouchType.Move) {
                            rotation += rotationAmount
                            // Movement delta along the X axis
                            var horizontalDirection = event.screenX - positionX
                            var verticalDirection = event.screenY - positionY
                            
                            // If the direction delta meets the selection sensitivity criteria
                            // as a left keboard swipe, than perform selection increment to the left.
                            if(horizontalDirection <= -selectionSensitivity) { // Movement to the left
                                
                                // check list boundry before performing selection
                                if(horizontalIndex < listSize) {
                                    horizontalIndex++
                                    selectedGear(horizontalIndex)
                                    
                                    // reset positions x/y at seleciton time to allow
                                    // calculation of new selection
                                    positionX = event.screenX
                                    positionY = event.screenY
                                }
                                
                            // If the direction delta meets the selection sensitivity criteria
                            // as a right keyboard swipe, than perform selection increment to the right.
                            } else if(horizontalDirection >= selectionSensitivity) { // Movement to the right
                                
                                // check list boundry before performing selection
                                if(horizontalIndex > 0) {
                                    horizontalIndex--
                                    selectedGear(horizontalIndex)
                                    
                                    // reset positions x/y at seleciton time to allow
                                    // calculation of new selection
                                    positionX = event.screenX
                                    positionY = event.screenY
                                }
                            } else {
                                // Verify the delta meets the movement sensitivity criteria before spinning the gears
                                if(verticalDirection <= -movementSensitivity || verticalDirection >= movementSensitivity) {
                                    biggerGear.rotationZ = rotation
                                    medGear.rotationZ = -rotation
                                    gear.rotationZ = rotation
                                }   
                            }
                            console.log("screenX(): " + event.screenX)
                            console.log("screenY(): " + event.screenY)
                            
                        } else if(event.touchType == TouchType.Down) {
                            positionX = event.screenX
                            positionY = event.screenY
                        }
                    }
                } 
            ]
        }
        
        // This button re-sets the focus back to the container. This is
        // required because sometimes when you swipe the keyboard your finger
        // accidently touches the screen and the current focus on the container is lost.
        // added by ekke (@ekkescorner) 
        // we don't need the button because
        // 1. we requestFocus() AFTER Page is created
        // 2. we set FocusRetentionPolicy.LoseToFocusable on gearContainer
//        Button {
//            verticalAlignment: VerticalAlignment.Bottom
//            horizontalAlignment: HorizontalAlignment.Center
//            text: "Re-Focus"
//            onClicked: {
//                gearContainer.requestFocus()
//            }
//        }
    }
    
    // Function for visual selection of the current gear to spin, and unselect the other
    // gears.
    function selectedGear(index) {
        switch(index) {
            case 0:
                imageList[index].imageSource = "asset:///images/gear_selected.png"
                imageList[index+1].imageSource = "asset:///images/medium_gear.png"
                imageList[index+2].imageSource = "asset:///images/bigger_gear.png"
                break;
            case 1:
                imageList[index-1].imageSource = "asset:///images/gear.png"
                imageList[index].imageSource = "asset:///images/medium_gear_selected.png"
                imageList[index+1].imageSource = "asset:///images/bigger_gear.png"
                break;
            case 2:
                imageList[index-2].imageSource = "asset:///images/gear.png"
                imageList[index-1].imageSource = "asset:///images/medium_gear.png"
                imageList[index].imageSource = "asset:///images/bigger_gear_selected.png"
                break;
        }
    }
    
    onCreationCompleted: {
        // to request the focus the page must be created first
        // so the QTimer helps us to have a delay
        timer.start()
    }
}
