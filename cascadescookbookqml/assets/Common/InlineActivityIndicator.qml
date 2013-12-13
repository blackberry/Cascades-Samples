/* Copyright (c) 2012 BlackBerry Limited.
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
    property alias indicatortext: indicatorText.text

    // Make it possible to change padding and height outside of this file.
    // The default values  set in this file suites 768x1280.
    property alias aiPreferredHeight: recipeIndicatorCon.preferredHeight
    property int padding: 20

    id: recipeIndicatorCon
    rightPadding: padding
    leftPadding: padding
    preferredHeight: 140
    
    // Need to set the right color dependent on theme.
    background: setBgColor(Application.themeSupport.theme.colorTheme.style)
    verticalAlignment: VerticalAlignment.Bottom
    horizontalAlignment: HorizontalAlignment.Fill
    visible: false
    
    layout: DockLayout {
    }

    Label {
        id: indicatorText
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Center
        text: "Default text"
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
    }

    // The activity indicator control.
    ActivityIndicator {
        id: indicator
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Right
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
