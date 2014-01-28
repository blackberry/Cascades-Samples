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
import bb.cascades 1.0
import bb.data 1.0
import bb.system 1.0

Tab {
    id: tab

    property variant template
    property variant navPaneAction
    property alias webcontainer: webcontainer

    title: qsTr(tab.template.name) + Retranslate.onLanguageChanged
    imageSource: tab.template.icon

    onTemplateChanged: {
        if (template === undefined)
            return;

        for (var i = 0; i < tab.template.actions.length; i ++) {
            var action = actionDef.createObject();
            action.template = tab.template.actions[i];

            action.navPane = navPane;
            page.addAction(action, ActionBarPlacement.OnBar);
        }
    } // onTemplateChanged

    attachedObjects: [
        ComponentDefinition {
            id: actionDef
            TemplateActionView {
            }
        },

        Phone {
            id: phone
        },

        ComponentDefinition {
            id: locationsPage
            source: "asset:///Locations.qml"
        }

    ] // attachedObjects

    NavigationPane {
        id: navPane

        onPopTransitionEnded: {
            page.destroy()
        }

        Page {
            id: page

            WebContainer {
                id: webcontainer
            }

            // actions that will always be in the app - static
            actions: [

                // Maps
                ActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    imageSource: "asset:///images/navigate.png"
                    title: qsTr("Directions") + Retranslate.onLanguageChanged
                    onTriggered: {

                        if (init.locationHandler === 1) {
                            invokeMap(init.locationsArray[0]);

                        } else {
                            tab.navPaneAction = "directions";
                            init.listViewArray = locationsArray;
                            navPane.push(locationsPage.createObject());
                        }
                    }
                },

                // Phone
                ActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Phone") + Retranslate.onLanguageChanged
                    imageSource: "asset:///images/phone.png"
                    onTriggered: {

                        if (init.locationHandler === 1) {
                            invokePhone(init.phoneNumbersArray[0]);

                        } else {
                            tab.navPaneAction = "phone";
                            init.listViewArray = phoneNumbersArray;
                            navPane.push(locationsPage.createObject());
                        }
                    }
                },

                // BBM Channel
                InvokeActionItem {
                    title: "BBM Channel"
                    query {
                        invokeTargetId: "sys.bbm.channels.card.previewer"
                        invokeActionId: "bb.action.OPENBBMCHANNEL"
                        uri: "bbmc:" + appSettings.bbmChannel
                    }
                },

                // Invite to download
                ActionItem {
                    title: qsTr("Invite To Download") + Retranslate.onLanguageChanged
                    imageSource: "asset:///images/bbm.png"
                    enabled: bbmHandler.allowed
                    onTriggered: {
                        bbmHandler.sendInvite();
                    }
                },

				// share
                InvokeActionItem {
                    query {
                        mimeType: "text/plain"
                        invokeActionId: "bb.action.SHARE"
                    }
                    onTriggered: {
                        data = appSettings.shareText;
                    }
                },

                // Facebook
                ActionItem {
                    imageSource: "asset:///images/facebook.png"
                    title: qsTr("Facebook") + Retranslate.onLanguageChanged
                    onTriggered: {
                        _socialInvocation.invoke("com.rim.bb.app.facebook", "bb.action.OPEN", "page", appSettings.facebook, "")
                    }
                },

                // Twitter
                ActionItem {
                    title: qsTr("Twitter") + Retranslate.onLanguageChanged
                    imageSource: "asset:///images/twitter.png"
                    onTriggered: {
                        _socialInvocation.invoke("com.twitter.urihandler", "bb.action.VIEW", "*", "twitter:connect:" + appSettings.twitter, "")
                    }
                },

                // Checkin via Foursquare
                ActionItem {
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    imageSource: "asset:///images/foursquare.png"
                    title: qsTr("Check-in with Foursquare") + Retranslate.onLanguageChanged

                    onTriggered: {
                        if (init.locationHandler === 1) {
                            invokeFoursquare(init.foursquareArray[0]);

                        } else {
                            tab.navPaneAction = "foursquare";
                            init.listViewArray = foursquareArray;
                            navPane.push(locationsPage.createObject());
                        }
                    }
                }
            ] // actions
        } // Page
    } // NavigationPane

    function invokeMap(data) {
        app.invokeMap(data.address);
    }

    function invokePhone(data) {
        phone.initiateCall(data.phone);
    }

    function invokeFoursquare(data) {
        app.invokeFoursquare(data.foursquare);
    }

}// Tab