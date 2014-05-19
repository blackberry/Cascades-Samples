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
import com.rundgang 1.0 
import "../"

// The Rundgang tab is a page that sports a number of ways to send feedback;
// a photo (or bar code), a recorded sound or simply text. The topic (subject and adressList) 
// of the feedback can be changed from this view.
NavigationPane {
    id: navRundgang
    
    Page {
        objectName: "navPage"

        titleBar: TitleBar {
            title: qsTr("Rundgång") + Retranslate.onLocaleOrLanguageChanged
        }
        
        // The current topic that will be used to send the feedback data.
        Container {
            CurrentTopic {
                onCurrentTopicTriggered: {
                    pushTopicList();
                }
            }
        }

        actions: [
            // Triggers navigation to a Page for taken a photo that will be sent via email. 
            ActionItem {
                title: qsTr("Photoback") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///images/rundgang_camera.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                accessibility.description: qsTr("Take a photo or scan a bar code and send as feedback.")
                
                // A topic has to be set in order to send and store image feedback.
                enabled: !topicData.noTopics

                onTriggered: {
                    pushPhotoFeedbackPage()
                }
            },
            // Triggers navigation to a Page for recording a sound that will be sent via email.
            ActionItem {
                title: qsTr("Audioback") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///images/rundgang_microphone.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: !topicData.noTopics
                accessibility.description: qsTr("Record audio and send as feedback.");
                
                onTriggered: {
                    pushAudioFeedbackPage()
                }
            },
            // Triggers either navigation to a page for entering a text or a direct invocation
            // of the email card pre-populated with the current topic.
            ActionItem {
                title: qsTr("Textback") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///images/ic_text.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: isTextbackEnabled()
                accessibility.description: qsTr("Write text and send as feedback.");
                
                onTriggered: {
                    // Depending on the directTextEmail property the email card will
                    // either be launched right away or a Page will be pushed. The latter
                    // makes it possible to store the actual text in the history as well.
                    if(_appSettings.directTextEmail){
                        textFeedback("");    
                    } else {
                     	pushTextFeedbackPage();   
                    }
                }
                
                // Writing feedback has some extra restrictions. If the app is set to open the 
                // email card directly an email list is required.
                function isTextbackEnabled(){
                    if (topicData.noTopics) {
                        return false;
                    } else if (topicData.currentAdressList.length == 0 && _appSettings.directTextEmail) {
                        return false;
                    }

                    return true;
                }
            },
            // Navigates to a list with previously used topics.
            ActionItem {
                title: qsTr("Topics") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///images/rundgang_entry.png"
                enabled: !topicData.noTopics
                
            	onTriggered: {
                    pushTopicList();
                }
            },
            // Launch the TopicData sheet for adding a new topic item.
            ActionItem {
                title: qsTr("New Topic") + Retranslate.onLocaleOrLanguageChanged
                imageSource: "asset:///images/rundgang_add_entry.png"
                
                onTriggered: {
                    topicData.newTopic();
                }
            }
        ]
    }

    attachedObjects: [
        ComponentDefinition {
            id: audioFeeddbackPageDef
            source: "asset:///Rundgang/AudioPage.qml"      
        },
        ComponentDefinition {
            id: photoFeedbackPageDef
            source: "asset:///Rundgang/PhotoPage.qml"
        },
        ComponentDefinition {
            id: textFeedbackPageDef
            source: "asset:///Rundgang/TextPage.qml"
        },
        ComponentDefinition {
            id: topicListDef
            source: "asset:///Rundgang/TopicList.qml"       
        },
        // A custom object for email control. 
        EmailController{
            id: emailController
        }
    ]

    onPopTransitionEnded: {
        page.destroy();
    }
    
    shortcuts: [
        // The create new short cut shows the data source add topic sheet.
        SystemShortcut {
            type: SystemShortcuts.CreateNew
            onTriggered: {
                topicData.newTopic();
            }
        }
    ]
    
    // Navigation functions below.
    function pushTopicList() {
        if(topicData.noTopics) {
            // If no topics have been added show the TopicData view for adding a topic.
            topicData.newTopic();
        } else {
            // Otherwise show a list with all topics in the TopicData Model.
            var topicList = topicListDef.createObject();
            topicList.listModel = topicData.model;
            
            // Connect list signals to the Data object that handles the datasource and model.
            topicList.addTopic.connect(topicData.newTopic);
            topicList.deleteTopics.connect(topicData.deleteTopics);
            topicList.topicSelected.connect(topicData.selectTopic);
            
            // When a topic is selected automatically navigate back 
            topicList.topicSelected.connect(navRundgang.pop);
            
            // Push the list Page.
            navRundgang.push(topicList);
        }
    }
    
    function pushPhotoFeedbackPage() {
        var page = photoFeedbackPageDef.createObject();
        navRundgang.push(page);
        page.rescalePhotos = _appSettings.loresPhoto;
        
        // Connect signals
        page.feedback.connect(navRundgang.photoFeedback);
        page.feedbackCode.connect(navRundgang.codeFeedback);
        page.feedback.connect(navRundgang.pop);
        page.feedbackCode.connect(navRundgang.pop);
    }
    
    function pushAudioFeedbackPage() {
        var page = audioFeeddbackPageDef.createObject();
        navRundgang.push(page);
        
        // Connect signals
        page.feedback.connect(navRundgang.audioFeedback);
        page.feedback.connect(navRundgang.pop);
    }
    
    function pushTextFeedbackPage() {
        var page = textFeedbackPageDef.createObject();
        navRundgang.push(page);
        
        // Set the text area to be focused immediately.
        page.readyForFocus = true;
        
        // Connect signals
        page.feedback.connect(navRundgang.textFeedback);
        page.feedback.connect(navRundgang.pop);
    }
    
    // Functions for setting up a map with content that can be pushed to the email composer.
    function audioFeedback(fileName) {
        var map = {};
        map["attachment"] = fileName;
        navRundgang.startEmailCard(map, "audio", "");
    }
    
    function photoFeedback(fileName) {
        var map = {};
        map["attachment"] = fileName;
        navRundgang.startEmailCard(map, "image", "");
    }
    
    function codeFeedback(codeData) {
        var map = {};
        navRundgang.startEmailCard(map, "qr", codeData);
    }
    
    function textFeedback(text) {
        var map = {};
        navRundgang.startEmailCard(map, "text", text);
    }
    
    function startEmailCard(data, type, additionalData) {
        // Set up the adressList of the QVariangMap that will be sent to
        // the email composer. 
        var adressList = topicData.currentAdressList.split(",");        
        
        // To is a reserved word in the data structure for invoking the email browser.
        data["to"] = [];
        for (var i = 0; i < adressList.length; i ++) {
            data["to"][i] = adressList[i];
        }
        
        // Set the "subject" of the email to be the current feedback topic.
        var topic = topicData.currentTopic; 
        data["subject"] = topic;
        
        // Add addtionalData to the email body.
        data["body"] = additionalData;
        
        // Add auto signature if one has been set in settings.
        if(_appSettings.autoEmailSignature){
            data["body"] = data["body"] + "\n\n\n" + _appSettings.emailSignature;
        }
        
        // Add the topic to the history database.
        historyData.addHistory(data, topicData.currentAdressList, type, additionalData);
        
        if(topicData.currentAdressList.length != 0){
            // Only Invoke the email composer card if there are addresses with the data,
            // the item is still saved to history might be a "local" feedback.
            emailController.invokeEmailComposer(data);            
        }
    }
}