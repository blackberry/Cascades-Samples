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

// The Container with the speed text and the text input control.
Container {
    // Properties for the scrolling speed text.
    property alias currentLine: speedTextScroller.line
    property alias remainingText: speedTextScroller.remainingText
    property alias speedText: speedTextScroller.text
    
    // Validation can be set on the input part of the Component.
    property alias isSpeedTextValid: speedTextInput.validSpeedText
    
    // State properties of the component.
    property bool ended: false
    property int averageSpeed: 0

	// Signal emitted as the user types in the input field.    
    signal inputTextChanging(string inputText);
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    Container {
        layoutProperties: StackLayoutProperties {
            spaceQuota: 2
        }
        
        SpeedTextScroller {
            id: speedTextScroller
            preferredWidth: ui.du(72.8)
            preferredHeight: ui.du(16.5)
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        // A text field used for text input
        SpeedTextInput {
            id: speedTextInput
            topMargin: ui.du(3)
            preferredWidth: ui.du(71.8)
            horizontalAlignment: HorizontalAlignment.Center
            enabled: !ended
            
            onTextChanging: {
                if(!ended)
            	   inputTextChanging(text);
            }
        }
    }
    
    onCurrentLineChanged: {
        // When one line has been entered correctly the speedTextInput is cleared
        // to make room for entering the next line.
        speedTextInput.text = "";
        speedTextInput.hintText = "";
    }

    onEndedChanged: {
        if(ended) {
            // The game is over, set up a text for displaying the final result in the text label and text area.
            speedText = "Your speed was " + averageSpeed + " words/min.\nWell done!";

            // Scroll to top to show the result in the scrolling area.
            speedTextScroller.scrollToLine(0);
            speedTextInput.text = "";
        }
    }
}
