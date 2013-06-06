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

// Main Container

Container {
    clipContentToBounds: false

    // Container for images and their animations
    Container {
        clipContentToBounds: false
        horizontalAlignment: HorizontalAlignment.Center
        
        layout: DockLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/Warp_Drive.png"
        }
        
        ImageView {
            imageSource: "asset:///images/Warp_Drive_Energy.png"
            
            animations: [
                FadeTransition {
                    id: fadeout
                    fromOpacity: 1
                    toOpacity: 0
                    duration: 2000
                    onEnded: {
                        fadein.play();
                    }
                },
                FadeTransition {
                    id: fadein
                    duration: fadeout.duration
                    fromOpacity: 0
                    toOpacity: 1
                    onEnded: {
                        fadeout.play();
                    }
                }
            ] // animations
        } // ImageView
        
        onCreationCompleted: {
            fadein.play();

            // The animation speed at start should be taken from the stored value of the slider.
            fadeout.duration = (1 - _starshipApp.getValueFor("warpDriveSpeedScanner", "0.0")) * 2000;
        }
    }
    
    Container {
        clipContentToBounds: false
        topPadding: 15
        Label {
            text: qsTr("WARP DRIVE SPEED") + Retranslate.onLanguageChanged
            
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                fontWeight: FontWeight.Bold
            }
        }
        // Slider Container
        Container {
            clipContentToBounds: false
            
            layout: DockLayout {
            }
            
            Slider {
                id: warpDriveSpeedScanner
                objectName: "warpDriveSpeedScanner"
                fromValue: 0
                toValue: 1
                value: _starshipApp.getValueFor(objectName, "0.0")
                
                onImmediateValueChanged: {
                    // If the value changes, we update the animation speed
                    fadeout.duration = (1 - immediateValue) * 2000;
                    if (tooltipcontainer.posValue != immediateValue) {
                        tooltipcontainer.opacity = 1.0;
                        tooltipcontainer.posValue = immediateValue;
                    }
                }
                
                onValueChanged: {

                    // Remove the tooltip and store the value of the Slider.
                    tooltipcontainer.opacity = 0.0;
                    _starshipApp.saveValueFor(warpDriveSpeedScanner.objectName, value)
                }
            }

            // This Container has the tooltip and text
            Container {
                id: tooltipcontainer
                property real posValue: 0.0
                clipContentToBounds: false
                translationX: -108 + 480 * posValue // The x position is determined by the slider handle.
                translationY: -100
                
                layout: DockLayout {
                }
                
                ImageView {
                    id: tooltipImage
                    imageSource: "asset:///images/_Tooltip.png"
                }
                
                Container {
                    bottomPadding: 25
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    layout: DockLayout {
                    }
                    
                    Label {
                        id: tooltipTip
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            color: Color.create("#ff262626")
                            textAlign: TextAlign.Center
                        }
                        
                        text: {
                            if (tooltipcontainer.posValue < 0.3) {
                                qsTr("Sub-light") + Retranslate.onLanguageChanged;
                            } else if (tooltipcontainer.posValue < 0.6) {
                                qsTr("Ridiculous") + Retranslate.onLanguageChanged;
                            } else {
                                qsTr("Ludicrous") + Retranslate.onLanguageChanged;
                            }
                        }
                    }
                } // Container
                
                attachedObjects: [
                    ImplicitAnimationController {
                        id: tooltipController
                        propertyName: "translationX"
                    }
                ]
                
                onCreationCompleted: {
                    tooltipController.enabled = false;
                }
                
            } // tooltip Container
        } // Slider Container
    } // Container
}// Main Container
