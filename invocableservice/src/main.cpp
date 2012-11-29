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

#include "AppUI.hpp"
#include "Service.hpp"

#include <bb/cascades/Application>

#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include <iostream>

using namespace std;

using ::bb::cascades::Application;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    qDebug() << "*main called in main.cpp";
    cout << "*main called in main.cpp" << endl;

    Application app(argc, argv);

    bool isService = false;

    const QStringList arguments = app.arguments();
    const QString appName = arguments.at(0);

    // if argc has more than 1 element
    if (arguments.count() > 1) {
        const QString parameter = arguments.at(1);

        qDebug() << "Startup arguments(>1): " << appName << ", " << parameter;
        cout << "Startup arguments: " << appName.toStdString() << ", " << parameter.toStdString() << endl;

        // launch service if parameter is "service_startup"
        if (parameter == QLatin1String("service_startup")) {
            isService = true;
        }
    } else {
        qDebug() << "Startup arguments: " << appName;
        cout << "Startup arguments: " << appName.toStdString() << endl;
    }


    // if launching service
    if (isService) {
        // call constructor for service to preparing for playing sound, flashing LED, or vibrating
        new Service();
    } else { // if launching app
        // call app constructor which initializes UI such as buttons and labels
        new AppUI();
    }

    return Application::exec();
}
