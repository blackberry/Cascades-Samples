/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
import com.example.bb10samples.pim.accounts 1.0

Page {
    id: root

    //! [0]
    onCreationCompleted: {
        _accounts.accountEditor.form = form
    }
    //! [0]

    //! [1]
    titleBar: TitleBar {
        id: pageTitleBar

        // The 'Create/Save' action
        acceptAction: ActionItem {
            title: (_accounts.accountEditor.mode == AccountEditor.CreateMode ? qsTr ("Create" ) : qsTr ("Save"))

            onTriggered: {
                _accounts.accountEditor.saveAccount()

                navigationPane.pop()
            }
        }

        // The 'Cancel' action
        dismissAction: ActionItem {
            title: qsTr ("Cancel")

            onTriggered: navigationPane.pop()
        }
    }
    //! [1]

    Container {
        layout: DockLayout {}

        // The background image
        BackgroundImage {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }

        Container {
            leftPadding: ui.du(5)
            topPadding: ui.du(5)
            rightPadding: ui.du(5)

            DropDown {
                visible: (_accounts.accountEditor.mode == AccountEditor.CreateMode)

                Option {
                    text: qsTr ("ActiveSync")
                    value: "activesync"
                }

                Option {
                    text: qsTr ("BBM Groups")
                    value: "bbgroups"
                }

                Option {
                    text: qsTr ("BBM")
                    value: "bbm"
                }

                Option {
                    text: qsTr ("CalDAV")
                    value: "caldav"
                }

                Option {
                    text: qsTr ("Call Log")
                    value: "calllog"
                }

                Option {
                    text: qsTr ("CardDAV")
                    value: "carddav"
                }

                Option {
                    text: qsTr ("Evernote")
                    value: "evernote"
                }

                Option {
                    text: qsTr ("Facebook")
                    value: "facebook"
                }

                Option {
                    text: qsTr ("Gist")
                    value: "gist"
                }

                Option {
                    text: qsTr ("Iceberg")
                    value: "iceberg"
                }

                Option {
                    text: qsTr ("IMAP")
                    value: "imapemail"
                }

                Option {
                    text: qsTr ("LinkedIn")
                    value: "linkedin"
                }

                Option {
                    text: qsTr ("Local Calendar")
                    value: "localcalendar"
                }

                Option {
                    text: qsTr ("Local Contacts")
                    value: "localcontacts"
                }

                Option {
                    text: qsTr ("Local Geolocations")
                    value: "localgeolocation"
                }

                Option {
                    text: qsTr ("Local Memos")
                    value: "localmemos"
                }

                Option {
                    text: qsTr ("Local Notebook")
                    value: "localnotebook"
                }

                Option {
                    text: qsTr ("Local Tasks")
                    value: "localtask"
                }

                Option {
                    text: qsTr ("MMS")
                    value: "mms"
                }

                Option {
                    text: qsTr ("POP")
                    value: "popemail"
                }

                Option {
                    text: qsTr ("IM")
                    value: "retailim"
                }

                Option {
                    text: qsTr ("SIM Contacts")
                    value: "simcontacts"
                }

                Option {
                    text: qsTr ("SMS")
                    value: "sms"
                }

                Option {
                    text: qsTr ("SyncML")
                    value: "syncml"
                }

                Option {
                    text: qsTr ("Tags")
                    value: "tags"
                }

                Option {
                    text: qsTr ("TungleProvider")
                    value: "tungleprovider"
                }

                Option {
                    text: qsTr ("Twitter")
                    value: "twitter"
                }

                Option {
                    text: qsTr ("Unified Contacts")
                    value: "unifiedcontacts"
                }

                onSelectedValueChanged: _accounts.accountEditor.providerId = selectedValue
            }

            //! [2]
            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }

                Container {
                    id: form
                }
            }
            //! [2]
        }
    }
}
