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
#include "applicationui.hpp"
#include "PaymentServiceControl.hpp"

#include <bb/cascades/Application>
#include <bb/platform/PaymentManager>

#include <QtCore/QTimer>

using namespace bb::cascades;
using namespace bb::platform;

/**
 * This sample application shows some basic
 * usage of the PaymentService API, such as buying items, querying item price,
 * querying the subscription terms, canceling subscription, canceling purchase and
 * querying purchases made.
 */
Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Since this is a sample application and is not affiliated with an actual application on
    // BlackBerry App World (and thus does not have any real digital goods), set the connection
    // mode to test.
    PaymentManager::setConnectionMode(PaymentConnectionMode::Test);

    // Register our class that wraps the C++ PaymentService interface with QML so that we
    // can make calls into the PaymentService and get results, through QML.
    qmlRegisterType<PaymentServiceControl>("com.sample.payment", 1, 0, "PaymentServiceControl");
    qmlRegisterType<QTimer>("com.sample.payment", 1, 0, "QTimer");

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui;

    return Application::exec();
}

