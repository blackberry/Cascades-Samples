/* Copyright (c) 2013 Research In Motion Limited.
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

// A component that is used the first time the app starts, it
// display a simple instruction so that the use knows how to use the app.
Container {
    property bool hasBeenPressed: false
    signal hideInstruction()

    rightPadding: 40
    leftMargin: 40
    background: Color.create("#121212")
    opacity: 0.75
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill

    layout: DockLayout {
    }

    animations: FadeTransition {
        id: hideAnim
        fromOpacity: opacity
        toOpacity: 0
        duration: 300

        onEnded: {
            hideInstruction();
        }
    }

    Label {
        text: qsTr("Press screen for new pattern. Use \"Shoot Screen\" to set as wallpaper or share.")
        multiline: true
        horizontalAlignment: HorizontalAlignment.Center;
        verticalAlignment: VerticalAlignment.Center;
        textStyle.base: SystemDefaults.TextStyles.BigText;
        textStyle.color: Color.create("#fefefe");
        textStyle.fontStyle: FontStyle.Italic
    }
    
    onTouch: {
        if(!hasBeenPressed && event.touchType == TouchType.Down){
            hideAnim.play();
        }
        hasBeenPressed = true;
    }
}
