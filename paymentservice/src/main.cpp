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

#include "PaymentServiceControl.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <bb/platform/PaymentManager>

#include <QtCore/QLocale>
#include <QtCore/QTimer>
#include <QtCore/QTranslator>

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

    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString("paymentservice_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    // Since this is a sample application and is not affiliated with an actual application on
    // BlackBerry App World (and thus does not have any real digital goods), set the connection
    // mode to test.
    PaymentManager::setConnectionMode(PaymentConnectionMode::Test);

    // Register our class that wraps the C++ PaymentService interface with QML so that we
    // can make calls into the PaymentService and get results, through QML.
    qmlRegisterType<PaymentServiceControl>("com.sample.payment", 1, 0, "PaymentServiceControl");
    qmlRegisterType<QTimer>("bb.cascades", 1, 0, "QTimer");

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    qml->setContextProperty("_absPane", appPage);
    Application::instance()->setScene(appPage);

    return Application::exec();
}

