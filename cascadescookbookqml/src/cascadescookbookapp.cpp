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

#include <bb/cascades/AbstractCover>
#include <bb/cascades/Container>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/SceneCover>
#include <bb/system/SystemToast>

CascadesCookbookApp::CascadesCookbookApp()
{
    // Create a QmlDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    if (!qml->hasErrors()) {

        // The application NavigationPane is created from QML.
        NavigationPane *navPane = qml->createRootObject<NavigationPane>();

        if (navPane) {
            qml->setContextProperty("_navPane", navPane);

            // Set the main scene for the application to the NavigationPane.
            Application::instance()->setScene(navPane);

            // Set the Cover for the application running in minimized mode
            addApplicationCover();
        }
    }
}

void CascadesCookbookApp::addApplicationCover() {
    // A small UI consisting of just an ImageView in a Container is set up
    // and used as the cover for the application when running in minimized mode.
    QmlDocument *qmlCover = QmlDocument::create("asset:///AppCover.qml").parent(this);

    if (!qmlCover->hasErrors()) {
        // Create the QML Container from using the QMLDocument.
        Container *coverContainer = qmlCover->createRootObject<Container>();

        // Create a SceneCover and set the application cover
        SceneCover *sceneCover = SceneCover::create().content(coverContainer);
        Application::instance()->setCover(sceneCover);
    }
}

CascadesCookbookApp::~CascadesCookbookApp()
{
}
