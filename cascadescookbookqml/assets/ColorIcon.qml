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
import bb.cascades 1.3

// A QML component used by the Color recipe to present a colored
// ImageView together with a string.
Container {
    property string iconImageSource: "asset:///images/color/icon_waiting.png"
    property variant iconColor: Color.Red
    
    ImageView {
        id: iconImage
        horizontalAlignment: HorizontalAlignment.Center
        imageSource: iconImageSource
        filterColor: iconColor
    }
    
    Label {
        horizontalAlignment: HorizontalAlignment.Center        
        text: Color.toHexString(iconColor).toUpperCase()
        
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            fontWeight: FontWeight.Bold
            color: iconColor
        }
    }
}
