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
        Label {
            text: "Raw Request Headers"
            horizontalAlignment: HorizontalAlignment.Fill
            topMargin: 20.0
            bottomMargin: 10.0

            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                textAlign: TextAlign.Center
                color: Color.White
            }
        }
        TextArea {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topMargin: 10
            bottomMargin: 10
            editable: false

            objectName: "txaRawHeaderInfo"
            hintText: "< Raw header data shown here >"
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                textAlign: TextAlign.Left
                color: Color.White
            }
        }
    }
}
