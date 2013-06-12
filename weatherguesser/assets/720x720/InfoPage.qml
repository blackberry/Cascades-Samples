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
import bb.cascades 1.2

// This is the info page with some information about the application and a bit of decorations.

NavigationPane {
    id: navigation
    
    Page {
        
        Container {
            background: backgroundPaint.imagePaint
            
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/info/background.png"
                }
            ]
            
            layout: DockLayout {
            }
            
            Container {
                leftPadding: 15 
                bottomPadding: -75
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Left
                
                ImageView {
                    imageSource: "asset:///images/info/cloud.png"
                }
            }
            // Sun image
            Container {
                rightPadding: 15
                topPadding: 15
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                
                ImageView {
                    imageSource: "asset:///images/info/sun.png"
                }
            }
            // Descriptive Text
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                
                TextArea {
                    text: "Welcome to the weather guesser. This little app will predict (guess) the weather, not only today or tomorrow, but for the whole year. Sounds too good to be true? Go ahead and try it."
                    enabled: false
                    editable: false
                    preferredWidth: 650
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        fontStyle: FontStyle.Italic
                        color: Color.White
                    }
                }
            }
            // Footer information
            Container {
                rightPadding: 15
                bottomPadding: 15
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Right
                
                Label {
                    text: "Cascades sample app 2012."
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.White
                    }
                }
            }
        }

        // The info page has an additional action, that is placed in the overflow menu to the right.
		// When the user pressed the "More info", they are directed to the MoreInfoPage.qml.
        actions: [
            ActionItem {
                title: "More Info"
                imageSource: "asset:///images/menuicons/icon_continents.png"
                
                onTriggered: {
                    var moreInfoPage = moreInfoPageDefinition.createObject();
                    navigation.push(moreInfoPage);
                }
            }
        ]
    }
    attachedObjects: [
        ComponentDefinition {
            id: moreInfoPageDefinition
            source: "MoreInfoPage.qml"
        }
    ]
}
