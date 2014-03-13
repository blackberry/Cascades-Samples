/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an 
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
 * either express or implied. See the License for the specific 
 * language governing permissions and limitations under the License.
 */

import bb.cascades 1.2

Container {
    horizontalAlignment: HorizontalAlignment.Center
    background: Color.White
    topPadding: 40.0
    attachedObjects: [
        TextStyleDefinition {
            base: SystemDefaults.TextStyles.TitleText
            id: titleStyle
            color: Color.create("#ff0056b3")
            textAlign: TextAlign.Center
        }
    ]
    Container {
        Label {
            text: "Certification Path"
            horizontalAlignment: HorizontalAlignment.Fill
            
            textStyle {
                base: titleStyle.style
            }
        }
        ListView {
            preferredHeight: 500.0
            scaleX: 0.9
            scaleY: 0.9
            dataModel: _CertInfoControl.model

            // Display selected certificate information on selection
            onTriggered: {
                clearSelection()
                select(indexPath)
                _CertInfoControl.setCurrentCertificate(indexPath)
            }
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        Label {
            text: "Certificate Information"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center

            textStyle {
                base: titleStyle.style
            }
        }
        TextArea {
            horizontalAlignment: HorizontalAlignment.Fill
            editable: false

            // Displays info of selected certificate
            text: _CertInfoControl.certificateInfo
        }
    }
    Button {
        text: "Close"
        horizontalAlignment: HorizontalAlignment.Center
        topMargin: 10.0
        scaleX: 0.8
        scaleY: 0.8

        //Hide this Container on click
        onClicked: _CertInfoControl.visible = false
    }
}