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
#include "hellocascadesapp.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

using namespace bb::cascades;

BucketListApp::BucketListApp()
{
    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("hellocascades.qml");

    if (!qml->hasErrors()) {

        // The application Page is created from QML.
        Page *appPage = qml->createRootNode<Page>();

        if (appPage) {
            // Finally the main scene for the application is set the Page.
            Application::setScene(appPage);
        }
    }
}
