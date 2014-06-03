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

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
//! [1]
    BTController btcontroller;

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui(btcontroller);
//! [1]

    return Application::exec();
}
