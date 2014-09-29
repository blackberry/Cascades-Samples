/* Copyright (c) 2014  BlackBerry Limited.
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

import bb.cascades 1.4
import bb.device 1.3

Page {
    property int rightColumnPadding: 2
    
    // Holds the screen height value in order
    // to calculate the screen half for column width
    property int screenHeight: 0
    
    // List of color to chose for highlight color
    property variant colorList: [Color.Cyan, Color.Red, Color.Green, Color.Yellow, Color.LightGray, Color.Blue]
    
    // Tracks color list index
    property int colorIndex: 0
    
    // Stores screen half point
    property int screenHalf: 0
    
    Container {
        
        layout: DockLayout {
        }
        
        // Control to display title
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            
            background: Color.create("#EFEFEF")
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: "Core Controls"
                textStyle.base: SystemDefaults.TextStyles.BodyText
            }
            
            Divider {
            }
        }
        
        // ScrollView holding the core controls
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            
            scrollViewProperties {
                pinchToZoomEnabled: false
            }
            
            Container {
                id: rootItem
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: ui.du(2)
                rightPadding: ui.du(2)
                
                layout: GridLayout {
                    columnCount: 2
                }
                
                // Required in order to disable the system
                // highlight, otherwise the user selected
                // highlight will be overlayed with the system one
                navigation {
                    defaultHighlightEnabled: false
                }
                
                // The following are various core controls meant to
                // show you visually how they would look when setting
                // custom effects on them such as Brightness, Saturation
                // and Highlight effects.
                Label {
                    maxWidth: screenHalf
                    
                    verticalAlignment: VerticalAlignment.Center
                    
                    id: lbl
                    // Localized text with the dynamic translation and locale updates support
                    text: qsTr("I am Label!") + Retranslate.onLocaleOrLanguageChanged
                    textStyle.base: SystemDefaults.TextStyles.BodyText
                }
                
                DropDown {
                    maxWidth: screenHalf
                    
                    title: "DropDown"
                    enabled: true
                    
                    // image + text + description
                    Option {
                        imageSource: "asset:///images/bar_chart.png"
                        text: "drop1"
                        description: "activity lapse"
                    }
                    
                    // text + description
                    Option {
                        text: "drop2"
                        description: "cool!"
                    }
                    
                    // text only
                    Option {
                        text: "drop3"
                    }
                }
                
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    ImageButton {
                        defaultImageSource: "asset:///images/pie_chart.png"   
                    }
                    
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        text: "ImageButton"
                    }
                }
                
                TextField {
                    verticalAlignment: VerticalAlignment.Center
                    leftPadding: ui.du(rightColumnPadding)
                    
                    maxWidth: screenHalf
                    enabled: false
                    hintText: "I am textField"
                }
                
                CheckBox {
                    verticalAlignment: VerticalAlignment.Center
                    
                    maxWidth: screenHalf
                    text: "I am checkbox"
                    checked: true
                }
                
                Container {
                    
                    maxWidth: screenHalf
                    
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: ui.du(rightColumnPadding)
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Right
                        
                        text: "ImageToggleButton"
                    }
                           
                    ImageToggleButton {
                        horizontalAlignment: HorizontalAlignment.Right
                        
                        imageSourceDefault: "asset:///images/dollar.png"
                        imageSourceChecked: "asset:///images/dollar_checked.png"
                    }
                
                }
                
                Container {
                    
                    maxWidth: screenHalf
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    ToggleButton {
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        text: "ToggleButton"
                    }
                }
                
                
                Container {
                    
                    maxWidth: screenHalf
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    ImageView {
                        imageSource: "asset:///images/rss.png"
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        text: "I am ImageView"
                    }
                }
                
                
                Container {
                    
                    maxWidth: screenHalf
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    ActivityIndicator {
                        id: myIndicator
                        preferredWidth: ui.du(13)
                    
                        onCreationCompleted: {
                            myIndicator.setRunning(true)
                        }
                    }
                    
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        
                        text: "ActivityIndicator"
                    }
                }
                
                Container {
                    id: colorBlock
                    background: Color.Gray
                    
                    maxWidth: screenHalf
                    
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    
                    Label {
                        multiline: true
                        text: "Click to select highlight color!"
                    }
                    
                    onTouch: {
                        if(event.isDown()) {
                            if(colorIndex >= colorList.length) {
                                colorIndex = 0;
                            } else {
                                colorIndex++;
                            }
                            colorBlock.background = colorList[colorIndex]
                        }
                    }
                }
                
                DateTimePicker {
                    maxWidth: screenHalf
                    id: picker1
                    title: "DateQml"
                    value: "1980-04-15"
                }
                
                Container {
                    maxWidth: screenHalf
                    
                    Label {
                        text: "Divider"
                    }
                    
                    Divider {
                    }
                }
                
                Button {
                    maxWidth: screenHalf
                    verticalAlignment: VerticalAlignment.Center
                    
                    text: "I am Button"
                    onClicked: {
                        rootItem.requestFocus()
                    }
                }
                
                Header {
                    maxWidth: screenHalf
                    
                    title: "Headers"
                    subtitle: "4 unread"
                }
                
                Container {
                    maxWidth: screenHalf
                    
                    Label {
                        text: "progress indicator"
                    }
                    ProgressIndicator {
                        
                        fromValue: 80
                        toValue: 0
                        value: 50
                    }
                }
                
                Container {
                    maxWidth: screenHalf
                    
                    Label {
                        text: "Slider"
                    }
                    
                    Slider {
                        id: slider
                        fromValue: 20
                        toValue: 30
                        value: 22
                    }
                }
                
                WebView {
                    id: webViewHtml
                    maxWidth: screenHalf
                    
                    html: "<html><head><title>Hello, world!</title></head>" +
                    "<body>I am WebView!!</body>"+
                    "</html>"
                }
                
                effects: [
                    BrightnessEffect {
                        enabled: (menu.effectSelectorOption.value == "B")
                        brightness: menu.brightSlider.value
                    },
                    SaturateEffect {
                        enabled: (menu.effectSelectorOption.value == "S")
                        saturation: menu.satSlider.value
                    },
                    FocusHighlightEffect {
                        enabled: (menu.effectSelectorOption.value == "H")
                        baseColor: colorBlock.background
                        style: (menu.focusSelectorOption.value == "flat") ? FocusHighlightEffectStyle.Flat : FocusHighlightEffectStyle.LinearGradient
                    }
                ]
            }
        }
        
        // Custom control representing the effects
        // menu, which allows you to chose the various options
        CMenu {
            id: menu
        }
    }
    
    // Required in order to get the screen size
    attachedObjects: [
        DisplayInfo {
            id: displayInfo
        }
    ]
    
    onCreationCompleted: {
        // calculate the screen half point and include some padding
        screenHalf = (displayInfo.pixelSize.width / 2) - 21
    }
}
