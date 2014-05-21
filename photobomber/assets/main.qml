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

// The PhotoBomber is a camera application that lets you take photos and then have them
// "bombed" by someone that jumps in, automatically.
//
// We use the Camera control from cascades multimedia, it needs to be initiated from C++
// code before we can use it though.
import bb.cascades 1.3
import bb.cascades.multimedia 1.0
import bb.multimedia 1.0
import bb.system 1.2

Page {
    id: photoPage
    actionBarVisibility: ChromeVisibility.Hidden
    // A Container is used to gather visual items together.
    Container {
        layout: DockLayout {
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            // This is the camera control that is defined in the cascades multimedia library.
            Camera {
                id: camera
                onTouch: {
                    if (event.isDown()) {
                        // Take photo
                        capturePhoto();
                    }
                }

                // When the camera is opened we want to start the viewfinder
                onCameraOpened: {
                    // Using helper function to set resolution and aspect ratio.
                    _photoBomber.selectAspectRatio(camera);

                    // Additional camera settings, setting focus mode and stabilization
                    getSettings(cameraSettings)
                    cameraSettings.focusMode = CameraFocusMode.ContinuousAuto
                    cameraSettings.shootingMode = CameraShootingMode.Stabilization
                    applySettings(cameraSettings)
                    camera.startViewfinder();
                }

                // There are loads of messages we could listen to here.
                // onPhotoSaved and onShutterFired are taken care of in the C++ code.
                onCameraOpenFailed: {
                    console.log("onCameraOpenFailed signal received with error " + error);
                    toast.show();
                }
                onViewfinderStartFailed: {
                    console.log("viewfinderStartFailed signal received with error " + error);
                    toast.show();
                }
                onViewfinderStopFailed: {
                    console.log("viewfinderStopFailed signal received with error " + error);
                }
                onPhotoCaptureFailed: {
                    console.log("photoCaptureFailed signal received with error " + error);
                }
                onPhotoSaveFailed: {
                    console.log("photoSaveFailed signal received with error " + error);
                }
                onPhotoSaved: {
                    _photoBomber.manipulatePhoto(fileName);
                    // Will set the filename of the latest captured bomber photo in the ImageButton property.
                    setting.lastFileName = fileName;
                    // Makes the ImageButton visible when a photo is captured.
                    setting.enabled = true;
                    photoPage.actionBarVisibility = ChromeVisibility.Compact
                }
                onShutterFired: {
                    // A cool trick here to play a sound. There are legal requirements in many countries to have a shutter-sound when
                    // taking pictures. So we need this shutter sound if you are planning to submit you're app to app world.
                    // So we play the shutter-fire sound when the onShutterFired event occurs.
                    cameraSound.play();
                }
                onCameraResourceAvailable: {
                    // This signal handler is triggered when the Camera resource becomes available to app
                    // after being lost by for example putting the phone to sleep, once it has been received
                    // it is possible to start the viewfinder again.
                    camera.startViewfinder()
                }

                onCreationCompleted: {
                    // Open the front facing camera.
                    camera.open(CameraUnit.Front);
                }

                attachedObjects: [
                    CameraSettings {
                        id: cameraSettings
                    },
                    SystemSound {
                        id: cameraSound
                        sound: SystemSound.CameraShutterEvent
                    },
                    SystemToast {
                        id: toast
                        body: qsTr("An error occurred. Make sure no other camera applications are running, then try again.")
                    }
                ]
            }
        }

    } // content Container
    actions: [
        ActionItem {
            id: setting
            title: "view photo"
            property string lastFileName: ""
            enabled: false
            imageSource: "asset:///images/settings_unpressed.png"
            onTriggered: {
                //Show the latest bomber image available. Takes the ImageButtons Property as argument.
                _photoBomber.showPhotoInCard(lastFileName);

            }
        } // ActionItem
    ]
}// Page
