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
#include <bb/system/SystemCredentialsPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiError>
#include <bb/system/SystemUiInputField>
#include <bb/system/SystemUiInputMode>
#include <bb/system/SystemUiPosition>
#include <bb/system/SystemUiResult>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    qmlRegisterType<bb::system::SystemUiButton>("bb.system", 1, 0, "SystemUiButton");
    qmlRegisterType<bb::system::SystemUiInputField>("bb.system", 1, 0, "SystemUiInputField");
    qmlRegisterType<bb::system::SystemToast>("bb.system", 1, 0, "SystemToast");
    qmlRegisterType<bb::system::SystemPrompt>("bb.system", 1, 0, "SystemPrompt");
    qmlRegisterType<bb::system::SystemCredentialsPrompt>("bb.system", 1, 0, "SystemCredentialsPrompt");
    qmlRegisterType<bb::system::SystemDialog>("bb.system", 1, 0, "SystemDialog");
    qmlRegisterUncreatableType<bb::system::SystemUiError>("bb.system", 1, 0, "SystemUiError", "");
    qmlRegisterUncreatableType<bb::system::SystemUiResult>("bb.system", 1, 0, "SystemUiResult", "");
    qmlRegisterUncreatableType<bb::system::SystemUiPosition>("bb.system", 1, 0, "SystemUiPosition", "");
    qmlRegisterUncreatableType<bb::system::SystemUiInputMode>("bb.system", 1, 0, "SystemUiInputMode", "");
    qmlRegisterUncreatableType<bb::system::SystemUiModality>("bb.system", 1, 0, "SystemUiModality", "");
    qRegisterMetaType<bb::system::SystemUiResult::Type>("bb::system::SystemUiResult::Type");

    Application app(argc, argv);

    // localization support
    QTranslator translator;
    const QString locale_string = QLocale().name();
    const QString filename = QString("dialogs_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    return Application::exec();
}
