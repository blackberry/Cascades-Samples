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

/* The PhotoBomber is a camera application that let's you take photos and then have them 
* "bombed" by someone that jumps in, automatically.
* 
* We use the Camera control from cascades multimedia, it needs to be initiated from C++  
* code before we can use it though.
*/

import bb.cascades 1.0
import bb.cascades.multimedia 1.0

Page {
    // A container is used to gather visual items together.
    content: Container {
        layout: DockLayout {
        }
        Container {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }
            // This is the camera control that is defined in the cascades multimedia library
            Camera {
                id: camera
                objectName: "myCamera"
                onTouch: {
                    if (event.isDown()) {
                        capturePhoto();
                        setting.visible = true;
                      
                    }
                } 
        		             
                // There are loads of messages we could listen to here
                //onPhotoSaved and onShutterFired are taken care of in the C code
                onCameraOpenFailed: {
                    console.log("onCameraOpenFailed signal received with error " + error);
                }
                onViewfinderStartFailed: {
                    console.log("viewfinderStartFailed signal received with error " + error);
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
                    photoBomber.manipulatePhoto(fileName);
                    imageSheet.visible = true;
                    imageSheet.previewPath = fileName;
                }
            }
        }
        Container {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Bottom
            }
            layout: DockLayout {
            }
            ImageButton {
                id: setting
                visible: false
                defaultImageSource: "asset:///images/settings_unpressed.png"
                pressedImageSource: "asset:///images/settings_pressed.png"
                onClicked: {
                    imageSheet.visible = true;
                }
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Bottom
                }
            }
        }
    }
    attachedObjects: [
        ImageSheet {
            id: imageSheet
        }
    ]
}
