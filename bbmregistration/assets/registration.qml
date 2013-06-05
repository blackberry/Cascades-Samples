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

NavigationPane {
    id: navigationPane

    Page {
        id: page

        Container {
    //! [0]
            onCreationCompleted: {
                navigationPane.push(page);
                _registrationHandler.registerApplication();
            }
    //! [0]

            layout: DockLayout {}

            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                imageSource: "asset:///images/background.png"
            }

    //! [1]
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    text: _registrationHandler.statusMessage
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.White
                    }
                    multiline: true
                }

                Button {
                    horizontalAlignment: HorizontalAlignment.Center

                    visible: _registrationHandler.temporaryError
                    text: qsTr("Connect to BBM")
                    onClicked: {
                        _registrationHandler.registerApplication()
                    }
                }
                // Display the main page if the user chooses to Continue
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    visible: _registrationHandler.allowed
                    text: qsTr("Continue")
                    onClicked: {
                        var mainPage = mainComponent.createObject();
                        navigationPane.push(mainPage);

                    }
                }
            }
    //! [1]
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: mainComponent
            source: "main.qml"
        }
    ]
}
