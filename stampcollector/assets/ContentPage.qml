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

Page {
    content: Container {
        background: Color.create ("#262626")
        layout: DockLayout {
        }
        Container {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
        
            // Large stamp image, source is updated in code, by setting the _contentView.URL property.
            ImageView {
                imageSource: _contentView.URL
                preferredWidth: 550
                preferredHeight: 450;
            
                // Since the large stamp images are of varying sizes the AspectFit is used to 
                // make sure that the image fits inside the preferred size and keeps its aspect ratio.            
                scalingMethod: ScalingMethod.AspectFit
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        
            // Descriptive text, will be replaced in code, by setting the _contentView.infoText property.
            TextArea {                
                topMargin: 30
                preferredHeight: 200
                text: _contentView.infoText
                editable: false;
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
 	                color: Color.LightGray
                    alignment: TextAlignment.Center
                }
                
            }
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {

            onTriggered: { 
                // The _nav context property is bound in c++.
                _nav.pop();
            }
        }
    }
}
