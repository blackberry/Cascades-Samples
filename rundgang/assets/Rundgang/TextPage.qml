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
import bb.cascades 1.3

// A page for entering text feedback data.
Page {
    property bool readyForFocus: false
    
    // Emitted when the entered text is expected to be used.
    signal feedback(string text);
    
    titleBar: TitleBar {
        title: qsTr("Text Feedback") + Retranslate.onLocaleOrLanguageChanged
         scrollBehavior: TitleBarScrollBehavior.Sticky 
    }

    Container {
        topPadding: ui.du(2)
        leftPadding: topPadding
        rightPadding: topPadding
        bottomPadding: topPadding
        
        TextArea {
            id: feedbackArea
            hintText: qsTr("Enter feedback text") + Retranslate.onLocaleOrLanguageChanged
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            onTextChanging: {
                if(text != ""){
                    sendButton.enabled = true;
                } else {
                    sendButton.enabled = false;
                }
            }
        }
        
        Button {
            id: sendButton
            text: qsTr("Send") + Retranslate.onLocaleOrLanguageChanged
            enabled: false
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
            onClicked: {
                // Emit the feedback signal. 
                feedback(feedbackArea.text);
            }
        }
    }
    onReadyForFocusChanged: {
        if (readyForFocus) {
            // This triggers focus if the Page is created using a ComponentDefinition.
            feedbackArea.requestFocus();
        }
    }
}
