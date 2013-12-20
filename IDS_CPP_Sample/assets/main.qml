/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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
import bb.IDS_QT_Sample 1.0

Page {
    Container {
        //! [0]
        Label {
            id: welcomeLabel
            text: "Please log in with your BlackBerry Id"
            textStyle.base: SystemDefaults.TextStyles.BigText
            onCreationCompleted: {
                blackBerryIdProvider.getUserInfo();
            }
        }
        Button {
            id: pressHereButton
            text: qsTr("Press Here") + Retranslate.onLocaleOrLanguageChanged
            onClicked: {
                resultsLabel.text = "Updating press count";
                blackBerryProfileProvider.incrementPressCount();
            }
        }
        Label {
            id: pressCountLabel
            text: ""
            visible: false

            onCreationCompleted: {
                blackBerryProfileProvider.getPressCount();
            }
        }
        Label {
            id: resultsLabel
            text: qsTr("Profile results") + Retranslate.onLocaleOrLanguageChanged
            textStyle.base: SystemDefaults.TextStyles.SmallText
        }
        //! [0]
    }
    attachedObjects: [
        //! [1]
        BlackBerryIdProvider {
            id: blackBerryIdProvider
            onGetUserInfoSuccess: {
                welcomeLabel.text = "Welcome " + userProperties[0].value;
            }
            onGetUserInfoFailed: {
                resultsLabel.text = "Failed to retrieve user info: " + errorMessage;
            }
        },
        //! [1]
        //! [2]
        BlackBerryProfileProvider {
            id: blackBerryProfileProvider
            
            onPressCountRetrieved: {
                resultsLabel.text = "Successfully retrieved press count";
                pressCountLabel.text = "Pressed " + data + " times";
                pressCountLabel.visible = true;
            }
            onPressCountRetrievalFailed: {
                resultsLabel.text = "Failed to retrieve press count: " + errorMessage;
            }
            onPressCountUpdated: {
                resultsLabel.text = "Retrieving press count";
                blackBerryProfileProvider.getPressCount();
            }
            onPressCountUpdateFailed: {
                resultsLabel.text = "Failed to update press count: " + errorMessage;
            }
            onPressCountCreated: {
                blackBerryProfileProvider.getPressCount();
            }
            onPressCountCreateFailed: {
                resultsLabel.text = "Failed to create press count: " + errorMessage;
            }
        }
        //! [2]
    ]
}
