/* Copyright (c) 2012 Research In Motion Limited.
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
    id: tabbedPane

    showTabsOnActionBar: true

    function selectReceiverTab()
    {
        _session.activeModule = 2
        activeTab = receiverTab
    }

    // Make the 'Receiver' Tab the visible one if we receive an NDEF message
    onCreationCompleted: _nfcReceiver.messageReceived.connect(selectReceiverTab)

    activePane: Page {
        ControlDelegate {
            delegateActive: true
            source: _session.activeModule == 0 ? "MacAddressPage.qml" :
                    _session.activeModule == 1 ? "SenderPage.qml" :
                    _session.activeModule == 2 ? "ReceiverPage.qml" :
                    _session.activeModule == 3 ? "SharePage.qml" : ""
        }
    }

    Tab {
        title: qsTr("MAC Address")
        imageSource: "asset:///images/ic_info.png"

        onTriggered: _session.activeModule = 0
    }

    Tab {
        title: qsTr("Sender")
        imageSource: "asset:///images/ic_send.png"

        onTriggered: _session.activeModule = 1
    }

    Tab {
        id: receiverTab

        title: qsTr("Receiver")
        imageSource: "asset:///images/ic_receive.png"

        onTriggered: _session.activeModule = 2
    }

    Tab {
        title: qsTr("Share")
        imageSource: "asset:///images/ic_share.png"

        onTriggered: _session.activeModule = 3
    }
}

