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

CascadesCookbookApp::CascadesCookbookApp()
{
    // To set up the application cover its types needs to be registered.
    registerQMLTypes();

    // Create a QmlDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    if (!qml->hasErrors()) {

        // The application NavigationPane is created from QML.
        NavigationPane *navPane = qml->createRootObject<NavigationPane>();

        if (navPane) {
            qml->setContextProperty("_navPane", navPane);

            // Set the main scene for the application to the NavigationPane.
            Application::instance()->setScene(navPane);

        }
    }
}

void CascadesCookbookApp::registerQMLTypes()
{
    // The SceneCover is registered so that it can be used in QML, we register it to the bb.cascades namespace
    // since this is really something that belongs in cascades.
    qmlRegisterType<SceneCover>("bb.cascades", 1, 0, "SceneCover");

    // Since it is not possible to create an instance of the AbstractCover it is registered as an uncreatable type,
    // we need to register it since it is used when setting the application cover in QML (Application.cover).
    qmlRegisterUncreatableType<AbstractCover>("bb.cascades", 1, 0, "AbstractCover", "An AbstractCover can not be created.");
}

CascadesCookbookApp::~CascadesCookbookApp()
{
}
