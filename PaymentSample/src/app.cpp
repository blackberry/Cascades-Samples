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

#include "app.hpp"
#include "PaymentServiceControl.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ForeignWindow>

#include <bb/platform/PaymentService>

#include <QTimer>
#include <QtDeclarative/QtDeclarative>
#include <QVariant>

#include <iostream>

using namespace bb::cascades;
using namespace bb::platform;

App::App()
{
    // Since this is a sample application and is not affiliated with an actual application on
    // BlackBerry App World (and thus does not have any real digital goods), set the connection
    // mode to test.
    PaymentService::getInstance().setConnectionMode(PaymentService::ConnectionModeTest);

    // Get the window group ID and pass it to the PaymentService instance.
    QString windowGroupId = ForeignWindow::mainWindowGroupId();
    PaymentService::getInstance().setWindowGroupId(windowGroupId);

    // Register our class that wraps the C++ PaymentService interface with QML so that we
    // can make calls into the PaymentService and get results, through QML.
    qmlRegisterType<PaymentServiceControl>("com.sample.payment", 1, 0, "PaymentServiceControl");
    qmlRegisterType<QTimer>("bb.cascades", 1, 0, "QTimer");

    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create(this, "main.qml");

    if (!qml->hasErrors()) {
        // The application AbstractPane is created from QML.
        AbstractPane  *absPane = qml->createRootNode<AbstractPane>();

        if (absPane) {
            qml->setContextProperty("_absPane", absPane);

            // Finally the main scene for the application is set.
            Application::setScene(absPane);
        }
    }
}
