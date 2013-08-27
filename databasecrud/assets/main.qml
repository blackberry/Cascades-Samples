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

import bb.cascades 1.0

// The root node has to inherit from AbstractPane -- in this case a TabbedPane.
// There is always only ONE root not in a QML file and it is typically
// an AbstractPane object like Page.

TabbedPane {
    id: root

    property bool databaseOpen: false

    showTabsOnActionBar: true
    //! [0]
    Tab {
        title: qsTr("Retrieve")
        imageSource: "asset:///images/retrieve.png"

        PageBase {
            databaseOpen: root.databaseOpen
            page: "RetrievePage.qml"
        }
    }

    Tab {
        title: qsTr("Create")
        imageSource: "asset:///images/create.png"

        PageBase {
            databaseOpen: root.databaseOpen
            page: "CreatePage.qml"
        }
    }

    Tab {
        title: qsTr("Update")
        imageSource: "asset:///images/update.png"

        PageBase {
            databaseOpen: root.databaseOpen
            page: "UpdatePage.qml"
        }
    }

    Tab {
        title: qsTr("Delete")
        imageSource: "asset:///images/delete.png"

        PageBase {
            databaseOpen: root.databaseOpen
            page: "DeletePage.qml"
        }
    }
    //! [0]
}
