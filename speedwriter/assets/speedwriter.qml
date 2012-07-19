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
import bb.cascades 1.0
import MyControl 1.0

Page {
    resizeBehavior: PageResizeBehavior.None
    
    Container {
        objectName: "mainContainer"
        layout: AbsoluteLayout {
        }

        // This is the background image that covers the entire screen.
        ImageView {
            imageSource: "asset:///images/deep_background.png"
            layoutProperties: AbsoluteLayoutProperties {
                positionY: 65
            }
        }

        // A multi-line text area, the background can be disabled via the backgroundOpacity property.
        TextArea {
            id: textDisplay
            objectName: "textDisplay"
            editable: false
            preferredWidth: 724
            preferredHeight: 1000
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 37
                positionY: 520
            }
            
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.create ("#e0e0e0")
            }
        }

         // A multi-line text area.
        TextArea {
            objectName: "overlayTextDisplay"
            editable: false
            preferredWidth: textDisplay.preferredWidth
            preferredHeight: textDisplay.preferredHeight
            translationY: textDisplay.translationY
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: textDisplay.layoutProperties.positionX
                positionY: textDisplay.layoutProperties.positionY
            }
            
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.create ("#1f1f1f")
            }
        }

        // A multi-line text area, the background can be disabled via the backgroundOpacity property.
        ImageView {
            imageSource: "asset:///images/background.png"
        }
        
        TextArea {
            objectName: "textInput"
            preferredWidth: 724
            preferredHeight: 99
            hintText: "Type here to see how fast you are."
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 23
                positionY: 695
            }
            
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
            }
        }

		// This is our custom control.
        SpeedGauge {
            objectName: "speedGauge"
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 190
                positionY: 60
            }
        }
    }
}
