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
#include "cascadescookbookapp.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/NavigationPane>

CascadesCookbookApp::CascadesCookbookApp()
{
    // Create a QMLDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    if (!qml->hasErrors()) {

        // The application NavigationPane is created from QML.
        NavigationPane  *navPane = qml->createRootObject<NavigationPane>();

        if (navPane) {
            qml->setContextProperty("_navPane", navPane);

            // Set the main scene for the application to the NavigationPane.
            Application::instance()->setScene(navPane);
        }
    }
}

CascadesCookbookApp::~CascadesCookbookApp()
{
}
