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

#include "Flashlight.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/device/VibrationController>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;
using namespace bb::device;

int main(int argc, char **argv)
{
    Application app(argc, argv);

//! [0]
    qmlRegisterType<VibrationController>("bb.vibrationController", 1, 0, "VibrationController");
    qmlRegisterType<Flashlight>("Flashlight", 1, 0, "Flashlight");
//! [0]

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString::fromLatin1("SensorDemo_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    app.setScene(root);

    return Application::exec();
}
