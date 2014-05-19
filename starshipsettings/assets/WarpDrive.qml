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

// Main Container of the warp drive component.
Container {    
    clipContentToBounds: false

    // Container for images and their animations
    Container {
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
                    toOpacity: 0
                    duration: 2000
                    onEnded: {
                        fadein.play();
                    }
                },
                FadeTransition {
                    id: fadein
                    duration: fadeout.duration
                    toOpacity: 1
                    onEnded: {
                        fadeout.play();
                    }
                }
            ]
        }

        onCreationCompleted: {
            // The animation speed at start should be taken from the stored value of the slider.
            fadeout.duration = (1 - appSettings.warpDriveSpeedScanner) * 2000;
            fadeout.play();
        }
    }

    Container {
        clipContentToBounds: false
        topPadding: ui.du(1)

        Label {
            id: warpLabel
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
                id: slider
                fromValue: 0
                toValue: 1
                value: appSettings.warpDriveSpeedScanner

                onImmediateValueChanged: {
                    // If the value changes, we update the animation speed
                    fadeout.duration = (1 - immediateValue) * 2000;
                    if (tooltipcontainer.posValue != immediateValue) {
                        tooltipcontainer.opacity = 1.0;
                        tooltipcontainer.posValue = immediateValue;
                    }
                }

                onValueChanged: {
                    // Store the value of the Slider.
                    appSettings.warpDriveSpeedScanner = value;                    
                }
                
                onTouch: {
                    if(event.touchType == TouchType.Up) {
                    		// Remove the tooltip on touch up event. 
                    		tooltipcontainer.opacity = 0.0;    
                    }                        
                }
                
                onFocusedChanged: {
                    console.debug("focus " + focused)
                }

                accessibility {
                    labelledBy: warpLabel
                }

                attachedObjects: [
                    LayoutUpdateHandler {
                        id: sliderLayoutHandler
                    }
                ]
            }

            // This Container has the tooltip and text
            Container {
                id: tooltipcontainer
                property real posValue: 0.0
                property real toolTipYOffset: -toolTipLayoutHandler.layoutFrame.height
                property real toolTipXOffset: -toolTipLayoutHandler.layoutFrame.width/2 + ui.du(2.25)
                property real toolTipTotalDistance: sliderLayoutHandler.layoutFrame.width - ui.du(4.5)
                
                // The tooltip offsets that will position center it and move it above the slider handle.
                translationX: toolTipXOffset + toolTipTotalDistance * posValue
                translationY: toolTipYOffset
                opacity: 0

                layout: DockLayout {
                }
                
                ImageView {
                    imageSource: "asset:///images/_Tooltip.png"
                    attachedObjects: [
                        // To position the tooltip we need to now the size of the image.
                        LayoutUpdateHandler {
                            id: toolTipLayoutHandler
                        }
                    ]
                }

                Container {
                    bottomPadding: ui.du(2)
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Fill

                    layout: DockLayout {
                    }

                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center

                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
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
                }
                
                attachedObjects: [
                    // Implicit animations are turned off on the tooltip this will make it snap to the handle.
                    ImplicitAnimationController {
                        id: tooltipController
                        propertyName: "translationX"
                        enabled: false
                    }
                ]
            }
        }
    }
}
