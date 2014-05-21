/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import "Common"

// This recipe illustrates the usage of Invocation and InvokeActionItem.
RecipePage {
    id: invokePage

    RecipeContainer {
        id: invokeContainer
        property variant pendingInvocation
        
        topPadding: ui.du(2)
        leftPadding: ui.du(2)
        rightPadding: ui.du(2)

        InvocationRecipeShoppingList {
            id: shoppingList

            onSelectedInvocationOptionChanged: {
                // Change which web page should be opened by the InvokeActionItem.
                infoItem.query.uri = href;

                // For the query to actually update a call to the updateQuery function has to be made.
                infoItem.query.updateQuery();
            }

            onInvoke: {
                // Update the query with new information from the relevant field, when
                // updateQuery is called the query will get re-armed and once that is done the
                // Invocation is ready to be triggered with updated information.
                hasPendingInvocation = false;

                if (value === "email") {
                    emailShare.query.uri = "mailto:" + shoppingList.adress + "?subject=" + shoppingList.title + "&body=" + shoppingList.message
                    emailShare.query.updateQuery();
                    invokeContainer.pendingInvocation = emailShare;
                    hasPendingInvocation = true;
                } else if (value === "unbound") {
                    unboundShare.query.data = shoppingList.title + "\n" + shoppingList.message
                    unboundShare.query.updateQuery();
                    invokeContainer.pendingInvocation = unboundShare;
                    hasPendingInvocation = true;
                } else if (value === "bbm") {
                    bbmShare.query.data = shoppingList.title + "\n" + shoppingList.message
                    bbmShare.query.updateQuery();
                    invokeContainer.pendingInvocation = bbmShare;
                    hasPendingInvocation = true;
                }
            }
        }

        function launchPendingInvokation() {
            if (shoppingList.hasPendingInvocation) {
                invokeContainer.pendingInvocation.trigger(invokeContainer.pendingInvocation.query.invokeActionId);
            }
            shoppingList.hasPendingInvocation = false;
        }
    }

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
            query.data: shoppingList.title + "\n" + shoppingList.message

            onArmed: {
                invokeContainer.launchPendingInvokation();
            }
        },
        Invocation {
            id: bbmShare

            // Query for sharing on BBM, notice that the invokeTargetId has been set here.

            query.mimeType: "text/plain"
            query.invokeTargetId: "sys.bbm.sharehandler"
            query.invokeActionId: "bb.action.SHARE"
            query.data: shoppingList.title + "\n" + shoppingList.message

            onArmed: {
                invokeContainer.launchPendingInvokation();
            }
        },
        Invocation {
            id: emailShare

            // A query for a pre-populated email.
            query.mimeType: "text/plain"
            query.invokeTargetId: "sys.pim.uib.email.hybridcomposer"
            query.invokeActionId: "bb.action.SENDEMAIL"

            // The uri follows the specification set up in documentation.
            query.uri: "mailto:someemailadress@cookbook.xyz?subject=" + shoppingList.title + "&body=+" + shoppingList.message

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
}
