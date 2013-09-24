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

// The first line imports the default Cascades UI library.
// This is the minimum you require.
import bb.cascades 1.0

// The root node has to inherit from AbstractPane -- in this case a Page.
// There is always only ONE root not in a QML file and it is typically
// an AbstractPane object

Page {
    id: root

    property bool databaseOpen: false

    titleBar: TitleBar {
        title: qsTr("Database Creation Sample")
    }

    // A Page contains a single control for its content -- in this case a container.
    Container {
        layout: DockLayout {
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            leftPadding: 30
            rightPadding: 30

            layout: StackLayout {
            }

            //! [0]
            // Four buttons to call each of the functions in app.
            Button {
                horizontalAlignment: HorizontalAlignment.Fill

                text: qsTr("Create/Register Database")

                onClicked: {
                    root.databaseOpen = _app.createDatabase()
                }
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Fill

                text: qsTr("Create Table")
                enabled: root.databaseOpen

                onClicked: {
                    _app.createTable()
                }
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Fill

                text: qsTr("Create Table Asynchronously")
                enabled: root.databaseOpen

                onClicked: {
                    _app.createTableAsync()
                }
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Fill

                text: qsTr("Drop Table")
                enabled: root.databaseOpen

                onClicked: {
                    _app.dropTable()
                }
            }

            Button {
                horizontalAlignment: HorizontalAlignment.Fill

                text: qsTr("Create Record")
                enabled: root.databaseOpen

                onClicked: {
                    _app.createRecord("John", "Doe")
                }
            }
            //! [0]
        }
    }
}
