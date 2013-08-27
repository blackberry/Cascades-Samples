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

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <bb/platform/Notification>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationError>
#include <bb/platform/NotificationResult>

#include <bb/system/SystemUiButton>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
//! [0]
    qmlRegisterType<bb::system::SystemUiButton>("bb.system", 1, 0, "SystemUiButton");
    qmlRegisterType<bb::platform::Notification>("bb.platform", 1, 0, "Notification");
    qmlRegisterType<bb::platform::NotificationDialog>("bb.platform", 1, 0, "NotificationDialog");
    qmlRegisterUncreatableType<bb::platform::NotificationError>("bb.platform", 1, 0, "NotificationError", "");
    qmlRegisterUncreatableType<bb::platform::NotificationResult>("bb.platform", 1, 0, "NotificationResult", "");
//! [0]

    Application app(argc, argv);

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString("notifications_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);

    // Build the path, add it as a context property, and expose
    // it to QML
    const QString workingDir = QDir::currentPath();
    const QString dirPaths = QString::fromLatin1("file://%1/app/public/").arg(workingDir);
    qml->documentContext()->setContextProperty("_publicDir", dirPaths);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    return Application::exec();
}
