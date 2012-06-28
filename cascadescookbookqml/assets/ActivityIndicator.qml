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
import "Common"

// Simple ActivityIndicator recipe.
RecipePage {
    RecipeContainer {
        Container {
            layout: StackLayout {
                leftPadding: 20
                rightPadding: leftPadding
            }
         
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            TextArea {
                bottomMargin:100
                text: "This is an egg boiling simulator"
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                }
                editable: false
            }

            Container {
                layout: DockLayout {
                }
                ImageView {
                    id: unbroken
                    imageSource: "asset:///images/stockcurve/egg.png"
                    
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
                ImageView {
                    id: broken
                    imageSource: "asset:///images/stockcurve/broken_egg.png"
                    opacity : 0.0
                    
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
                
                ActivityIndicator {
                    id: myIndicator
                    preferredWidth: 130
                    preferredHeight: 130
                    layoutProperties: DockLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
            }
            Button {
                topMargin:100
                text: "start cooking"
                
                onClicked: {
                    if( text== "start cooking") {
                        text = "look away"
                        myIndicator.start();
                    } else {
                        myIndicator.stop();
                        enabled = false
                        text = "clean up"  
                        unbroken.opacity =0.0 
                        broken.opacity = 1.0
                    }
                }
            }
        }
    }
}

