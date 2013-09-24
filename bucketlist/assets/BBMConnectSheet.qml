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

import bb.cascades 1.2

Sheet {
    id: bbmConnectSheet
    property string waitingMessage
    property string waitingImage
    property string errorMessage
    signal connectToBBM(bool connect)

    // Page with UI for connection to BBM.
    Page {
        id: bbmConnectScreen
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            leftPadding: 50
            rightPadding: leftPadding
            
            Container {
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: qsTr("Connect Bucket List to BBM?") + Retranslate.onLanguageChanged
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Button {
                        text: qsTr("Yes") + Retranslate.onLanguageChanged
                        onClicked: {
                            // When yes has been answered the only outcome is success or
                            // failure, after failure it is not possible to try again.
                            enabled = false;
                            bbmConnectSheet.connectToBBM(true);
                        }
                    }
                    
                    Button {
                        text: qsTr("No") + Retranslate.onLanguageChanged
                        onClicked: {
                            bbmConnectSheet.connectToBBM(false);
                        }
                    }
                }
                
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    multiline: true
                    text: bbmConnectSheet.errorMessage
                }
            }
        }
    }
}
