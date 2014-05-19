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

// A custom header item mimicking the built in Header item but with different colors.
Container {
    property string title
    background: ui.palette.primary

    // Header title Container with padding and Label for text.
    Container {
        topPadding: ui.du(0.5) 
        bottomPadding: ui.du(0.5) 
        leftPadding: ui.du(2) 
        rightPadding: ui.du(2) 
        
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
    
        Label {
            text: title
            textStyle {
                base: SystemDefaults.TextStyles.SubtitleText
                fontWeight: FontWeight.W500
                color: ui.palette.textOnPrimary
            }
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
        }
    }
}
