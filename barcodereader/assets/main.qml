/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
import bb.cascades.multimedia 1.0
import bb.multimedia 1.0

Page {
    id: page

    Container {
        background: Color.Black

        layout: DockLayout {}

        //! [0]
        // The camera preview control
        Camera {
            id: camera

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            onCameraOpened: {
                // Apply some settings after the camera was opened successfully
                getSettings(cameraSettings)
                cameraSettings.focusMode = CameraFocusMode.ContinuousAuto
                cameraSettings.shootingMode = CameraShootingMode.Stabilization
                applySettings(cameraSettings)

                // Start the view finder as it is needed by the barcode detector
                camera.startViewfinder()
            }

            //! [2]
            attachedObjects: [
                BarcodeDetector {
                    id: barcodeDetector
                    camera: camera
                    formats: BarcodeFormat.Any
                    onDetected: {
                        if (resultLabel.text != data) {
                            resultLabel.text = data;
                            resultArea.visible = true;
                            scannedSound.play()
                        }
                    }
                },
                SystemSound {
                    id: scannedSound

                    sound: SystemSound.GeneralNotification
                },
                CameraSettings {
                    id: cameraSettings
                }
            ]
            //! [2]
        }
        //! [0]

        // The overlay image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/overlay.png"
        }

        // The area at the top that shows the results
        Container {
            id: resultArea

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top

            layout: DockLayout {}

            visible: false

            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                imageSource: "asset:///images/result_background.png"
            }

            Label {
                id: resultLabel

                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    color: Color.White
                    textAlign: TextAlign.Center
                }

                multiline: true
            }
        }

        // The upper cover image
        ImageView {
            id: topCover

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            imageSource: "asset:///images/top_cover.png"

            onTouch: {
                if (event.isDown())
                    startupAnimation.play()
            }
        }

        // The lower cover image
        ImageView {
            id: bottomCover

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            imageSource: "asset:///images/bottom_cover.png"

            onTouch: {
                if (event.isDown())
                    startupAnimation.play()
            }
        }

        // The startup animation that slides out the cover images
        animations: ParallelAnimation {
            id: startupAnimation

            SequentialAnimation {
                target: topCover
                TranslateTransition {
                    fromY: 0
                    toY: -640
                    duration: 1250
                    easingCurve: StockCurve.QuarticInOut
                }
            }
            SequentialAnimation {
                target: bottomCover
                TranslateTransition {
                    fromY: 0
                    toY: 680
                    duration: 1250
                    easingCurve: StockCurve.QuarticInOut
                }
            }

            //! [1]
            onStarted: {
                camera.open()
            }
            //! [1]
        }
    }
}
