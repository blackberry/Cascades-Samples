/* Copyright (c) 2013 BlackBerry Limited.
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

Sheet {
    id: helpSheet
    content: Page {
        titleBar: TitleBar {
            title: qsTr("Help") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    helpSheet.close()
                    if (helpSheet) helpSheet.destroy();
                }
            }
        }
        Container {
           	leftPadding: 4.0
           	rightPadding: 4.0           	
            Label {
                text: qsTr("This application serves as a starting point for creating Built For BlackBerry applications.") + Retranslate.onLanguageChanged
                multiline: true
            }
            Label {
                text: qsTr("To go beyond the code that is provided here, visit our documentation at https://developer.blackberry.com/native/") + Retranslate.onLanguageChanged
                multiline: true
                content.flags: TextContentFlag.ActiveText
            }
            Label {
                text: qsTr("Or, find sample code at https://github.com/blackberry/Cascades-Samples") + Retranslate.onLanguageChanged
                multiline: true
                content.flags: TextContentFlag.ActiveText
            }
        }

    }
}
