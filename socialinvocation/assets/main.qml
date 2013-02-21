/* Copyright (c) 2013 Research In Motion Limited.
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

TabbedPane {
    showTabsOnActionBar: false
    Tab {
        title: qsTr("All Targets")
        Page {
            titleBar: TitleBar {
                title: qsTr("Social Invocation")
            }

            //! [0]
            actions: [
                // General SHARE Framework call
                // Will display all the SHARE Targets available
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    query {
                        mimeType: "text/plain"
                        invokeActionId: "bb.action.SHARE"
                    }
                    onTriggered: {
                        data = _socialInvocation.encodeQString(generalShare.statusText);
                        console.log(data);
                    }
                }
            ]

            GeneralBlock {
                id: generalShare
            }
            //! [0]
        }
    }
    Tab {
        title: qsTr("Facebook")
        imageSource: "asset:///images/ic_facebook.png"
        Page {
            id: facebookTab
            actions: [
                //! [1]
                // Facebook - update status
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Update Status")
                    query {
                        mimeType: "text/plain"
                        invokeTargetId: "Facebook"
                        invokeActionId: "bb.action.SHARE"
                    }
                    onTriggered: {
                        data = _socialInvocation.encodeQString(fbShare.statusText);
                        console.log(data);
                    }
                },

                // Facebook - share URL
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Post URL")
                    query {
                        invokeTargetId: "Facebook"
                        invokeActionId: "bb.action.SHARE"
                        uri: "http://www.blackberry.com"
                    }
                },

                // Facebook - share an image from the asset directory
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Post Image")
                    query {
                        mimeType: "image/jpeg"
                        invokeTargetId: "Facebook"
                        invokeActionId: "bb.action.SHARE"
                        // Share an image that is packaged with the app
                        uri: _dirPaths.asset + "images/MissScarlett.png"
                    }
                }
                //! [1]
            ]
            titleBar: TitleBar {
                title: qsTr("Social Invocation - Facebook")
            }
            GeneralBlock {
                id: fbShare
                statusLabel: qsTr("Set your Facebook Status")

                Divider {
                }

                // Invoke Facebook to a specified profile page
                Label {
                    text: qsTr("View a Facebook Profile")
                    textStyle.fontWeight: FontWeight.Bold
                }
                RadioGroup {
                    id: fbGroup
                    Option {
                        id: optionUser
                        text: qsTr("User Profile")
                        selected: true
                        // The flag "user" is passed to the Facebook app
                        // to identify what type of page to access
                        value: "user"
                    }
                    Option {
                        id: optionBusiness
                        text: qsTr("Business page")
                        // The flag "page" is passed to the Facebook app
                        // to identify what type of page to access
                        value: "page"
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    TextField {
                        id: txtFBProfile
                        hintText: qsTr("Enter numeric Facebook id")
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.75
                        }
                    }
                    //! [2]
                    Button {
                        text: qsTr("GO!")
                        onClicked: {
                            _socialInvocation.invoke("com.rim.bb.app.facebook", "bb.action.OPEN",
                                                     fbGroup.selectedValue, txtFBProfile.text)
                        }
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.25
                        }
                    }
                    //! [2]
                }

                Divider {
                }

                Label {
                    text: qsTr("Share an image on Facebook")
                    textStyle.fontWeight: FontWeight.Bold
                }

                //! [3]
                ImageSelectionBlock {
                    invokeTarget: "Facebook"
                }
                //! [3]
            }
        }
    }
    Tab {
        title: qsTr("Twitter")
        imageSource: "asset:///images/ic_twitter.png"
        Page {
            actions: [
                //! [4]
                // Twitter - tweet
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Tweet")
                    query {
                        mimeType: "text/plain"
                        invokeTargetId: "Twitter"
                        invokeActionId: "bb.action.SHARE"
                    }
                    onTriggered: {
                        data = _socialInvocation.encodeQString(twShare.statusText);
                        console.log(data);
                    }
                },

                // Twitter - tweet URL
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Post URL")
                    query {
                        invokeTargetId: "Twitter"
                        invokeActionId: "bb.action.SHARE"
                        uri: "http://www.blackberry.com"
                    }
                },

                // Twitter - tweet image
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Post Image")
                    query {
                        mimeType: "image/png"
                        invokeTargetId: "Twitter"
                        invokeActionId: "bb.action.SHARE"
                        // Share an image from the user's shared folder
                        uri: _dirPaths.camera + "IMG_00000004.jpg"
                    }
                }
                //! [4]
            ]
            titleBar: TitleBar {
                title: qsTr("Social Invocation - Twitter")
            }
            GeneralBlock {
                id: twShare
                statusLabel: qsTr("Compose a Tweet")

                // Invoke Twitter to a specified profile page
                Label {
                    text: qsTr("View a Twitter Profile")
                    textStyle.fontWeight: FontWeight.Bold
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    //! [5]
                    TextField {
                        id: txtProfile
                        hintText: qsTr("Enter Twitter handle")
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.75
                        }
                    }
                    Button {
                        text: qsTr("GO!")
                        onClicked: {
                            _socialInvocation.invoke("com.twitter.urihandler", "bb.action.VIEW",
                                                     "", "twitter:connect:" + txtProfile.text)
                        }
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.25
                        }
                    }
                    //! [5]
                }

                // Invoke Twitter to the search results page
                Label {
                    text: qsTr("Search Twitter")
                    textStyle.fontWeight: FontWeight.Bold
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    //! [6]
                    TextField {
                        id: txtSearch
                        hintText: qsTr("Enter search term")
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.75
                        }
                    }
                    Button {
                        text: qsTr("GO!")
                        onClicked: {
                            _socialInvocation.invoke("com.twitter.urihandler", "bb.action.VIEW",
                                                     "", "twitter:search:" + txtSearch.text)
                        }
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 0.25
                        }
                    }
                    //! [6]
                }

                Label {
                    text: qsTr("Share an image on Twitter")
                    textStyle.fontWeight: FontWeight.Bold
                }

                //! [7]
                ImageSelectionBlock {
                    invokeTarget: "Twitter"
                }
                //! [7]
            }
        }
    }
    Tab {
        title: qsTr("LinkedIn")
        imageSource: "asset:///images/ic_linked_in.png"
        Page {
            actions: [
                //! [8]
                // LinkedIn - update status
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("Update Status")
                    query {
                        mimeType: "text/plain"
                        invokeTargetId: "LinkedIn"
                        invokeActionId: "bb.action.SHARE"
                    }
                    onTriggered: {
                        data = _socialInvocation.encodeQString(liShare.statusText);
                        console.log(data);
                    }
                },

                // LinkedIn - invoke to profile
                ActionItem {
                    ActionBar.placement: ActionBarPlacement.OnBar
                    title: qsTr("View Profile")
                    onTriggered: {
                        // URI format is "linkedin:contact:" + public URL that you can find
                        // on someone's LinkedIn Profile Page, generally of the form:
                        // www.linkedin.com/pub/firstname-lastname/xx/yyy/zzz/
                        //
                        // Please note that currently (10.0.0.31) the LinkedIn app is only
                        // able to view the profile of a 1st, 2nd, or 3rd degree connection
                        // of the signed-in user.  For connections beyond that distance,
                        // LinkedIn will display a permission error.
                        //
                        // Add your LinkedIn URL to the fourth parameter below
                        _socialInvocation.invoke("com.linkedin.urihandler", "bb.action.VIEW",
                                                 "", "linkedin:contact:" + "")
                    }
                }
                //! [8]
            ]
            titleBar: TitleBar {
                title: qsTr("Social Invocation - LinkedIn")
            }
            GeneralBlock {
                id: liShare
                statusLabel: qsTr("Share an Update:")
            }
        }
    }
}
