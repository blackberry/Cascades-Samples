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
#include "bucketdata/bucketmodel.h"
#include "bucketdata/bucketsettings.h"
#include "bucketbbm/bucketbbmmanager.h"

#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;
using namespace bucketbbm;

BucketListApp::BucketListApp()
{
    // Set the application organization and name, which is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Bucket List Settings");

    // The model for populating the bucket list is registered, so that it and all its
    // properties can be accessed directly from QML. This is done before creating the
    // QmlDocument below so that it is available when the corresponding QML component
    // is needed (see main.qml).
    qmlRegisterType<BucketModel>("com.bucketlist.bucketdata", 1, 0, "BucketModel");

    // The application settings object used to store the BBM connection state
    qmlRegisterType<BucketSettings>("com.bucketlist.bucketdata", 1, 0, "BucketSettings");

    // The BBM manager that can connect the application to BBM and update the BBM status message
    qmlRegisterType<BucketBBMManager>("com.bucketlist.bucketbbm", 1, 0, "BucketBBMManager");

    // Create a QMLDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    if (!qml->hasErrors()) {

        AbstractPane *appPage = qml->createRootObject<AbstractPane>();

        if (appPage) {
            // Set the main scene to the application Page.
            Application::instance()->setScene(appPage);
        }
    }
}
