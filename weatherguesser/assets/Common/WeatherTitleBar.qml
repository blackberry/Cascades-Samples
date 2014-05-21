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

// A custom title bar used throughout the weather guesser app.
TitleBar {
    property alias weatherTitle: titleLabel.text
    
    kind: TitleBarKind.FreeForm
    appearance: TitleBarAppearance.Branded
    
    // This is a custom title bar so we put the content (a text)
    // and an image) in a FreeFormTitleBarKindProperties.
    kindProperties: FreeFormTitleBarKindProperties {
        Container {            
            background: ui.palette.primary
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                id: titleLabel
                verticalAlignment: VerticalAlignment.Center
                
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontSize: FontSize.PercentageValue
                    fontSizeValue: 200
                    color: ui.palette.textOnPrimary
                    textAlign: TextAlign.Center
                }
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }        
        }
    }
} 