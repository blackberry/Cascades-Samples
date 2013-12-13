/* Copyright (c) 2013 BlackBerry Limited.
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

Container {
    property alias title: titleLable.text
    property string headerColor: "#000000"
    property real backgroundOpacity: 1.0
    
    // The header height is specified in the design guidelines:
    // https://developer.blackberry.com/devzone/design/bb10/active_frames.html
    preferredHeight: 62    
    horizontalAlignment: HorizontalAlignment.Fill
    
    layout: DockLayout {
    }
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: Color.create(headerColor)
        opacity: backgroundOpacity
    }
    
    Container {
        leftPadding: 27
        verticalAlignment: VerticalAlignment.Center
        Label {
            id: titleLable
            text: "QML"
            textStyle.color: Color.create("#ebebeb")
        }
    }
}
