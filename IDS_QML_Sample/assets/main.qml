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
import bb.IDS_QML_Sample 1.0

Page {
    id: mainPage
    property int pressCount;
    Container {
        Label {
            id: welcomeLabel
            text: "Please log in with your BlackBerry Id"
            textStyle.base: SystemDefaults.TextStyles.BigText
            //! [0]
            onCreationCompleted: {
                mainPage.pressCount = 0;
                // Register to use BlackBerryId and request users information
                var result = blackBerryIdProvider.setProvider(blackBerryId.providerName);
                if( result != IdentityServiceResult.Success ) {
                    // There was an error registering BlackBerryId - handle appropriately
                    // calling error() for more information about the failure.
                    resultsLabel.text = "Unable to communicate with BlackBerryId";
                } else {
                    // Successfully registered BlackBerryId - request user properties
                    if( blackBerryIdProvider.requestProperties(BlackBerryIdPropertyType.Core, blackBerryId.firstNamePropertyKey) == 0 ) {
                        resultsLabel.text = "Failed to get user properties: " + blackBerryIdProvider.error();                        
                    }
                }

                // Register to use BlackBerryProfile and request pressCount
                result = blackBerryProfileProvider.setProvider(blackBerryProfile.providerName);
                if( result != IdentityServiceResult.Success ) {
                    // There was an error registering BlackBerryProfile - handle appropriately
                    // calling error() for more information about the failure.
                    resultsLabel.text = "Unable to communicate with BlackBerryProfile";
                } else {
                    // Successfully registered BlackBerryProfile - request press count.
                    if( blackBerryProfileProvider.requestData(BlackBerryProfilePropertyType.Application, BlackBerryProfileGetDataFlag.CacheData, "pressCount") == 0) {
                        resultsLabel.text = "Failed to get data: " + blackBerryProfileProvider.error()
                    }
                }
            }
            //! [0]
        }
        //! [1]
        Button {
            id: pressHereButton
            text: qsTr("Press Here") + Retranslate.onLocaleOrLanguageChanged
            onClicked: {
                mainPage.pressCount++;
                if( blackBerryProfileProvider.setData(BlackBerryProfilePropertyType.Application, BlackBerryProfileGetDataFlag.CacheData, "pressCount", mainPage.pressCount) == 0) {
                    // There was an error calling setData, call error() for more information
                    resultsLabel.text = "Failed to set data: " + blackBerryProfileProvider.error()
                } else {
                    resultsLabel.text = "Updating press count";
                }
            }
        }
        //! [1]
        Label {
            id: pressCountlabel
            text: ""
            visible: false
        }
        Label {
            id: resultsLabel
            text: qsTr("Profile Results") + Retranslate.onLocaleOrLanguageChanged
            textStyle.base: SystemDefaults.TextStyles.SmallText
        }
    }
    attachedObjects: [
        //! [2]
        IdentityServiceProvider{
            id:blackBerryIdProvider
            onPropertiesRetrieved: {
                welcomeLabel.text = "Welcome " + userProperties[blackBerryId.firstNamePropertyKey];
            }
            onPropertyRetrievalFailed:{
                resultsLabel.text = "Failed to retrieve user info: " + result
            }
        },
       
        IdentityServiceProvider{
            id:blackBerryProfileProvider
            onDataRetrieved: {
                pressCount = data;
                pressCountlabel.text = "Press Count: " + data
                pressCountlabel.visible = true;
            }
            onDataRetrievalFailed: {
                if( result == 50007 ) {
                    if( blackBerryProfileProvider.createData(BlackBerryProfilePropertyType.Application, BlackBerryProfileCreateDataFlag.CacheData, "pressCount", mainPage.pressCount) == 0 ) {
                        // There was an error calling createData, call error() for more information
                        resultsLabel.text = "Failed to create data: " + blackBerryProfileProvider.error();
                    } else {
                        resultsLabel.text = "Creating press count";
                    }
                } else if( result == IdentityServicePropertyResult.UserCouldNotBeAuthenticated ) {
                    resultsLabel.text = "You didn't login to BBID :(";
                } else {
                    resultsLabel.text = "Unable to retrieve press count: " + result;
                }
            }
            onDataCreated: {
                if( blackBerryProfileProvider.requestData(BlackBerryProfilePropertyType.Application, BlackBerryProfileGetDataFlag.CacheData, "pressCount") == 0 ) {
                    // There was an error calling requestData, call error() for more information
                    resultsLabel.text = "Failed to get data: " + blackBerryProfileProvider.error();
                } else {
                    resultsLabel.text = "Press count created";
                }
            }
            onDataCreateFailed: {
                resultsLabel.text = "Unable to create press count: " + result;
            }
            onDataSet: {
                if( blackBerryProfileProvider.requestData(BlackBerryProfilePropertyType.Application, BlackBerryProfileGetDataFlag.CacheData, "pressCount") == 0 ) {
                    // There was an error calling requestData, call error() for more information
                    resultsLabel.text = "Failed to get data: " + blackBerryProfileProvider.error();
                } else {
                    resultsLabel.text = "Press count updated";
                }
            }
            onDataSetFailed: {
                resultsLabel.text = "Unable to update press count: " + result;
            }

        },
        //! [2]
        //! [3]
        BlackBerryId{
            id: blackBerryId
        },
        BlackBerryProfile{
            id: blackBerryProfile
        }
        //! [3]
    ]
}
