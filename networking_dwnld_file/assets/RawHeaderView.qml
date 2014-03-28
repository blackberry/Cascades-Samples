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
 * either express or implied.
 * See the License for the specific language governing permissions 
 * and limitations under the License.
 */

import bb.cascades 1.2

Container {
    layout: DockLayout {}
    preferredHeight: 700.0
    
    ImageView {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        imageSource: "asset:///images/background.png"
    }
    Container {
        topPadding: 30.0
        leftPadding: 20.0
        rightPadding: 20.0
        
        // Title of the screen
        Label {
            text: "Raw Request Headers"
            horizontalAlignment: HorizontalAlignment.Fill
            topMargin: 20.0
            bottomMargin: 10.0
            textStyle.fontSize: FontSize.Large
            textStyle.textAlign: TextAlign.Center
            textStyle.color: Color.White
        }
        
        // Raw header information
        TextArea {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topMargin: 10
            bottomMargin: 10
            editable: false

            objectName: "txaRawHeaderInfo"
            textStyle.fontSize: FontSize.Small
            textStyle.textAlign: TextAlign.Left
            textStyle.color: Color.White
        }
    }
}
