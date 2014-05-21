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
import com.rundgang 1.0
import bb.cascades.multimedia 1.0

// A details page used to present all the information available
// on a previously sent feedback. ControlDelegates are used to load
// the appropriate Controls for audio and image.
Page {
    property alias topic: topicLabel.text
    property alias adressList: adressListLabel.text
    property alias date: dateAdded.text
    property alias additionalData: additional.text
    property string informationType: "text"

    property string filePath;

    property variant fontColor: Color.create("#f8f8f8")

    Container {
        id: detailsContainer
            
        Container {
            id: topicContainer
            topPadding: ui.du(2)
            rightPadding: ui.du(2)
            leftPadding: ui.du(2)
            bottomPadding: ui.du(2)
            
            background: Color.create("#121212")
            opacity: 1.0
            horizontalAlignment: HorizontalAlignment.Fill
            
            Label {
                id: dateAdded
                textStyle.color: fontColor
                accessibilityMode: A11yMode.Collapsed
            }
            
            Label {
                id: topicLabel
                multiline: true
                textStyle.base: SystemDefaults.TextStyles.BigText
                textStyle.color: fontColor
                accessibilityMode: A11yMode.Collapsed
            }
            
            // Only visible if a text has been set.
            Label {
                id: adressListLabel
                textStyle.base: SystemDefaults.TextStyles.PrimaryText
                multiline: true
                textStyle.color: fontColor
                visible: (text == "") ? false : true 
                accessibilityMode: A11yMode.Collapsed
            }
            
            // Only visible if a text has been set.
            Label {
                id: additional
                multiline: true
                content.flags: TextContentFlag.ActiveText
                visible: (text == "") ? false : true 
                textStyle.color: fontColor;
                accessibilityMode: A11yMode.Collapsed
            }
        }
        
        ControlDelegate {
            id: infoControlDelegate
            delegateActive: false
            visible: delegateActive
            sourceComponent: barCodeDef
            horizontalAlignment: HorizontalAlignment.Fill
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1;
            }
        }

        // The details view has all A11y information in one place, the other
        // components have been set to  A11yMode.Collapsed (except the audio button).
        accessibility: CustomA11yObject {
            role: A11yRole.Container
            name: informationType + ", " + topicLabel.text + ", " + adressListLabel.text + " ," + additional.text + ", date: " + dateAdded.text
        }
        
        attachedObjects: ImagePaintDefinition {
            id: bkgPaint
        }
    }

    onInformationTypeChanged: {
        if (informationType == "image") {
            // The image is loaded using an ImageTracker, that way a broken image can be shown 
            // if the user has removed the photo from the device.
            topicContainer.opacity = 0.8
            bkgImageTracker.imageSource = "file://" + filePath;
		} else if (informationType == "audio") {
            // If the information is of type audio display the audio control in the control delegate.
            infoControlDelegate.sourceComponent = audioControlDef
            infoControlDelegate.delegateActive = true;
        } else if(informationType == "qr") {
            // Show the barcode as a QR code if it is of bar code type.
            infoControlDelegate.sourceComponent = barCodeDef;
            infoControlDelegate.delegateActive = true;
            infoControlDelegate.accessibilityMode = A11yMode.Collapsed
        }
    }

    attachedObjects: [
        ImageTracker {
            id: bkgImageTracker

            onStateChanged: {
                if (state == ResourceState.Loaded) {
                    bkgPaint.image = image; 
                    detailsContainer.background = bkgPaint.imagePaint
                } else if (state == ResourceState.ErrorNotFound) {
                    // If the image does not exist anymore show broken image.
                    bkgPaint.imageSource = "asset:///images/broken.png"
                    detailsContainer.background = bkgPaint.imagePaint;
                }
            }
        },
        ComponentDefinition {
            id: audioControlDef
            Container {
                layout: DockLayout {
                }
                bottomPadding: ui.du(2)
                rightPadding: ui.du(2)
                
                Button {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Bottom
                    text: qsTr("Play Sound") + Retranslate.onLocaleOrLanguageChanged
                    
                    onClicked: {
                        audioController.invokeAudio("file://" + filePath);
                    }
                    
                    attachedObjects: [
                        AudioController {
                            id: audioController
                        }
                    ]
                }
                accessibility: CustomA11yObject {
                    role: A11yRole.Container
                    name: informationType + ", " + topicLabel.text + ", " + adressListLabel.text + " ," + additional.text + ", date: " + dateAdded.text
                }
            }
        },
        ComponentDefinition {
            id: barCodeDef
            
            QrCodeView {
                preferredHeight: ui.du(50)
                preferredWidth: ui.du(50)
                data: additionalData
            }
        }
    ]
}
