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

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <QLocale>
#include <QTranslator>

#include "BTController.hpp"
#include "ChatManager.hpp"
#include "DeviceListing.hpp"
#include "LocalDeviceInfo.hpp"
#include "RemoteDeviceInfo.hpp"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
//! [0]
    qmlRegisterType<ChatManager>();
    qmlRegisterType<DeviceListing>();
    qmlRegisterType<LocalDeviceInfo>();
    qmlRegisterType<RemoteDeviceInfo>();
//! [0]

    Application app(argc, argv);

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString::fromLatin1("bluetoothsppchat_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
//! [1]
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);
    qml->setContextProperty("_btController", new BTController(&app));
//! [1]

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // set created root object as a scene
    app.setScene(root);

    return Application::exec();
}
