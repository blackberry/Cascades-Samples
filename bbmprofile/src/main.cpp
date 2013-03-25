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

#include <bb/cascades/Application>
#include <bb/cascades/pickers/FilePicker>

#include <QLocale>
#include <QTranslator>

#include "Profile.hpp"
#include "ProfileEditor.hpp"
#include "RegistrationHandler.hpp"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString::fromLatin1("bbmprofile_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    qmlRegisterType<ProfileEditor>();
    qmlRegisterType<bb::cascades::pickers::FilePicker>("bb.cascades.pickers", 1, 0, "FilePicker");

    //TODO: Define your own UUID here. You can generate one here: http://www.guidgenerator.com/
    const QString uuid(QLatin1String(""));

//! [0]
    RegistrationHandler *registrationHandler = new RegistrationHandler(uuid, &app);

    Profile *profile = new Profile(registrationHandler->context(), &app);

    QObject::connect(registrationHandler, SIGNAL(registered()), profile, SLOT(show()));
//! [0]

    return Application::exec();
}
