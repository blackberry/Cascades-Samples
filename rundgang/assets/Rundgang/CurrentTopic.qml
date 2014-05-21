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

TopicItem {
    id: topicItem
    topic: topicData.currentTopic
    adressList: topicData.currentAdressList
    signal currentTopicTriggered();
    
    onTouch: {
        if (event.touchType == TouchType.Up) {
            currentTopicTriggered();
        }
    }
    
    // A CustomA11yObject is used so that onTouch above can be translated to a Button Behavior.
    accessibility: CustomA11yObject {
        role: A11yRole.Button
        // The A11y information depends on if there already are topics or not.
        name: (topicData.noTopics == true) ? "Tap to add your first topic." : qsTr("Current topic: ")+ topicItem.topic + ", " + topicItem.adressList + ".";
        description: (topicData.noTopics == true) ? qsTr("") : qsTr("Tap to navigate to list of topics.");
        
        ComponentA11ySpecialization {
            onActivationRequested: {
                if (event.type == A11yComponentActivationType.Tap) {
                    currentTopicTriggered();
                }
            }
        }    
    }
}
