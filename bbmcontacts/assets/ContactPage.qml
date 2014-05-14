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

import bb.cascades 1.3
import com.example.contact 1.0

// Page for displaying Contact details
Page {

    property Contact contact

    ContactScrollView {
        Container {

            layout: DockLayout {
            }

            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                imageSource: "asset:///images/background_blurred.png"
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                topPadding: ui.du(2.2)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)

                // Container for Contact avatar image
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    //! [0]
                    ImageView {
                        preferredHeight: ui.du(33.3)
                        preferredWidth: ui.du(33.3)

                        image: contact ? contact.avatar : null
                    }
                    //! [0]
                }

                Divider {
                }
                
                // Container to display Contact availability
                Container {
                    minHeight: ui.du(5.6)

                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    //! [1]
                    ImageView {
                        verticalAlignment: VerticalAlignment.Center

                        imageSource: "images/busy.png"
                        visible: contact ? contact.busy : false
                    }

                    Label {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }

                        text: contact ? contact.displayName : "n/a"
                        textStyle {
                            color: Color.White
                            fontWeight: FontWeight.Bold
                        }
                    }
                    //! [1]
                }

                // A set of custom fields to display contact details such as,
                // display name, handle, etc.
                //! [2]
                Field {
                    title: qsTr("status message")
                    value: contact ? contact.statusMessage : "n/a"
                }
                //! [2]
                Field {
                    title: qsTr("personal message")
                    value: contact ? contact.personalMessage : "n/a"
                }

                Field {
                    title: qsTr("pp id")
                    value: contact ? contact.ppid : "n/a"
                }

                Field {
                    title: qsTr("app version")
                    value: contact ? contact.appVersion : "n/a"
                }

                Field {
                    title: qsTr("handle")
                    value: contact ? contact.handle : "n/a"
                }

                Field {
                    title: qsTr("platform version")
                    value: contact ? contact.platformVersion : "n/a"
                }
            }
        }
    }
}
