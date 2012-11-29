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

#include "LocationDiagnostics.hpp"
#include "LocationSession.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // Register custom type at QML runtime environment
    qmlRegisterType<LocationSession>();

    Application app(argc, argv);

    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store.
    app.setOrganizationName("Shadid Haque");
    app.setApplicationName("Location Diagnostics");

    // Create the locationDiagnostics object
    LocationDiagnostics locationDiagnostics;

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    // Make the LocationDiagnostics object available to the UI as context property
    qml->setContextProperty("_locationDiagnostics", &locationDiagnostics);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    Application::instance()->setAutoExit(false);

    return Application::exec();
}
