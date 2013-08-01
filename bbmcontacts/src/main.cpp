/* Copyright (c) 2013 Research In Motion Limited.
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

#include <QLocale>
#include <QTranslator>

#include "RegistrationHandler.hpp"
#include "ContactsDisplay.hpp"
#include "Contact.hpp"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);
    qmlRegisterType<Contact>("com.example.contact", 1, 0, "Contact");

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString::fromLatin1("bbmprofilebox_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    //TODO: Define your own UUID here. You can generate one here: http://www.guidgenerator.com/

    const QString uuid(QLatin1String(""));

    //! [0]
    RegistrationHandler *registrationHandler = new RegistrationHandler(uuid, &app);

    ContactsDisplay *contactsDisplay = new ContactsDisplay(registrationHandler->context(), &app);

    // Whenever the registration has finished successfully, we continue to the main UI
    bool ok = QObject::connect(registrationHandler, SIGNAL(registered()), contactsDisplay, SLOT(show()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    //! [0]

    return Application::exec();
}
