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

#include "applicationui.hpp"
#include <bb/cascades/Application>

#include <bb/platform/Notification>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationError>
#include <bb/platform/NotificationResult>

#include <bb/system/SystemUiButton>

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

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui;

    return Application::exec();
}
