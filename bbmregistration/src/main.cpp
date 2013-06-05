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

#include "App.hpp"
#include "RegistrationHandler.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

int main(int argc, char **argv)
{
    // Create a Cascades application.
    Application app(argc, argv);
    // Configure localization support.
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename =
        QString::fromLatin1("bbmregistration_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }
    // Every application is required to have its own unique UUID. You should
    // keep using the same UUID when you release a new version of your
    // application.
    // TODO:  YOU MUST CHANGE THIS UUID!
    // You can generate one here: http://www.guidgenerator.com/
    const QUuid uuid(QLatin1String(""));

//! [0]
    // Register with BBM.
    RegistrationHandler registrationHandler(uuid, &app);
//! [0]

    return Application::exec();
}
