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

// A page helping the user understand how the application is used.
Page {
    titleBar: TitleBar {
        title: qsTr("Rundgång") + Retranslate.onLocaleOrLanguageChanged
    }

    ScrollView {

        Container {
            topPadding: ui.du(2)
            rightPadding: topPadding
            leftPadding: topPadding
            bottomPadding: topPadding

            Label {
                multiline: true
                text: qsTr("<html>Welcome to Rundgång! This manual contain some information" + 
                "on how to use this application. First up the name, Rundgång is swedish for" +
                " feedback. Well actually the word refers to the kind of feedback one gets" +
                " when the sound guy is not doing his job properly.\n</html>")
            }
            
            Label {
                multiline: true
                text: qsTr("<html><span style='font-size:x-large'>Feedback</span> \n" + 
                "On the start screen you have three different actions, these are different" +
                " ways of record data that you want to feedback to a group of people.\n</html>")
            }
            
            Label {
                multiline: true
                text: qsTr("<html><span style='font-size:x-large'>People &amp; Topic</span> \n" + 
                "The group of people and the topic of the feedback that are currently selected is" + 
                " presented on the home screen and a list of different topics is available either pressing the item" + 
                " or going action item overflow." + "\n\nHappy feedbacking.</html>")
            }
        }
    }
}