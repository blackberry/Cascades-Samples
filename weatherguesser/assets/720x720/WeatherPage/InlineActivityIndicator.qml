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

Container {
    // Make it possible to set the label text from the qml where this component is used.
    property alias indicatorText: indicatorText.text
    
    // Start the animation and make it visible without using functions.
    // Possible to set the properties directly.
    property alias indicatorRunning: indicator.running
    property alias indicatorVisble: recipeIndicatorCon.visible 
    
    // Make it possible to change padding and height outside of this file.
    // The default values  set in this file suites 768x1280.
    property alias aiPreferredHeight: recipeIndicatorCon.preferredHeight
    property int padding: 10
    
    id: recipeIndicatorCon
    preferredHeight: 130
    layout: DockLayout {
    }
    // Need to set the right color dependent on theme.
    background: setBgColor(Application.themeSupport.theme.colorTheme.style);
    verticalAlignment: VerticalAlignment.Bottom
    horizontalAlignment: HorizontalAlignment.Fill
    visible: false 
    
    Container {
        id: labelContainer
        layout: DockLayout {
        }
        leftPadding: padding
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Center 
        
        Label {
            id: indicatorText
            text: "Default text"
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
        }
    }
    
    Container {
        id: indicatorContainer
        rightPadding: padding
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Right
        
        // The activity indicator control.
        ActivityIndicator { 
            id: indicator
        }
    }
    
    // Functions used for making the activity indicator visible and start the aniamtion.
    function startIndicator () {
        recipeIndicatorCon.visible = true;
        indicator.start();
    }
    
    function stopIndicator () {
        indicator.stop();
        recipeIndicatorCon.visible = false;
    }
    
    // Function used for setting background color dependent on theme.
    function setBgColor(style){
        switch(style) {
            case VisualStyle.Bright:    return Color.create("#e6f8f8f8")
            case VisualStyle.Dark:      return Color.create("#ff000000")
        }
        return Color.create("#e6f8f8f8")
    }
}