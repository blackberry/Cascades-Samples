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

// Custom container making up the user menu
// for selecting various effect options
Container {
    id: menu
    
    // Menu size
    property int menuSize: 45
    
    // Offset for transitioning menu off screen but leaving
    // a band visible for exanding menu
    property int offset: ui.du(menuSize - 10)
    
    // Aliases to reference saturation, brightness sliders, and
    // effects radio groups
    property alias effectSelectorOption: effectSelector.selectedOption
    property alias brightSlider: brightSlider
    property alias satSlider: satSlider
    property alias focusSelectorOption: focusRadio.selectedOption
    
    verticalAlignment: VerticalAlignment.Bottom
    
    // Start menu positioning off the screen
    translationY: offset
    
    background: Color.Black
    minHeight: ui.du(menuSize)
    maxHeight: ui.du(menuSize)
    horizontalAlignment: HorizontalAlignment.Fill    
    

    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        horizontalAlignment: HorizontalAlignment.Fill
        
        // Toggle for exanding and collapsing the menu
        ImageToggleButton {
            imageSourceDefault: "asset:///images/forward.png"
            imageSourceChecked: "asset:///images/downward.png"
            
            onCheckedChanged: {
                if(checked) {
                    hideMenu.stop()
                    showMenu.play()
                } else {
                    showMenu.stop()
                    hideMenu.play()
                }
            }
        }
        
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 4
            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            
            
            // Menu title
            Label {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                text: "Highlight Effects"
                textStyle.color: ui.palette.primaryDark
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.fontFamily: 'Comic Sans MS'
            }
        }
        
        
    }
    
    // Segmented control for selecting type of effect
    SegmentedControl {
        id: effectSelector
        Option {
            id: option1
            text: "Brightness"
            value: "B"
            selected: true
        }
        Option {
            id: option2
            text: "Saturation"
            value: "S"
        }
        Option {
            id: option3
            text: "HighlightEffect"
            value: "H"
        }
    }
    
    Label {
        text: "Brightness"
        visible: (effectSelector.selectedValue == "B")
        textStyle.color: ui.palette.secondaryTextOnPrimaryDark
    }
    
    // Slider for selecting the brightness value
    Slider {
        //the valid range for brightness is -150 to 150
        id: brightSlider
        fromValue: -150.0
        toValue: 150
        visible: (effectSelector.selectedValue == "B")
    }
    
    Label {
        text: "Saturation"
        visible: (effectSelector.selectedValue == "S")
        textStyle.color: ui.palette.secondaryTextOnPrimaryDark
    }
    
    // Slider for selecting the saturation value
    Slider {
        //the valid range for saturation is  -100 to 100
        id: satSlider
        fromValue: -100.0
        toValue: 100.0
        visible: (effectSelector.selectedValue == "S")
    }
    
    // Radio group for selecting the hightlight style
    RadioGroup {
        id: focusRadio
        visible: (effectSelector.selectedValue == "H")
        Option { 
            text: "Flat"
            value: "flat"
            selected: true 
        }
        Option { 
            text: "LinearGradient"
            value: "grad"
        }
        
        effects: [
            FocusHighlightEffect {
                enabled: true
                baseColor: ui.palette.primaryDark
                style: FocusHighlightEffectStyle.Flat
            }
        ]
    }
    
    // Animations for creating expanding/collapsing
    // menu effect
    animations: [
        TranslateTransition {
            id: showMenu
            duration: 400
            fromY: offset
            toY: 0
            easingCurve: StockCurve.Linear
        },
        TranslateTransition {
            id: hideMenu
            duration: 400
            fromY: 0
            toY: offset
            easingCurve: StockCurve.Linear
        }
    ]
}

