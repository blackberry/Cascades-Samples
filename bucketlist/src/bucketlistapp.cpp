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
#include "bucketlistapp.h"
#include "bucketmodel.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

BucketListApp::BucketListApp()
{
    // The model for populating the bucket list is registered, so that it and all its
    // properties can be accessed directly from QML. This is done before creating the
    // QmlDocument below so that it is available when the corresponding QML component
    // is needed (see main.qml).
    qmlRegisterType<BucketModel>("com.bucketlist.bucketmodel", 1, 0, "BucketModel");

    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("main.qml");

    if (!qml->hasErrors()) {

      AbstractPane *appPage = qml->createRootNode<AbstractPane>();

        if (appPage) {
            // Finally the main scene for the application is set the Page.
            Application::instance()->setScene(appPage);
        }
    }
}
