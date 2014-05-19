/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
import bb.cascades.multimedia 1.0
import com.rundgang 1.0

// A simple camera that takes a picture and return the path so that it
// can be shared. The camera can also act as a BarCodeDetector in which case
// the bar code data will be sent as feedback.
Page {
    id: photoPage
    property bool rescalePhotos: false;
    signal feedback(string fileName)
    signal feedbackCode(string codeData)
    
    // In order for the Camera viewfinder not to be scaled the action bar is overlayed.
    actionBarVisibility: ChromeVisibility.Overlay

    // A Container is used to gather visual items together.
    Container {
        background: Color.Black
        layout: DockLayout {
        }
        Container {
            id: cameraContainer
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            // The Camera itself does not have  A11y yet so we put an object on the 
            // Container that will trigger capturePhoto().
            accessibility: CustomA11yObject {
                role: A11yRole.Button
                name: qsTr("Viewfinder, tap to take a photo.")
                
                ComponentA11ySpecialization {
                    onActivationRequested: {
                        if (event.type == A11yComponentActivationType.Tap) {
                            camera.capturePhoto();     
                            camera.photoTaken = true;
                        }
                    }
                }    
            }
            
            // This is the camera control that is defined in the cascades multimedia library.
            Camera {
                id: camera
                property bool photoTaken: false
                
                // Let touch events through in order to be able to trigger capture photo 
                // when screen reader is enabled, see  CustomA11yObject on cameraContainer.
                touchPropagationMode: TouchPropagationMode.Full
                                    
                onTouch: {
                    if (event.touchType == TouchType.Down) {
                        if(!photoTaken){
                            // Only one photo for each time the camera is opened is allowed
                            // or there might be multiple signals that could mess up navigation.
                            capturePhoto();     
                            photoTaken = true;
                        }
                    }
                }

                // When the camera is opened we want to start the viewfinder.
                onCameraOpened: {
                    // A helper function is called to set resolution with the correct aspect ratio, this can not be done in QML.
                    photoController.selectAspectRatio(camera);

                    // Additional camera settings, setting focus mode and stabilization.
                    getSettings(cameraSettings)
                    cameraSettings.focusMode = CameraFocusMode.ContinuousAuto
                    cameraSettings.shootingMode = CameraShootingMode.Stabilization
                    applySettings(cameraSettings)
                    camera.startViewfinder();

                    barcodeDetector.codeDetected = false;
                    photoTaken = false;
                }
                
                onPhotoSaved: {
                    // Check if the photo is supposed to be rescaled and call function that does the scaling.
                    var photoScale = ((rescalePhotos == true) ? 0.25 : 1.0);
                    photoController.scaleImage(fileName, photoScale); 
                    feedback(fileName);
                }

                onCameraResourceAvailable: {
                    // Start the viewfinder if the resource becomes available (can occur when returning from e.g. sleep).
                    camera.startViewfinder();
                }
                
                onPhotoSaveFailed: {
                    // It was not possible to save the photo, it could be that the permission to access shared files was not granted.
                    // This could occur for other reasons as well, but we display a dialog since this it the most likely scenario.
                    displayPermissionError(qsTr("Sorry could not snap the shot, could be the Shared Files permission is not set?"))
                    photoTaken = false;                    
                }
                
                attachedObjects: [
                    CameraSettings {
                        id: cameraSettings
                    }
                ]
                
                // Find a supported camera unit and open the camera.
                onCreationCompleted: {
                    var cameraList = camera.supportedCameras;

                    // Check to see if your app has access to the camera.
                    if (cameraList.length == 0 || cameraList[0] == CameraUnit.None) {
                        // No camera is available instruct the user that in order to use
                        // this functionality it has to set the permission in settings. Call the 
                        // application function to display an error message (see main.qml) 
                        displayPermissionError(qsTr("The Camera cannot be opened. Do we have permission to use it?"))

						// Triggering photos so no false signals are sent.
                        photoTaken = true;
                    } else if (cameraList[0] == CameraUnit.Rear || cameraList[1] == CameraUnit.Rear) {
                        camera.open(CameraUnit.Rear);
                    } else if (cameraList[0] == CameraUnit.Front || cameraList[1] == CameraUnit.Front) {
                        camera.open(CameraUnit.Front);
                    } 
                }   
            }
        }
    } // content Container

    attachedObjects: [
        // A bar code detector that adds the possibility to feedback captured bar codes.
        BarcodeDetector {
            id: barcodeDetector
            property bool codeDetected: false            
            camera: camera
            formats: BarcodeFormat.Any

            onDetected: {
                if (! codeDetected) {
                    // We only allow for one code to be sent as soon as one is detected we stop detecting.
                    codeDetected = true;
                    feedbackCode(data);
                }
            }
        },
        PhotosController {
            id: photoController
        }
    ]
}// Page
