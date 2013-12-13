/* Copyright (c) 2012 BlackBerry Limited.
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
import "Common"

// This recipe illustrates the usage of Invocation and InvokeActionItem.
RecipePage {
    id: invokePage

    RecipeContainer {
        id: invokeContainer
        
        topPadding: 20
        leftPadding: 20
        rightPadding: 20
        
        layout: StackLayout {
        }
        DropDown {
            title: "Delivery method"
            id: invokeSelection
            options: [
                Option {
                    // Each option has a string to update which web page should be open by the infoItem InvokeActionItem.
                    property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/sending_invocation.html"
                    
                    // The id of the Invocation set up as an attached object below.
                    value: unboundShare
                    
                    description: "Don't know"
                    selected: true
                },
                Option {
                    property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/bbm.html"
                    value: bbmShare
                    description: "BBM"
                },
                Option {
                    property string href: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/email.html"
                    value: emailShare
                    description: "Email"
                }
            ]
            
            onSelectedOptionChanged: {
                // Change which web page should be opened by the InvokeActionItem.
                infoItem.query.uri = selectedOption.href;
                
                // For the query to actually update a call to the updateQuery function has to be made.
                infoItem.query.updateQuery();
            }
            
            onSelectedValueChanged: {
                // As the selected value changes we show and hide certain text fields .
                adress.visible = false

                if(selectedValue == emailShare) {
                    adress.visible = true
                }
            }
        }

        Label {
            id: title
            text: "Shopping List"
        }

        TextField {
        	id: adress
        	text: "someemailaddress@cookbook.xyz"
        	hintText: "Email address"        	
        	inputMode: TextFieldInputMode.EmailAddress
        	
            // The email address field is initially hidden, only to be shown when the Email option is selected in the DropDown.
            visible: false
        }
                
        TextArea {
            id: message
            hintText: "Enter items to buy"
            text:"Chocolate\n"+
            "Fudge\n"+
            "Turkish Delight"
        }

        Button {
            id: invokeButton
            property bool hasPendingInvocation: false
            
            text: "Invoke!"
            enabled: !hasPendingInvocation
            
            onClicked: {
                // As the button is clicked the invoke query is changed this takes
                // a little bit of time and its not until the Invocation is armed again
                // it is possible to actually trigger the invocation.
                hasPendingInvocation = invokeContainer.updateQueries();
            }
        }

        function updateQueries(){
            // Update the query with new information from the relevant field, when 
            // updateQuery is called the query will get re-armed and once that is done the 
            // Invocation is ready to be triggered with updated information.
            if(invokeSelection.selectedOption.value == emailShare) {
                emailShare.query.uri = "mailto:" + adress.text + "?subject=" + title.text + "&body=" + message.text
                emailShare.query.updateQuery();
                return true;
            } else if(invokeSelection.selectedOption.value == unboundShare) {
                unboundShare.query.data = title.text + "\n" + message.text
                unboundShare.query.updateQuery();
                return true;
            } else if(invokeSelection.selectedOption.value == bbmShare) {
                bbmShare.query.data = title.text + "\n" + message.text
                bbmShare.query.updateQuery();
                return true;
            }

            return false;
        }
        
        function launchPendingInvokation(){
            if(invokeButton.hasPendingInvocation){
            	invokeSelection.selectedOption.value.trigger(invokeSelection.selectedOption.value.query.invokeActionId);
            }
            invokeButton.hasPendingInvocation = false;
        }
    } // RecipeContainer
        
    // The Invocations are non-UI objects and as such they are added 
    // to QML as attachedObjects.
    attachedObjects: [
        Invocation {
            id: unboundShare

			// The first invocation is an unbound share. By not setting a invokeTargetId
			// a specific application will not be targeted instead the user will be presented
			// with a list of apps that can handle the mime type and the share action.
            query.mimeType: "text/plain"
            query.invokeActionId: "bb.action.SHARE"

            // The data property of the query is a QByteArray, the conversion from a string
            // like we have here into a QByteArray is not flawless. To be absolutely sure that
            // all characters are correctly encoded it might be necessary to do the appropriate 
            // conversion in C++ (check out QString::toUtf8()).
            query.data: title.text + "\n" + message.text

            onArmed: {
                invokeContainer.launchPendingInvokation();
            }
            
        },
        Invocation {
            id: bbmShare
            
            // Query for sharing on BBM, notice that the invokeTargetId has been set here.
            query {
                mimeType: "text/plain"
                invokeTargetId: "sys.bbm.sharehandler"
                invokeActionId: "bb.action.SHARE"
                data:  title.text + "\n" + message.text
            }
            onArmed: {
                invokeContainer.launchPendingInvokation();
            }
        },
        Invocation {
            id: emailShare
            
            // A query for a pre-populated email.
            query {
                mimeType: "text/plain"
                invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                invokeActionId: "bb.action.SENDEMAIL"
                
                // The uri follows the specification set up in documentation.
                uri: "mailto:someemailadress@cookbook.xyz?subject="+title.text+"&body=+"+message.text
            }
            onArmed: {
                invokeContainer.launchPendingInvokation();
            }
        }
    ]
    
    actions: [
        // An invoke action item that launches the browser with the documentation page of the currently selected Invocation.
        InvokeActionItem {
            id: infoItem
            title: "More info"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            query.mimeType: "text/html"
            query.invokeTargetId: "sys.browser"
            query.uri: "http://developer.blackberry.com/cascades/documentation/device_platform/invocation/sending_invocation.html"
            query.invokeActionId: "bb.action.OPEN"

        }
    ]
}// RecipePage
