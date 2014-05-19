/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

// Import all our cascades functions.
import bb.cascades 1.3

Page {
    // A container is used for grouping controls.
    Container {

        // A cozy warm white color is used as background.
        background: Color.create("#F2EDEA");

        // Padding adds a bit of space between the screen edge and the UI controls.
        topPadding: ui.du(3)
        leftPadding: ui.du(3)

        // The bubble image.
        ImageView {
            imageSource: "asset:///images/hello_image.png"
        }

        // A text label with the comforting hello world text.
        Label {
            // The label text is set using the qsTr() function to support translation.
            // and Retranslate.onLanguageChanged will force the string to update
            // if language change happens while the app is running.
            text: qsTr("hello world") + Retranslate.onLanguageChanged

            // The label text style.
            textStyle.base: SystemDefaults.TextStyles.BigText
            textStyle.color: Color.create("#085861")
        }
    }
}