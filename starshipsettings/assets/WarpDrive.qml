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

Container {
    clipContentToBounds: false
    Container {
        clipContentToBounds: false
        layout: DockLayout {
        }
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Center
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
                        fadein.play ();
                    }
                },
                FadeTransition {
                    id: fadein
                    duration: fadeout.duration
                    fromOpacity: 0
                    toOpacity: 1
                    onEnded: {
                        fadeout.play ();
                    }
                }
            ]
        }
        onCreationCompleted: {
            fadein.play ();

            // The animation speed at start up should be taken from the stored value of the slider.
            fadeout.duration = (1 - _starshipApp.getValueFor ("warpDriveSpeedScanner", "0.0")) * 2000;
        }
    }
    Container {
        clipContentToBounds: false
        layout: StackLayout {
            topPadding: 35
        }
        Label {
            text: "WARP DRIVE SPEED"
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                fontWeight: FontWeight.Bold
                color: Color.create ("#ff262626")
            }
        }
        Container {
            layout: DockLayout {
            }
            clipContentToBounds: false
            Slider {
                id: warpDriveSpeedScanner
                objectName: "warpDriveSpeedScanner"
                fromValue: 0
                toValue: 1
                value: _starshipApp.getValueFor (objectName, "0.0")
                onValueChanging: {
                    // If we get a change we update the animation speed.
                    fadeout.duration = (1 - value) * 2000;
                    if (tooltipcontainer.posValue != value) {
                        tooltipcontainer.opacity = 1.0;
                        tooltipcontainer.posValue = value;
                    }
                }
                onValueChanged: {
                    // Remove tooltip and store the value of the Slider.
                    tooltipcontainer.opacity = 0.0;
                    _starshipApp.saveValueFor (warpDriveSpeedScanner.objectName, value)
                }
            }

            // This contains the tooltip and the text.
            Container {
                id: tooltipcontainer
                property real posValue: 0.0
                layout: DockLayout {
                }
                clipContentToBounds: false
                translationX: -108 + 480 * posValue // The x position is determined by the slider handle.
                translationY: -100
                ImageView {
                    id: tooltipImage
                    imageSource: "asset:///images/_Tooltip.png"
                }
                Container {
                    layout: DockLayout {
                        bottomPadding: 25
                    }
                    layoutProperties: DockLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Fill
                    }
                    Label {
                        id: tooltipTip
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            color: Color.create ("#ff262626")
                            alignment: TextAlignment.Center
                        }
                        text: {
                            if (tooltipcontainer.posValue < 0.3) {
                                "Sub-light"
                            } else if (tooltipcontainer.posValue < 0.6) {
                                "Ridiculous";
                            } else {
                                "Ludicrous";
                            }
                        }
                        layoutProperties: DockLayoutProperties {
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                }
                attachedObjects: [
                    ImplicitAnimationController {
                        id: tooltipController
                        propertyName: "translationX"
                    }
                ]
                onCreationCompleted: {
                    tooltipController.enabled = false;
                }
            }
        }
    }
}
