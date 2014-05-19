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
import bb.multimedia 1.0
import com.rundgang 1.0

// A page for recording feedback from the microphone.
Page {
    id: audioPage
    signal feedback(string fileName)
    
    titleBar: TitleBar {
        title: qsTr("Record audio feedback") + Retranslate.onLocaleOrLanguageChanged
        scrollBehavior: TitleBarScrollBehavior.Sticky;  
    }

    Container {
        id: mainContainer
        layout: DockLayout {
        }
        bottomPadding:  ui.du(3) 
        rightPadding: bottomPadding
        
        Button {
            id: recordButton
            property string fixedUrl
            property bool recording: false
            property variant recordBackground: Color.create("#850606");
            
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Bottom
            text: qsTr("Record") + Retranslate.onLocaleOrLanguageChanged
            accessibility.description: qsTr("Press to record, press again to stop recording.")
            color: Color.create("#cc1212")

            onClicked: {
                if (!recording) {
                    // Set up the output url and start recording.
                    recorder.outputUrl = audioController.dirPaths.voice + guid() + ".m4a";
                    recorder.record();
                    text = qsTr("Stop");
                    mainContainer.background = recordBackground;
                    recording = true;
                    accessibility.description = qsTr("Press to stop recording.")
                } else {
                    // Stop recording. Get the url and emit a signal that the recorded data is stored at that location.
                    recorder.reset();
                    fixedUrl = recorder.outputUrl;
                    feedback(fixedUrl.substring(7))
                    text = qsTr("Record")
                    mainContainer.background = SystemDefaults.Paints.ContainerBackground                    
                    recording = false;
                    accessibility.description = qsTr("Press to record, press again to stop recording.")
                }
            }
        }
    }
    
    attachedObjects: [
        // The AudioRecorder that can record audio from the microphone.
        AudioRecorder {
            id: recorder
            outputUrl: audioController.dirPaths.voice + guid() + ".m4a"
        },
        // Custom controller object see rundgang/audiocontroller.h
        AudioController {
            id: audioController
        }
    ]
    
    onCreationCompleted: {
        // Prepare the Audio recorder and make sure no error is detected.
        var error = recorder.prepare();
        if(error == MediaError.SourceUnavailable ) {
            // The mic is not available disable the record button and show an information hud 
            // that the user need to set permission to use the mic.
            displayPermissionError(qsTr("Audio recording via the microphone is not working. Could it be that the permission has not been given to use it?"))
            recordButton.enabled = false;
        }
        recorder.reset();
    }

	// Function used to set up a file name for the sound file.
	function s4() {
	    return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
	}
	
    // Function used to set up a file name for the sound file.
	function guid() {
	    return s4() + s4() + '-' + s4() + '-' + s4() + '-' + s4() + '-' + s4() + s4() + s4();
	}
	
}// Page
